#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "defines.h"
#include "board.h"
#include "highscore.h"

#define FPS 30

struct game
{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Board *pBoard;
    Pair HighScores[HIGHSCORE_MAX_SAVES];
    int score;
    int level;
    int lines;
    int gravity;
};
typedef struct game Game;

const SDL_Color White = {255, 255, 255, 255};
TTF_Font *arial;

int initiate(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(Game *pGame, const uint8_t *keysPressed);
SDL_Rect ShowText(SDL_Renderer *pRenderer, char text[], SDL_Rect rect);
void DrawGameUI(Game *pGame);

int main(int argv, char **args)
{

    Game g = {0};
    if (!initiate(&g))
        return 1;
    run(&g);
    close(&g);

    return 0;
}

int initiate(Game *pGame)
{
    srand(time(NULL));
    rand();

    if (TTF_Init() == -1)
        printf("FONT ERROR\n");
    arial = TTF_OpenFont("./assets/BigBlueTermPlusNerdFont-Regular.ttf", 25);
    if (!arial)
    {
        printf("COULD NOT LOAD FONT\n");
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 0;
    }
    pGame->pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!pGame->pWindow)
    {
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!pGame->pRenderer)
    {
        printf("Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->pBoard = CreateBoard(pGame->pRenderer);
    pGame->level = 0;
    pGame->lines = 0;
    pGame->score = 0;
    pGame->gravity = pGame->level;
    return 1;
}

void run(Game *pGame)
{

    LoadHighscore(pGame->HighScores);

    bool close_requested = false;
    SDL_Event event;
    const uint8_t *keyPressed = SDL_GetKeyboardState(NULL);
    int frameCounter = 0;

    while (!close_requested)
    {
        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pGame->pRenderer);
        SDL_SetRenderDrawColor(pGame->pRenderer, 230, 230, 230, 255);
        frameCounter++;
        if (GameOverCheck(pGame->pBoard))
        {
            close_requested = true;
            continue;
        }
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                close_requested = 1;
            else if (event.key.type == SDL_KEYDOWN)
            {
                handleInput(pGame, keyPressed);
            }
            else if (event.key.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                pGame->gravity = pGame->level;
            }
        }

        if (isTetrinoOnGround(pGame->pBoard))
        {
            NextRound(pGame->pBoard, &pGame->score, &pGame->level, &pGame->lines);
            pGame->gravity = pGame->level;
        }
        else if (frameCounter % (FPS - ((pGame->gravity >= FPS) ? FPS - 1 : pGame->gravity)) == 0)
        {
            MoveDown(pGame->pBoard);
        }

        DrawTetrino(pGame->pBoard);
        DrawOccupied(pGame->pBoard);
        DrawGameUI(pGame);

        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(1000 / FPS);
    }
}

void handleInput(Game *pGame, const uint8_t *keysPressed)
{

    if (( keysPressed[SDL_SCANCODE_A] && !keysPressed[SDL_SCANCODE_LEFT]) ||
        (!keysPressed[SDL_SCANCODE_A] &&  keysPressed[SDL_SCANCODE_LEFT]))
    {
        MoveSideways(pGame->pBoard, -1);
    }
    if (( keysPressed[SDL_SCANCODE_D] && !keysPressed[SDL_SCANCODE_RIGHT]) ||
        (!keysPressed[SDL_SCANCODE_D] &&  keysPressed[SDL_SCANCODE_RIGHT]))
    {
        MoveSideways(pGame->pBoard, +1);
    }
    if (( keysPressed[SDL_SCANCODE_W] && !keysPressed[SDL_SCANCODE_UP]) ||
        (!keysPressed[SDL_SCANCODE_W] &&  keysPressed[SDL_SCANCODE_UP]))
    {
        RotateClockwise(pGame->pBoard);
    }
    if (( keysPressed[SDL_SCANCODE_S] && !keysPressed[SDL_SCANCODE_DOWN]) ||
        (!keysPressed[SDL_SCANCODE_S] &&  keysPressed[SDL_SCANCODE_DOWN]))
    {
        RotateAntiClockwise(pGame->pBoard);
    }
    if (keysPressed[SDL_SCANCODE_SPACE])
    {
        pGame->gravity = FPS;
    }
}

void close(Game *pGame)
{
    if (pGame->pRenderer)
        SDL_DestroyRenderer(pGame->pRenderer);
    if (pGame->pWindow)
        SDL_DestroyWindow(pGame->pWindow);
    if (arial)
        TTF_CloseFont(arial);
    InsertScore(pGame->HighScores,"theodor",pGame->score);
    SaveHighscore(pGame->HighScores);
    SDL_Quit();
}
SDL_Rect ShowText(SDL_Renderer *pRenderer, char text[], SDL_Rect rect)
{
    SDL_Surface *surface = TTF_RenderText_Solid(arial, text, White);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(pRenderer, surface);
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_FreeSurface(surface);
    SDL_RenderCopy(pRenderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    return rect;
}

void DrawGameUI(Game *pGame)
{
    char textBuffer[100];

    SDL_Rect linesRect;
    linesRect.x = BOARD_X + BOARD_WIDTH / 2 - 153 / 2;
    linesRect.y = BOARD_Y - 50;

    // Lines
    sprintf(textBuffer, "Lines-%03d", pGame->lines);
    linesRect = ShowText(pGame->pRenderer, textBuffer, linesRect);

    SDL_Rect rightTextRect;
    // Score
    rightTextRect.x = BOARD_X + BOARD_WIDTH + 20;
    rightTextRect.y = BOARD_Y;
    sprintf(textBuffer, "Score");
    rightTextRect = ShowText(pGame->pRenderer, textBuffer, rightTextRect);
    rightTextRect.y += rightTextRect.h;
    sprintf(textBuffer, "%06d", pGame->score);
    rightTextRect = ShowText(pGame->pRenderer, textBuffer, rightTextRect);

    // Next piece
    rightTextRect.y += rightTextRect.h + 20;
    rightTextRect = ShowText(pGame->pRenderer, "Next:", rightTextRect);
    ShowNextPiece(pGame->pBoard, rightTextRect.x, rightTextRect.y + rightTextRect.h);

    // Level
    rightTextRect.y += 5 * TETRINOSIZE;
    sprintf(textBuffer, "Level");
    rightTextRect = ShowText(pGame->pRenderer, textBuffer, rightTextRect);
    rightTextRect.y += rightTextRect.h;
    sprintf(textBuffer, "%4d", pGame->level);
    ShowText(pGame->pRenderer, textBuffer, rightTextRect);
}
