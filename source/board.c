#include <SDL.h>
#include "defines.h"
#include "standard.h"
#include "highscore.h"
#include "text.h"
#include "tetromino.h"
#include "board.h"



Board_t* CreateBoard(SDL_Renderer* pRenderer)
{
    Board_t* board = malloc(sizeof(struct board));
    board->pRenderer = pRenderer;
    for(int i = 0; i < BOARD_ROWS; i++)
    {
        for(int j = 0; j < BOARD_COLUMNS; j++)
        {
            board->occupied[i][j] = 0;
        }
    }

#if DEBUG_BOARD == 1
    printf("Custom starting board\n");
    const int test[BOARD_ROWS][BOARD_COLUMNS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    for(int i = 0; i < BOARD_ROWS; i++)
    {
        for(int j = 0; j < BOARD_COLUMNS; j++)
        {
            board->occupied[i][j] = test[i][j] * 2;
        }
    }

#endif

    // board->occupied[]
    board->now_t = 0;
    board->prev_t = 0;
    SDL_Color Cyan = { 0, 255, 255, 255 };
    SDL_Color Yellow = { 255, 255, 0, 255 };
    SDL_Color Purple = { 200, 0, 200, 255 };
    SDL_Color Green = { 0, 255, 0, 255 };
    SDL_Color Blue = { 0, 0, 255, 255 };
    SDL_Color Red = { 255, 0, 0, 255 };
    SDL_Color Orange = { 255, 127, 0, 255 };

    board->colors[I - 1] = Cyan;
    board->colors[O - 1] = Yellow;
    board->colors[T - 1] = Purple;
    board->colors[J - 1] = Blue;
    board->colors[L - 1] = Orange;
    board->colors[Z - 1] = Red;
    board->colors[S - 1] = Green;

    board->x = BOARD_X;
    board->y = BOARD_Y;
    board->width = BOARD_WIDTH;
    board->height = BOARD_HEIGHT;

    Tetromino_t tmp, nextPiece;
    randomPiece(&tmp);
    board->activeTetromino = tmp;
    randomPiece(&nextPiece);
    board->nextTetromino = nextPiece;

    SDL_Rect rightside = { board->x + board->width + 20,board->y,0,0 };

    board->pLinesText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, board->pRenderer, (SDL_Rect) { board->x + board->width / 2, board->y - 50 }, White, true, "LINES - %03d");
    board->pScoreText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, board->pRenderer, rightside, White, false, "SCORE:\n%7d");
    rightside.y += 100;
    board->pNextText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, board->pRenderer, rightside, White, false, "Next:");
    rightside.y += 100;
    board->pLevelText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", FONT_SIZE, board->pRenderer, rightside, White, false, "Level:\n%4d");

#if DEBUG_BOARD
    printf("setting non random piece %d\n", DEBUG_PIECEID);
    board->activeTetromino = createPiece(DEBUG_PIECEID);
    board->nextTetromino = createPiece(DEBUG_PIECEID);
#endif

    return board;
}

void DrawOccupied(Board_t* pBoard)
{
    SDL_Surface* surface;
    SDL_Rect rect;
    rect.h = TETROMINOSIZE;
    rect.w = TETROMINOSIZE;
    rect.x = pBoard->x;
    rect.y = pBoard->y;

    for(int y = 0; y < BOARD_ROWS; y++)
    {
        for(int x = 0; x < BOARD_COLUMNS; x++)
        {
            int8_t current = pBoard->occupied[y][x];

            rect.x = pBoard->x + TETROMINOSIZE * x;
            rect.y = pBoard->y + TETROMINOSIZE * y;

            if(current > 0)
            {
                SDL_Color color = pBoard->colors[current - 1];
                SDL_SetRenderDrawColor(pBoard->pRenderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(pBoard->pRenderer, &rect);
            }
            else if(DRAW_GRID)
            {
                SDL_SetRenderDrawColor(pBoard->pRenderer, 100, 100, 100, 255);
                SDL_RenderDrawRect(pBoard->pRenderer, &rect);
            }
        }
    }

    //--- BORDER ---

    SDL_SetRenderDrawColor(pBoard->pRenderer, 255, 255, 255, 255);
    // Left Line
    SDL_RenderDrawLine(pBoard->pRenderer, pBoard->x - 1, pBoard->y, pBoard->x - 1, pBoard->y + pBoard->height);
    // Bottom Line
    SDL_RenderDrawLine(pBoard->pRenderer, pBoard->x - 1, pBoard->y + pBoard->height + 1, pBoard->x - 1 + pBoard->width + 1, pBoard->y + pBoard->height + 1);
    // Right Line
    SDL_RenderDrawLine(pBoard->pRenderer, pBoard->x + pBoard->width + 1, pBoard->y, pBoard->x + pBoard->width + 1, pBoard->y + pBoard->height);
}
/// @brief
/// @param pTetromino
/// @param pBoard
/// @return 1 if collision is detected, else 0
int TetrominoCollisionCheck(Board_t* pBoard, int offsetX, int offsetY, int orientation)
{

    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            int activeTetrominoBlockValue = pBoard->activeTetromino.orientations[orientation][y][x];
            int boardBlockX = pBoard->activeTetromino.x + x + offsetX;
            int boardBlockY = pBoard->activeTetromino.y + y + offsetY;
            if(boardBlockY < 0 || boardBlockX < 0 || boardBlockX >= BOARD_COLUMNS || boardBlockY >= BOARD_ROWS)
            {
                if(activeTetrominoBlockValue > 0)
                {
                    return 1;
                }
                continue;
            }
            int boardBlockValue = pBoard->occupied[boardBlockY][boardBlockX];
            if(activeTetrominoBlockValue > 0 && boardBlockValue != 0 && boardBlockValue < 10)
            {
                return 1;
            }
        }
    }
    return 0;
}
void drawTetromino(Board_t* pBoard)
{
    // bool hit = TetrominoCollisionCheck(pBoard, 0, 0, pBoard->activeTetromino.orientationIndex);
    SDL_Rect rect;
    rect.h = rect.w = TETROMINOSIZE;
    Tetromino_t* piece = &pBoard->activeTetromino;
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            rect.x = pBoard->x + (piece->x + x) * TETROMINOSIZE;
            rect.y = pBoard->y + (piece->y + y) * TETROMINOSIZE;
            bool bounds = rect.y < 0 || rect.x < 0 || rect.x >= BOARD_COLUMNS || rect.y >= BOARD_ROWS;
            int value = piece->orientations[piece->orientationIndex][y][x];
            if(bounds && value > 0)
            {
                SDL_Color color = pBoard->colors[value - 1];
                SDL_SetRenderDrawColor(pBoard->pRenderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(pBoard->pRenderer, &rect);
            }
        }
    }
#if DEBUG
    int pieceX = piece->x * TETROMINOSIZE + pBoard->x;
    int pieceY = piece->y * TETROMINOSIZE + pBoard->y;
    int pieceWidth = piece->width * TETROMINOSIZE;
    SDL_SetRenderDrawColor(pBoard->pRenderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(pBoard->pRenderer, pieceX, pieceY, pieceX, pieceY + pieceWidth);
    SDL_RenderDrawLine(pBoard->pRenderer, pieceX, pieceY, pieceX + pieceWidth, pieceY);
    SDL_RenderDrawLine(pBoard->pRenderer, pieceX + pieceWidth, pieceY, pieceX + pieceWidth, pieceY + pieceWidth);
    SDL_RenderDrawLine(pBoard->pRenderer, pieceX + pieceWidth, pieceY + pieceWidth, pieceX, pieceY + pieceWidth);
#endif
}
void movePiece(Board_t* pBoard, int dx, int dy)
{
    if(!TetrominoCollisionCheck(pBoard, dx, dy, pBoard->activeTetromino.orientationIndex))
    {
        if(dx)
        {
            printfd("MOVE %d: \n", dx);

            pBoard->onGroundTime = 0;
            pBoard->onGround = false;
        }
        pBoard->activeTetromino.x += dx;
        pBoard->activeTetromino.y += dy;
    }
    else if(dx)
    {
        printfd("MOVE %d: REVERT\n", dx);
    }
    else if(dy)
    {
        pBoard->onGround = true;
    }
}

/// @brief rotatePiece
/// @param pBoard
/// @param direction ±1
/// @return 0 if failed, 1 if success
bool rotatePiece(Board_t* pBoard, int direction)
{
    int oldOrientation = pBoard->activeTetromino.orientationIndex;
    int newOrientation = modneg(oldOrientation + direction, 4);
    const int srsIndex = getSRSindex(oldOrientation, newOrientation);
    if(srsIndex == -1)
    {
        printfd("ERROR SRS");
        return true;
    }
    printfd("srsIndex: %d\n", srsIndex);
#if USE_SRS
    int imax = 5;
#else
    int imax = 1;
#endif
    for(int i = 0; i < imax; i++)
    {
        int srsX = pBoard->activeTetromino.srsTests[srsIndex][i][0];
        int srsY = pBoard->activeTetromino.srsTests[srsIndex][i][1];
        bool success = TetrominoCollisionCheck(pBoard, srsX, -srsY, newOrientation) == 0;
        printfd("test %d: %-7s (%d,%d)\n", i + 1, (success) ? "success" : "fail", srsX, srsY);
        if(success)
        {
            pBoard->activeTetromino.x += srsX;
            pBoard->activeTetromino.y += -srsY;
            pBoard->activeTetromino.orientationIndex = newOrientation;
            pBoard->onGround = false;
            pBoard->onGroundTime = 0;
            printfd("Current rotation: I%d\n", newOrientation + 1);
            return true;
        }
    }

    printfd("No rotation\n");
    return false;
}

void convertToStatic(Board_t* pBoard)
{

    SDL_Rect rect;
    rect.h = rect.w = TETROMINOSIZE;
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            rect.x = pBoard->x + (pBoard->activeTetromino.x + x) * TETROMINOSIZE;
            rect.y = pBoard->y + (pBoard->activeTetromino.y + y) * TETROMINOSIZE;
            bool bounds = rect.y < 0 || rect.x < 0 || rect.x >= BOARD_COLUMNS || rect.y >= BOARD_ROWS;
            int value = pBoard->activeTetromino.orientations[pBoard->activeTetromino.orientationIndex][y][x];
            if(bounds && value > 0)
            {
                pBoard->occupied[pBoard->activeTetromino.y + y][pBoard->activeTetromino.x + x] = value;
            }
        }
    }
}

void shiftRowsDown(Board_t* pBoard, int y)
{

    for(int row = 0; row < BOARD_COLUMNS; row++)
    {
        for(int j = y; j > 1; j--)
        {
            pBoard->occupied[j][row] = pBoard->occupied[j - 1][row];
        }
        pBoard->occupied[2][row] = 0;
    }
}
int clearCompleteRows(Board_t* pBoard)
{
    int lines = 0;
    for(int y = 0; y < BOARD_ROWS; y++)
    {
        bool completeRow = true;
        for(int x = 0; x < BOARD_COLUMNS && completeRow == true; x++)
        {
            if(pBoard->occupied[y][x] == 0)
            {
                completeRow = false;
            }
        }
        if(completeRow == true)
        {
            lines++;
            shiftRowsDown(pBoard, y);
        }
    }
    return lines;
}

void showNextPiece(Board_t* pBoard, int x0, int y0)
{

    SDL_Rect rect;
    rect.h = rect.w = TETROMINOSIZE;
    for(int y = 0; y < 4; y++)
    {
        if(pBoard->nextTetromino.width == 3)
        {
            rect.y = y0 + TETROMINOSIZE + y * TETROMINOSIZE;
        }
        else
        {
            rect.y = y0 + y * TETROMINOSIZE;
        }
        for(int x = 0; x < 4; x++)
        {
            int currentPixel = pBoard->nextTetromino.orientations[0][y][x];

            rect.x = x0 + x * TETROMINOSIZE;
            if(currentPixel > 0)
            {
                if(currentPixel > 10)
                {
                    currentPixel -= 10;
                }
                SDL_Color color = pBoard->colors[currentPixel - 1];
                SDL_SetRenderDrawColor(pBoard->pRenderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(pBoard->pRenderer, &rect);
            }
        }
    }
}

bool isTetrominoOnGround(Board_t* pBoard)
{
    updateOnGroundTime(pBoard);
    return (pBoard->onGround && pBoard->onGroundTime > ONGROUND_TIME_MIN);
}


void beginNextRound(Board_t* pBoard, Score_t* player)
{
    convertToStatic(pBoard);
    pBoard->activeTetromino = pBoard->nextTetromino;
    randomPiece(&pBoard->nextTetromino);
#if DEBUG_BOARD
    pBoard->nextTetromino = createPiece(DEBUG_PIECEID);
#endif
    int linesCleared = clearCompleteRows(pBoard);

    player->score += addPoints(player->level, linesCleared);
    // player->lines += linesCleared;
    int threshold1 = player->startingLevel * 10 + 10;
    int threshold2 = max(100, player->startingLevel * 10 - 50);
    while(linesCleared > 0)
    {
        player->lines += 1;
        linesCleared--;
        bool aboveThreshold = player->lines >= threshold1 || player->lines >= threshold2;
        if(aboveThreshold)
        {
            bool multipleOfTen = (player->lines - threshold1) % 10 == 0 || (player->lines - threshold2) % 10 == 0;
            if(!player->passedThreshold)
            {
                player->level += 1;
                player->passedThreshold = true;
            }
            else if(multipleOfTen)
            {
                player->level += 1;
            }
        }
    }

    pBoard->onGround = false;
}
void updateOnGroundTime(Board_t* pBoard)
{
    pBoard->prev_t = pBoard->now_t;
    pBoard->now_t = clock();
    if(pBoard->onGround)
    {
        pBoard->onGroundTime += (pBoard->now_t - pBoard->prev_t) / CLOCKS_PER_SEC;
    }
    else
    {
        pBoard->onGroundTime = 0;
    }
}
bool gameOverCheck(Board_t* pBoard)
{
    bool test = pBoard->activeTetromino.y < 1 && TetrominoCollisionCheck(pBoard, 0, 0, pBoard->activeTetromino.orientationIndex);
    return test;
}
void drawBoard(Board_t* pBoard, Score_t* player)
{
    DrawOccupied(pBoard);
    drawTetromino(pBoard);
    updateText(pBoard->pLinesText, player->lines);
    SDL_Rect rect = updateText(pBoard->pScoreText, player->score);
    pBoard->pNextText->rect.y = rect.y + rect.h + 20;
    showNextPiece(pBoard, pBoard->x + pBoard->width + 20, pBoard->pNextText->rect.y + pBoard->pNextText->rect.h + 20);
    pBoard->pLevelText->rect.y = pBoard->pNextText->rect.y + pBoard->pNextText->rect.h + 20 + 4 * TETROMINOSIZE;
    updateText(pBoard->pLevelText, player->level);

    drawText(pBoard->pLevelText);
    drawText(pBoard->pLinesText);
    drawText(pBoard->pNextText);
    drawText(pBoard->pScoreText);
}
int addPoints(int level, int lines)
{
    int scoreToAdd;
    switch(lines)
    {
    case 0:
        scoreToAdd = 0;
        break;
    case 1:
        scoreToAdd = 40 * (level + 1);
        break;
    case 2:
        scoreToAdd = 100 * (level + 1);
        break;
    case 3:
        scoreToAdd = 300 * (level + 1);
        break;
    case 4:
        scoreToAdd = 1200 * (level + 1);
        break;
    default:
        scoreToAdd = -1;
        break;
    }
    return scoreToAdd;
}