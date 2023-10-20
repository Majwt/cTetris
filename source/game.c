#include <SDL.h>
#include <SDL_ttf.h>

#include "common.h"
#include "game.h"

#define FPS 60

bool pointRectCollision(int x, int y, SDL_Rect rect)
{
    return (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h);
}
SDL_Rect center(SDL_Rect rect)
{
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    return rect;
}

int initGame(Game_t* pGame)
{

    pGame->keysPressed = SDL_GetKeyboardState(NULL);
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
    SDL_Rect startButtonRect;
    startButtonRect.h = 75;
    startButtonRect.w = 200;
    startButtonRect.x = WINDOW_WIDTH / 2;
    startButtonRect.y = WINDOW_HEIGHT / 2;

    pGame->pScoreText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 100 }, White, true, "SCORE: %6d");
    pGame->pGameOverText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 50 }, White, true, "GAME OVER");
    pGame->pNameText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 250 }, White, true, "TYPE NAME:\n    %3s");
    pGame->pTetrisText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 50 }, White, true, "TETRIS");
    pGame->pStartingLevelText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", 12, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 100 }, White, true, "STARTING LEVEL: %2d");
    pGame->pStartText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, startButtonRect, White, true, "START");
    if (!pGame->pTetrisText || !pGame->pStartingLevelText)
    {
        printfd("Error: FONT\n");
        return 0;
    }

    initBoard(pGame);
    pGame->state = MENU;

    pGame->highscores = initHighscore(pGame->pRenderer, (SDL_Rect) { 30, 150 });
    bool fileExists = loadHighscoresTxt(&pGame->highscores);
    printfd("fileExists: %d\n", fileExists);
    if (fileExists == false)
    {
        printfd("STARTING GAME\n");
        createHighscoresTxtFile();
        if (!loadHighscoresTxt(&pGame->highscores))
        {
            return 0;
        }
    }


    return 1;
}
void initBoard(Game_t* pGame)
{
    if (pGame->pBoard)
    {
        free(pGame->pBoard);
    }
    pGame->pBoard = createBoard(pGame->pRenderer);
    pGame->pBoard->onGround = false;
    pGame->pBoard->onGroundTime = 0;
    pGame->player.score = 0;
    pGame->player.lines = 0;
    strcpy(pGame->player.name, "YOU");
    pGame->player.passedThreshold = false;

}
void closeGame(Game_t* pGame)
{
    if (pGame->pRenderer)
        SDL_DestroyRenderer(pGame->pRenderer);
    if (pGame->pWindow)
        SDL_DestroyWindow(pGame->pWindow);
    if (pGame->pBoard->pScoreText)
        destroyText(pGame->pBoard->pScoreText);
    if (pGame->pBoard->pLevelText)
        destroyText(pGame->pBoard->pLevelText);
    if (pGame->pBoard->pLinesText)
        destroyText(pGame->pBoard->pLinesText);
    if (pGame->pBoard->pNextText)
        destroyText(pGame->pBoard->pNextText);
    if (!saveHighscoresTxt(&pGame->highscores))
    {
        printfd("ERROR SAVING FILE\n");
    }
    if (pGame->pBoard)
        free(pGame->pBoard);

    SDL_Quit();
}


void runGame(Game_t* pGame)
{

    int frameCounter = 0;

    const int framesPerFall[29] = { 48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3,2,2,2,2,2,2,2,2,2,1 };
    pGame->gravity = pGame->player.startingLevel;
    while (pGame->state == PLAY)
    {
        frameCounter++;
        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pGame->pRenderer);
        SDL_SetRenderDrawColor(pGame->pRenderer, 230, 230, 230, 255);
        if (gameOverCheck(pGame->pBoard))
        {
            pGame->state = GAMEOVER;
            continue;
        }
        handleEvents(pGame);

        if (gameOverCheck(pGame->pBoard))
        {
            pGame->state = GAMEOVER;
            break;
        }
        if (isTetrominoOnGround(pGame->pBoard))
        {
            beginNextRound(pGame->pBoard, &pGame->player);
            pGame->gravity = pGame->player.level;
        }
        else if ((frameCounter % framesPerFall[min(pGame->gravity, 28)] == 0))
        {

            movePiece(pGame->pBoard, 0, 1);
            frameCounter = 0;
        }
        drawBoard(pGame->pBoard, &pGame->player);
        displayScoreboard(pGame->highscores, &pGame->player);

        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(1000 / FPS);
    }
}

void gameOverView(Game_t* pGame)
{

    pGame->player.name[0] = '\0';
    while (pGame->state == GAMEOVER)
    {

        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pGame->pRenderer);

        handleEvents(pGame);



        updateText(pGame->pScoreText, pGame->player.score);
        updateText(pGame->pNameText, pGame->player.name);

        drawText(pGame->pGameOverText);
        drawText(pGame->pScoreText);
        drawText(pGame->pNameText);


        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(1000 / FPS);
    }
}
void mainMenuView(Game_t* pGame)
{
    SDL_Rect startButtonRect = center(pGame->pStartText->rect);
    int level = 0;
    int mouseX, mouseY;

    while (pGame->state == MENU)
    {


        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pGame->pRenderer);

        handleEvents(pGame);

        updateText(pGame->pStartingLevelText, pGame->player.startingLevel);

        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 200, 0, 255);
        SDL_RenderFillRect(pGame->pRenderer, &startButtonRect);

        displayScoreboard(pGame->highscores, NULL);
        drawText(pGame->pTetrisText);
        drawText(pGame->pStartingLevelText);
        drawText(pGame->pStartText);

        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(1000 / FPS);
    }
}


void handleEvents(Game_t* pGame)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Rect startButtonRect = center(pGame->pStartText->rect);
    while (SDL_PollEvent(&pGame->event))
    {
        if (pGame->event.type == SDL_QUIT)
        {
            pGame->state = QUIT;
            break;
        }
        if (pGame->state == PLAY)
        {
            if (pGame->event.key.type == SDL_KEYDOWN)
            {
                handleInput(pGame);
            }
            else if (pGame->event.key.type == SDL_KEYUP && pGame->event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                pGame->gravity = pGame->player.level;
            }
        }
        else if (pGame->state == GAMEOVER)
        {

            if (pGame->event.type == SDL_KEYDOWN)
            {
                textInput(pGame->event, pGame->player.name);
                if (pGame->event.key.keysym.sym == SDLK_RETURN && strlen(pGame->player.name) >= 1)
                {
                    pGame->state = MENU;
                    insertScore(&pGame->highscores, pGame->player);
                    if (!saveHighscoresTxt(&pGame->highscores))
                        printfd("ERROR SAVING FILE\n");
                    pGame->highscores.size = 0;


                    if (!loadHighscoresTxt(&pGame->highscores))
                        printfd("ERROR LOADING FILE\n");

                }
            }
        }
        else if (pGame->state == MENU) {
            if (pGame->event.button.state == SDL_PRESSED && pointRectCollision(mouseX, mouseY, startButtonRect))
            {
                printfd("PRESSED\n");
                pGame->player.level = pGame->player.startingLevel;
                pGame->state = PLAY;
            }
            if (pGame->event.type == SDL_KEYDOWN)
            {
                if (pGame->event.key.keysym.sym == SDLK_UP && pGame->player.startingLevel < 29)
                {
                    pGame->player.startingLevel += 1;
                }
                if (pGame->event.key.keysym.sym == SDLK_DOWN && pGame->player.startingLevel > 0)
                {
                    pGame->player.startingLevel -= 1;
                }
            }
        }
    }
}
void handleInput(Game_t* pGame)
{
    bool left = xor (pGame->keysPressed[SDL_SCANCODE_A], pGame->keysPressed[SDL_SCANCODE_LEFT]);
    bool right = xor (pGame->keysPressed[SDL_SCANCODE_D], pGame->keysPressed[SDL_SCANCODE_RIGHT]);
    bool spinright = xor (pGame->keysPressed[SDL_SCANCODE_W], pGame->keysPressed[SDL_SCANCODE_UP]);
    bool spinleft = xor (pGame->keysPressed[SDL_SCANCODE_S], pGame->keysPressed[SDL_SCANCODE_DOWN]);

    if (left)
    {
        movePiece(pGame->pBoard, -1, 0);
    }
    if (right)
    {
        movePiece(pGame->pBoard, +1, 0);
    }
    if (spinright)
    {
        rotatePiece(pGame->pBoard, 1);
    }
    if (spinleft)
    {
        rotatePiece(pGame->pBoard, -1);
    }
    if (pGame->keysPressed[SDL_SCANCODE_K])
    {
        pGame->player.lines += 1;
    }
    if (pGame->keysPressed[SDL_SCANCODE_L])
    {
        pGame->player.level += 1;
    }
    if (pGame->keysPressed[SDL_SCANCODE_ESCAPE])
    {
        pGame->state = MENU;
    }
    if (pGame->keysPressed[SDL_SCANCODE_B]) {
        hardDrop(pGame->pBoard);
    }
    if (pGame->keysPressed[SDL_SCANCODE_SPACE] && pGame->pBoard->onGround == false && pGame->player.level < 29)
    {
        pGame->gravity = 18;
    }
}