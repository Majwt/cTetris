#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "defines.h"
#include "board.h"
#include "highscore.h"

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

const SDL_Color White = {255, 255, 255, 255};
TTF_Font *arial;

int initiate(Game *pGame);
void run(Game *pGame);
void close(Game *pGame);
void handleInput(Game *pGame, const uint8_t *keysPressed);
void initBoard(Game *pGame);
void mainMenu(Game *pGame);
SDL_Rect ShowText(SDL_Renderer *pRenderer, char text[], SDL_Rect rect, bool centered);
void DrawGameUI(Game *pGame);
void TextInput(SDL_Event event, char buffer[]);
bool PointRectCollision(int x, int y, SDL_Rect rect);
void SetNameView(Game *pGame);



void mainMenu(Game *pGame)
{
    static SDL_Event event;
    SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pGame->pRenderer);
    static char nameBuffer[10] = "";
    static SDL_Rect startButtonRect;
    static int level = 0;
    startButtonRect.h = 75;
    startButtonRect.w = 200;
    startButtonRect.x = WINDOW_WIDTH/2 - startButtonRect.w/2;
    startButtonRect.y = WINDOW_HEIGHT/2 - startButtonRect.h/2;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX,&mouseY);
    while (SDL_PollEvent(&event))
    {
        if (event.button.state == SDL_PRESSED && PointRectCollision(mouseX,mouseY,startButtonRect)) {
            printf("PRESSED\n");
            pGame->level = level;
            pGame->state = PLAY;
        }
        if (event.type == SDL_QUIT)
        {
            pGame->state = QUIT;
        }
        if (event.type == SDL_KEYDOWN)
        {
            TextInput(event, nameBuffer);
            if (event.key.keysym.sym == SDLK_UP)
            {
                level += 1;
            }
            if (event.key.keysym.sym == SDLK_DOWN && pGame->level > 0)
            {
                level -= 1;
            }
        }
    }
    static SDL_Rect nameRect;
    nameRect.x = WINDOW_WIDTH / 2;
    nameRect.y = 100;
    ShowText(pGame->pRenderer, nameBuffer, nameRect, true);

    static SDL_Rect startLevelRect;
    startLevelRect.x = WINDOW_WIDTH / 2;
    startLevelRect.y = 200;
    static char levelBuffer[25];
    sprintf(levelBuffer, "STARTING LEVEL: %d", level);
    ShowText(pGame->pRenderer, levelBuffer, startLevelRect, true);

    SDL_SetRenderDrawColor(pGame->pRenderer,0,200,0,255);
    SDL_RenderFillRect(pGame->pRenderer,&startButtonRect);
    startButtonRect.x = WINDOW_WIDTH/2;
    startButtonRect.y = WINDOW_HEIGHT/2-10;
    ShowText(pGame->pRenderer, "START", startButtonRect, true);

    SDL_RenderPresent(pGame->pRenderer);
    SDL_Delay(1000 / FPS);
}

int main(int argv, char **args)
{

    Game g = {0};
    if (!initiate(&g))
        return 1;
    while (g.state != QUIT)
    {
        if (g.state == PLAY)
        {
            initBoard(&g);
            run(&g);
        }
        else if (g.state == GAMEOVER)
        {
            SetNameView(&g);
        }
        else if (g.state == MENU)
        {
            mainMenu(&g);
        }
    }

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
    initBoard(pGame);
    pGame->state = MENU;
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

void run(Game *pGame)
{

    LoadHighscore(pGame->HighScores);

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

void close(Game *pGame)
{
    if (pGame->pRenderer)
        SDL_DestroyRenderer(pGame->pRenderer);
    if (pGame->pWindow)
        SDL_DestroyWindow(pGame->pWindow);
    if (arial)
        TTF_CloseFont(arial);
    SaveHighscore(pGame->HighScores);
    SDL_Quit();
}
SDL_Rect ShowText(SDL_Renderer *pRenderer, char text[], SDL_Rect rect, bool centered)
{
    if (strlen(text) > 0)
    {

        SDL_Surface *surface = TTF_RenderText_Solid(arial, text, White);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(pRenderer, surface);
        rect.w = surface->w;
        rect.h = surface->h;
        if (centered)
        {
            rect.x -= rect.w / 2;
        }
        SDL_FreeSurface(surface);
        SDL_RenderCopy(pRenderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    }
    return rect;
}

void DrawGameUI(Game *pGame)
{
    char textBuffer[100];

    SDL_Rect linesRect;
    linesRect.x = BOARD_X + BOARD_WIDTH / 2;
    linesRect.y = BOARD_Y - 50;

    // Lines
    sprintf(textBuffer, "Lines-%03d", pGame->lines);
    linesRect = ShowText(pGame->pRenderer, textBuffer, linesRect, true);

    SDL_Rect rightTextRect;
    // Score
    rightTextRect.x = BOARD_X + BOARD_WIDTH + 20;
    rightTextRect.y = BOARD_Y;
    sprintf(textBuffer, "Score");
    rightTextRect = ShowText(pGame->pRenderer, textBuffer, rightTextRect, false);
    rightTextRect.y += rightTextRect.h;
    sprintf(textBuffer, "%06d", pGame->score);
    rightTextRect = ShowText(pGame->pRenderer, textBuffer, rightTextRect, false);

    // Next piece
    rightTextRect.y += rightTextRect.h + 20;
    rightTextRect = ShowText(pGame->pRenderer, "Next:", rightTextRect, false);
    ShowNextPiece(pGame->pBoard, rightTextRect.x, rightTextRect.y + rightTextRect.h);

    // Level
    rightTextRect.y += 5 * TETRINOSIZE;
    sprintf(textBuffer, "Level");
    rightTextRect = ShowText(pGame->pRenderer, textBuffer, rightTextRect, false);
    rightTextRect.y += rightTextRect.h;
    sprintf(textBuffer, "%4d", pGame->level);
    ShowText(pGame->pRenderer, textBuffer, rightTextRect, false);
}

void TextInput(SDL_Event event, char buffer[])
{
    const char *keyName = SDL_GetKeyName(event.key.keysym.sym);
    int bufferlen = strlen(buffer);
    if (bufferlen < 8 && strlen(keyName) == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z')
    {
        strcat(buffer, keyName);
    }
    else if (strcmp(keyName, "Backspace") == 0 && bufferlen > 0)
    {
        int len = strlen(buffer);
        buffer[len - 1] = '\0';
    }
}
bool PointRectCollision(int x, int y, SDL_Rect rect)
{
    return (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h);
}

void SetNameView(Game *pGame) {
    static SDL_Event event;
    SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pGame->pRenderer);
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
            if (event.key.keysym.sym == SDLK_RETURN)
            {
                pGame->state = MENU;
                InsertScore(pGame->HighScores,nameBuffer,pGame->score);
                SaveHighscore(pGame->HighScores);
                LoadHighscore(pGame->HighScores);
            }
            
        }
    }
    SDL_Rect nameRect;
    nameRect.x = WINDOW_WIDTH / 2;
    nameRect.y = 100;
    ShowText(pGame->pRenderer, "Type Name:", nameRect, true);
    nameRect.y += 50;

    ShowText(pGame->pRenderer, nameBuffer, nameRect, true);

    SDL_RenderPresent(pGame->pRenderer);
    SDL_Delay(1000 / FPS);
}