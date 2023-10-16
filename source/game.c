#include <SDL.h>
#include <SDL_ttf.h>

#include "defines.h"
#include "standard.h"
#include "math.h"
#include "highscore.h"
#include "text.h"
#include "tetromino.h"
#include "board.h"
#include "game.h"

bool pointRectCollision ( int x, int y, SDL_Rect rect )
{
    return (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h);
}
SDL_Rect center ( SDL_Rect rect )
{
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    return rect;
}
bool xor ( bool a, bool b )
{
    return (a || b) && !(a && b);
}
int max ( int a, int b )
{
    return a > b ? a : b;
}

int initGame ( Game* pGame )
{


    pGame->keysPressed = SDL_GetKeyboardState ( NULL );
    if (SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0)
    {
        printfd ( "Error: %s\n", SDL_GetError () );
        return 0;
    }
    pGame->pWindow = SDL_CreateWindow ( "Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0 );
    if (!pGame->pWindow)
    {
        printfd ( "Error: %s\n", SDL_GetError () );
        closeGame ( pGame );
        return 0;
    }
    pGame->pRenderer = SDL_CreateRenderer ( pGame->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if (!pGame->pRenderer)
    {
        printfd ( "Error: %s\n", SDL_GetError () );
        closeGame ( pGame );
        return 0;
    }
    SDL_Rect startButtonRect;
    startButtonRect.h = 75;
    startButtonRect.w = 200;
    startButtonRect.x = WINDOW_WIDTH / 2;
    startButtonRect.y = WINDOW_HEIGHT / 2;

    pGame->pScoreText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 100 }, White, true, "SCORE: %6d" );
    pGame->pGameOverText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 50 }, White, true, "GAME OVER" );
    pGame->pNameText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 250 }, White, true, "TYPE NAME:\n    %3s" );
    pGame->pTetrisText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 50 }, White, true, "TETRIS" );
    pGame->pStartingLevelText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", 10, pGame->pRenderer, (SDL_Rect) { WINDOW_WIDTH / 2, 100 }, White, true, "STARTING LEVEL: %2d" );
    pGame->pStartText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, pGame->pRenderer, startButtonRect, White, true, "START" );
    if (!pGame->pTetrisText || !pGame->pStartingLevelText)
    {
        printfd ( "Error: FONT\n" );
        return 0;
    }

    initBoard ( pGame );
    pGame->state = MENU;

    bool fileExists = loadHighscoresTxt ( &pGame->highscores );
    printfd ( "fileExists: %d\n", fileExists );
    if (fileExists == false) {
        printfd ( "STARTING GAME\n" );
        createHighscoresTxtFile ();
        if (!loadHighscoresTxt ( &pGame->highscores )) {
            return 0;
        }
    }

#if 0
    bool fileExists = loadHighscoresBin ( &pGame->highscores );
    printfd ( "fileExists: %d\n", fileExists );
    if (fileExists == false) {
        printfd ( "STARTING GAME\n" );
        createHighscoresBinFile ();
        if (!loadHighscoresBin ( &pGame->highscores )) {
            return 0;
        }
    }
#endif
    return 1;
}
void initBoard ( Game* pGame )
{
    if (pGame->pBoard)
    {
        free ( pGame->pBoard );
    }
    pGame->pBoard = CreateBoard ( pGame->pRenderer );
    pGame->lines = 0;
    pGame->score = 0;
    pGame->gravity = pGame->level;
}

void runGame ( Game* pGame )
{

    bool close_requested = false;
    // const uint8_t* keyPressed = SDL_GetKeyboardState ( NULL );
    int frameCounter = 0;
    pGame->pBoard->onGround = false;
    pGame->pBoard->onGroundTime = 0;
    int framesPerFall[29] = {48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3,2,2,2,2,2,2,2,2,2,1};
    while (pGame->state == PLAY)
    {
        SDL_SetRenderDrawColor ( pGame->pRenderer, 0, 0, 0, 255 );
        SDL_RenderClear ( pGame->pRenderer );
        SDL_SetRenderDrawColor ( pGame->pRenderer, 230, 230, 230, 255 );
        frameCounter++;
        if (GameOverCheck ( pGame->pBoard ))
        {
            close_requested = true;
            pGame->state = GAMEOVER;
            continue;
        }
        while (SDL_PollEvent ( &pGame->event ))
        {
            if (pGame->event.type == SDL_QUIT)
            {
                close_requested = 1;
                pGame->state = QUIT;
            }
            else if (pGame->event.key.type == SDL_KEYDOWN)
            {
                handleInput ( pGame);
            }
            else if (pGame->event.key.type == SDL_KEYUP && pGame->event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                pGame->gravity = pGame->level;
            }
        }
        if (GameOverCheck ( pGame->pBoard ))
        {
            pGame->state = GAMEOVER;
        }

        if (isTetrominoOnGround ( pGame->pBoard ))
        {
            NextRound ( pGame->pBoard, &pGame->score, &pGame->level, &pGame->lines );
            pGame->gravity = pGame->level;
        }

        else if (frameCounter % framesPerFall[max(pGame->level,29)] == 0 && frameCounter > 0)
        {
            
            Move ( pGame->pBoard, 0, 1 );
            frameCounter = 0;
        }
        DrawOccupied ( pGame->pBoard );
        DrawTetromino ( pGame->pBoard );
        drawGameUI ( pGame );

        SDL_RenderPresent ( pGame->pRenderer );
        SDL_Delay ( 1000 / FPS );
    }
}

void handleInput ( Game* pGame)
{
    bool left = xor(pGame->keysPressed[SDL_SCANCODE_A], pGame->keysPressed[SDL_SCANCODE_LEFT]);
    bool right = xor(pGame->keysPressed[SDL_SCANCODE_D], pGame->keysPressed[SDL_SCANCODE_RIGHT]);
    bool spinright = xor(pGame->keysPressed[SDL_SCANCODE_W], pGame->keysPressed[SDL_SCANCODE_UP]);
    bool spinleft = xor(pGame->keysPressed[SDL_SCANCODE_S], pGame->keysPressed[SDL_SCANCODE_DOWN]);
    
    if (left)
    {
        Move ( pGame->pBoard, -1, 0 );
    }
    if (right)
    {
        Move ( pGame->pBoard, +1, 0 );
    }
    if (spinright)
    {
        Rotation ( pGame->pBoard, 1 );
    }
    if (spinleft)
    {
        Rotation ( pGame->pBoard, -1 );
    }
    if (pGame->keysPressed[SDL_SCANCODE_K])
    {
        Move ( pGame->pBoard, 0, 1 );
    }
    if (pGame->keysPressed[SDL_SCANCODE_SPACE] && pGame->pBoard->onGround == false)
    {
        pGame->gravity = FPS;
        pGame->score += 1;
    }
}

void closeGame ( Game* pGame )
{
    if (pGame->pRenderer)
        SDL_DestroyRenderer ( pGame->pRenderer );
    if (pGame->pWindow)
        SDL_DestroyWindow ( pGame->pWindow );
    if (pGame->pBoard->pScoreText)
        destroyText ( pGame->pBoard->pScoreText );
    if (pGame->pBoard->pLevelText)
        destroyText ( pGame->pBoard->pLevelText );
    if (pGame->pBoard->pLinesText)
        destroyText ( pGame->pBoard->pLinesText );
    if (pGame->pBoard->pNextText)
        destroyText ( pGame->pBoard->pNextText );
    if (!saveHighscoresTxt ( &pGame->highscores )) {
        printfd ( "ERROR SAVING FILE\n" );
    }
#if 0
    if (!saveHighscoresBin ( &pGame->highscores )) {
        printfd ( "ERROR SAVING FILE\n" );
    }
#endif
    SDL_Quit ();
}

void drawGameUI ( Game* pGame )
{

    // Lines
    showText ( pGame->pBoard->pLinesText, pGame->lines );
    // Score
    showText ( pGame->pBoard->pScoreText, pGame->score );
    // Next piece
    SDL_Rect rightTextRect = showText ( pGame->pBoard->pNextText );
    ShowNextPiece ( pGame->pBoard, rightTextRect.x, rightTextRect.y + rightTextRect.h );
    // Level
    showText ( pGame->pBoard->pLevelText, pGame->level );
    SDL_Rect scoreboardRect = { 10,100 };
    displayScoreboard ( pGame->highscores, pGame->pRenderer, scoreboardRect );
}

void gameOverView ( Game* pGame )
{
    
    char nameBuffer[10] = "";
    while (pGame->state == GAMEOVER) {

        while (SDL_PollEvent ( &pGame->event ))
        {
            SDL_SetRenderDrawColor ( pGame->pRenderer, 0, 0, 0, 255 );
            SDL_RenderClear ( pGame->pRenderer );
            if (pGame->event.type == SDL_QUIT)
            {
                pGame->state = QUIT;
            }
            if (pGame->event.type == SDL_KEYDOWN)
            {
                textInput ( pGame->event, nameBuffer );
                if (pGame->event.key.keysym.sym == SDLK_RETURN && strlen ( nameBuffer ) >= 1)
                {
                    pGame->state = MENU;
                    insertScore ( &pGame->highscores, nameBuffer, pGame->score );
                    if (!saveHighscoresTxt ( &pGame->highscores ))
                        printfd ( "ERROR SAVING FILE\n" );
                    pGame->highscores.size = 0;
                    nameBuffer[0] = '\0';

                    if (!loadHighscoresTxt ( &pGame->highscores ))
                        printfd ( "ERROR LOADING FILE\n" );
#if 0
                    if (!saveHighscoresBin ( &pGame->highscores ))
                        printfd ( "ERROR SAVING FILE\n" );
                    pGame->highscores.size = 0;

                    if (!loadHighscoresBin ( &pGame->highscores ))
                        printfd ( "ERROR LOADING FILE\n" );
#endif
                }
            }
        }


        // showText(pGame->pRenderer, White, FONT_SIZE, nameRect, true, "Type Name:");

        showText ( pGame->pGameOverText );
        // showText(pGame->pRenderer, White, FONT_SIZE, gameOverRect, true, "GAME OVER");

        // showText(pGame->pRenderer, White, FONT_SIZE, scoreRect, true, "Score: %d", pGame->score);
        showText ( pGame->pScoreText, pGame->score );

        // showText(pGame->pRenderer, White, FONT_SIZE, nameinputRect, true, nameBuffer);
        showText ( pGame->pNameText, nameBuffer );

        SDL_RenderPresent ( pGame->pRenderer );
        SDL_Delay ( 1000 / FPS );
    }
}
void mainMenuView ( Game* pGame)
{
    SDL_Rect startButtonRect = center ( pGame->pStartText->rect );
    SDL_Rect scoreboardRect = { 10,100 };
    int level = 0;
    int mouseX, mouseY;

    while (pGame->state == MENU) {


        SDL_GetMouseState ( &mouseX, &mouseY );

        while (SDL_PollEvent ( &pGame->event ))
        {
            SDL_SetRenderDrawColor ( pGame->pRenderer, 0, 0, 0, 255 );
            SDL_RenderClear ( pGame->pRenderer );


            if (pGame->event.button.state == SDL_PRESSED && pointRectCollision ( mouseX, mouseY, startButtonRect ))
            {
                printfd ( "PRESSED\n" );
                pGame->level = level;
                pGame->state = PLAY;
            }
            if (pGame->event.type == SDL_QUIT)
            {
                pGame->state = QUIT;
            }
            if (pGame->event.type == SDL_KEYDOWN)
            {
                if (pGame->event.key.keysym.sym == SDLK_UP)
                {
                    level += 1;
                }
                if (pGame->event.key.keysym.sym == SDLK_DOWN && level > 0)
                {
                    level -= 1;
                }
            }
        }
        displayScoreboard ( pGame->highscores, pGame->pRenderer, scoreboardRect );
        showText ( pGame->pTetrisText );
        showText ( pGame->pStartingLevelText, level );
        SDL_SetRenderDrawColor ( pGame->pRenderer, 0, 200, 0, 255 );
        SDL_RenderFillRect ( pGame->pRenderer, &startButtonRect );
        showText ( pGame->pStartText );

        SDL_RenderPresent ( pGame->pRenderer );
        SDL_Delay ( 1000 / FPS );
    }
}
