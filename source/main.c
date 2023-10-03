#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include "defines.h"
#include "utils.h"
#include "board.h"
#include "highscore.h"
#include "special.h"

#define FPS 30

enum gamestate
{
    PLAY,
    GAMEOVER,
    QUIT,
    MENU
};
typedef enum gamestate GameState;

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
    GameState state;
};
typedef struct game Game;


int initGame(Game *pGame);
void runGame(Game *pGame);
void closeGame(Game *pGame);
void handleInput(Game *pGame, const uint8_t *keysPressed);
void initBoard(Game *pGame);
void mainMenu(Game *pGame);
void drawGameUI(Game *pGame);
void gameOverView(Game *pGame);



void mainMenu(Game *pGame)
{
    static SDL_Event event;

    static SDL_Rect startButtonRect;
    static int level = 0;
    startButtonRect.h = 75;
    startButtonRect.w = 200;
    startButtonRect.x = WINDOW_WIDTH / 2 - startButtonRect.w / 2;
    startButtonRect.y = WINDOW_HEIGHT / 2 - startButtonRect.h / 2;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    while (SDL_PollEvent(&event))
    {
        if (event.button.state == SDL_PRESSED && PointRectCollision(mouseX, mouseY, startButtonRect))
        {
            printfd("PRESSED\n");
            pGame->level = level;
            pGame->state = PLAY;
        }
        if (event.type == SDL_QUIT)
        {
            pGame->state = QUIT;
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_UP)
            {
                level += 1;
            }
            if (event.key.keysym.sym == SDLK_DOWN && level > 0)
            {
                level -= 1;
            }
        }
    }
    static SDL_Rect titleRect = {WINDOW_WIDTH/2,50};
    ShowText(pGame->pRenderer, White, FONT_SIZE, titleRect, true, "TETRIS");

    static SDL_Rect startLevelRect = {WINDOW_WIDTH/2,200};
    ShowText(pGame->pRenderer, White, FONT_SIZE, startLevelRect, true, "STARTING LEVEL: %2d",level);

    SDL_SetRenderDrawColor(pGame->pRenderer, 0, 200, 0, 255);
    SDL_RenderFillRect(pGame->pRenderer, &startButtonRect);

    startButtonRect.x = WINDOW_WIDTH / 2;
    startButtonRect.y = WINDOW_HEIGHT / 2 - 10;
    ShowText(pGame->pRenderer, White, FONT_SIZE, startButtonRect, true, "START");
}

int main(int argv, char **args)
{

    Game g = {0};
    if (!initGame(&g))
        return 1;
    while (g.state != QUIT)
    {
        SDL_SetRenderDrawColor(g.pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(g.pRenderer);
        if (g.state == PLAY)
        {
            initBoard(&g);
            runGame(&g);
        }
        else if (g.state == GAMEOVER)
        {
            gameOverView(&g);
        }
        else if (g.state == MENU)
        {
            mainMenu(&g);
        }
        SDL_RenderPresent(g.pRenderer);
        SDL_Delay(1000 / FPS);
    }

    closeGame(&g);

    return 0;
}

int initGame(Game *pGame)
{
    srand(time(NULL));
    rand();

    if (!InitFont("./assets/BigBlueTermPlusNerdFont-Regular.ttf")) {
        printfd("Error: FONT\n");
        return 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printfd("Error: %s\n", SDL_GetError());
        return 0;
    }
    pGame->pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!pGame->pWindow)
    {
        printfd("Error: %s\n", SDL_GetError());
        closeGame(pGame);
        return 0;
    }
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!pGame->pRenderer)
    {
        printfd("Error: %s\n", SDL_GetError());
        closeGame(pGame);
        return 0;
    }
    initBoard(pGame);
    pGame->state = MENU;
    LoadHighscore(pGame->HighScores);
    return 1;
}
void initBoard(Game *pGame)
{
    if (pGame->state == GAMEOVER)
    {
        free(pGame->pBoard);
    }
    pGame->pBoard = CreateBoard(pGame->pRenderer);
    pGame->lines = 0;
    pGame->score = 0;
    pGame->gravity = pGame->level;
}

void runGame(Game *pGame)
{

    bool close_requested = false;
    SDL_Event event;
    const uint8_t *keyPressed = SDL_GetKeyboardState(NULL);
    int frameCounter = 0;

    while (pGame->state == PLAY)
    {
        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pGame->pRenderer);
        SDL_SetRenderDrawColor(pGame->pRenderer, 230, 230, 230, 255);
        frameCounter++;
        if (GameOverCheck(pGame->pBoard))
        {
            close_requested = true;
            pGame->state = GAMEOVER;
            continue;
        }
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                close_requested = 1;
                pGame->state = QUIT;
            }
            else if (event.key.type == SDL_KEYDOWN)
            {
                handleInput(pGame, keyPressed);
            }
            else if (event.key.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                pGame->gravity = pGame->level;
            }
        }

        if (isTetrominoOnGround(pGame->pBoard))
        {
            NextRound(pGame->pBoard, &pGame->score, &pGame->level, &pGame->lines);
            pGame->gravity = pGame->level;
        }
        else if (frameCounter % (FPS - ((pGame->gravity >= FPS) ? FPS - 1 : pGame->gravity)) == 0)
        {
            MoveDown(pGame->pBoard);
        }

        DrawTetromino(pGame->pBoard);
        DrawOccupied(pGame->pBoard);
        drawGameUI(pGame);

        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(1000 / FPS);
    }
}

void handleInput(Game *pGame, const uint8_t *keysPressed)
{

    if ((keysPressed[SDL_SCANCODE_A] && !keysPressed[SDL_SCANCODE_LEFT]) ||
        (!keysPressed[SDL_SCANCODE_A] && keysPressed[SDL_SCANCODE_LEFT]))
    {
        MoveSideways(pGame->pBoard, -1);
    }
    if ((keysPressed[SDL_SCANCODE_D] && !keysPressed[SDL_SCANCODE_RIGHT]) ||
        (!keysPressed[SDL_SCANCODE_D] && keysPressed[SDL_SCANCODE_RIGHT]))
    {
        MoveSideways(pGame->pBoard, +1);
    }
    if ((keysPressed[SDL_SCANCODE_W] && !keysPressed[SDL_SCANCODE_UP]) ||
        (!keysPressed[SDL_SCANCODE_W] && keysPressed[SDL_SCANCODE_UP]))
    {
        RotateClockwise(pGame->pBoard);
    }
    if ((keysPressed[SDL_SCANCODE_S] && !keysPressed[SDL_SCANCODE_DOWN]) ||
        (!keysPressed[SDL_SCANCODE_S] && keysPressed[SDL_SCANCODE_DOWN]))
    {
        RotateAntiClockwise(pGame->pBoard);
    }
    if (keysPressed[SDL_SCANCODE_SPACE])
    {
        pGame->gravity = FPS;
    }
}

void closeGame(Game *pGame)
{
    if (pGame->pRenderer)
        SDL_DestroyRenderer(pGame->pRenderer);
    if (pGame->pWindow)
        SDL_DestroyWindow(pGame->pWindow);
    if (font)
        TTF_CloseFont(font);
    SaveHighscore(pGame->HighScores);
    SDL_Quit();
}


void drawGameUI(Game *pGame)
{
    char textBuffer[100];

    SDL_Rect linesRect = {BOARD_X + BOARD_WIDTH/2, BOARD_Y - 50};
    linesRect.x = BOARD_X + BOARD_WIDTH / 2;
    linesRect.y = BOARD_Y - 50;

    // Lines
    linesRect = ShowText(pGame->pRenderer, White, FONT_SIZE, linesRect, true, "Lines-%03d", pGame->lines);

    SDL_Rect rightTextRect = {BOARD_X + BOARD_WIDTH + 20,BOARD_Y};
    
    // Score
    rightTextRect = ShowText(pGame->pRenderer, White, FONT_SIZE, rightTextRect, false, "Score");
    rightTextRect.y += rightTextRect.h;
    rightTextRect = ShowText(pGame->pRenderer, White, FONT_SIZE, rightTextRect, false, "%06d", pGame->score);

    // Next piece
    rightTextRect.y += rightTextRect.h + 20;
    rightTextRect = ShowText(pGame->pRenderer, White, FONT_SIZE, rightTextRect, false, "Next:");
    ShowNextPiece(pGame->pBoard, rightTextRect.x, rightTextRect.y + rightTextRect.h);

    // Level
    rightTextRect.y += 5 * TETROMINOSIZE;

    rightTextRect = ShowText(pGame->pRenderer, White, FONT_SIZE, rightTextRect, false, "Level");

    rightTextRect.y += rightTextRect.h;

    ShowText(pGame->pRenderer, White, FONT_SIZE, rightTextRect, false, "%4d",pGame->level);
}


void gameOverView(Game *pGame)
{
    static SDL_Event event;
    static char nameBuffer[10] = "";
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            pGame->state = QUIT;
        }
        if (event.type == SDL_KEYDOWN)
        {
            TextInput(event, nameBuffer);
            if (event.key.keysym.sym == SDLK_RETURN && strlen(nameBuffer) > 1)
            {
                pGame->state = MENU;
                InsertScore(pGame->HighScores, nameBuffer, pGame->score);
                SaveHighscore(pGame->HighScores);
                LoadHighscore(pGame->HighScores);
            }
        }
    }
    static SDL_Rect nameRect = {WINDOW_WIDTH/2,200};
    static SDL_Rect gameOverRect = {WINDOW_WIDTH/2,50};
    static SDL_Rect scoreRect = {WINDOW_WIDTH/2,100};
    static SDL_Rect nameinputRect = {WINDOW_WIDTH/2,300};

    ShowText(pGame->pRenderer, White, FONT_SIZE, gameOverRect, true,  "GAME OVER");
    ShowText(pGame->pRenderer, White, FONT_SIZE, nameRect, true, "Type Name:");

    ShowText(pGame->pRenderer, White, FONT_SIZE, scoreRect, true, "Score: %d",pGame->score);

    ShowText(pGame->pRenderer, White, FONT_SIZE, nameRect, true, nameBuffer);

}