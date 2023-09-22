#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include "defines.h"
#include "tetrino.h"
#include "board.h"

struct board
{
    int x, y;
    int width, height;
    SDL_Renderer *pRenderer;
    int occupied[BOARD_ROWS][BOARD_COLUMNS];
    SDL_Color colors[7];
    Tetrino activeTetrino;
    Tetrino nextTetrino;
    double onGroundTime;
    bool onGround;
    double now_t, prev_t;
};

Board *CreateBoard(SDL_Renderer *pRenderer)
{
    Board *board = malloc(sizeof(struct board));
    board->pRenderer = pRenderer;
    for (int i = 0; i < BOARD_ROWS; i++)
    {
        for (int j = 0; j < BOARD_COLUMNS; j++)
        {
            board->occupied[i][j] = 0;
        }
    }
    board->now_t = 0;
    board->prev_t = 0;
    SDL_Color Cyan = {0, 255, 255, 255};
    SDL_Color Yellow = {255, 255, 0, 255};
    SDL_Color Purple = {200, 0, 200, 255};
    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color Blue = {0, 0, 255, 255};
    SDL_Color Red = {255, 0, 0, 255};
    SDL_Color Orange = {255, 127, 0, 255};

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

    Tetrino tmp, nextPiece;
    RandomPiece(&tmp);
    board->activeTetrino = tmp;
    RandomPiece(&nextPiece);
    board->nextTetrino = nextPiece;

    return board;
}

void DrawOccupied(Board *pBoard)
{
    SDL_Surface *surface;
    SDL_Rect rect;
    rect.h = TETRINOSIZE;
    rect.w = TETRINOSIZE;
    rect.x = pBoard->x;
    rect.y = pBoard->y;

    for (int y = 0; y < BOARD_ROWS; y++)
    {
        for (int x = 0; x < BOARD_COLUMNS; x++)
        {
            int8_t current = pBoard->occupied[y][x];

            rect.x = pBoard->x + TETRINOSIZE * x;
            rect.y = pBoard->y + TETRINOSIZE * y;
            if (current > 0)
            {
                if (current > 10)
                {
                    current -= 10;
                }
                SDL_Color color = pBoard->colors[current - 1];
                SDL_SetRenderDrawColor(pBoard->pRenderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(pBoard->pRenderer, &rect);
            }
            else if (DRAW_GRID)
            {
                SDL_SetRenderDrawColor(pBoard->pRenderer, 100, 100, 100, 255);
                SDL_RenderDrawRect(pBoard->pRenderer, &rect);
            }
        }
    }
    //--- BORDER ---

    SDL_SetRenderDrawColor(pBoard->pRenderer, 255, 255, 255, 255);
    // Left Line
    SDL_RenderDrawLine(pBoard->pRenderer, pBoard->x, pBoard->y, pBoard->x, pBoard->y + pBoard->height);
    // Bottom Line
    SDL_RenderDrawLine(pBoard->pRenderer, pBoard->x, pBoard->y + pBoard->height, pBoard->x + pBoard->width, pBoard->y + pBoard->height);
    // Right Line
    SDL_RenderDrawLine(pBoard->pRenderer, pBoard->x + pBoard->width, pBoard->y, pBoard->x + pBoard->width, pBoard->y + pBoard->height);
}
/// @brief
/// @param pTetrino
/// @param pBoard
/// @return 1 if collision is detected, else 0
int TetrinoCollisionCheck(Board *pBoard)
{
    const int orientation = pBoard->activeTetrino.orientationIndex;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            int activeTetrinoBlockValue = pBoard->activeTetrino.orientations[orientation][y][x];
            int boardBlockX = x + pBoard->activeTetrino.x;
            int boardBlockY = y + pBoard->activeTetrino.y;
            if (boardBlockY < 0 || boardBlockX < 0 || boardBlockX >= BOARD_COLUMNS || boardBlockY >= BOARD_ROWS)
            {
                if (activeTetrinoBlockValue > 0)
                {
                    return 1;
                }
                continue;
            }

            int boardBlockValue = pBoard->occupied[boardBlockY][boardBlockX];

            if (activeTetrinoBlockValue > 0 && boardBlockValue != 0 && boardBlockValue < 10)
            {
                return 1;
            }
        }
    }
    return 0;
}
void DrawTetrino(Board *pBoard)
{

    int y0 = pBoard->activeTetrino.y;
    int x0 = pBoard->activeTetrino.x;
    for (int y = 0; y < BOARD_ROWS; y++)
    {
        for (int x = 0; x < BOARD_COLUMNS; x++)
        {
            if (pBoard->occupied[y][x] > 10 || pBoard->occupied[y][x] < 0)
            {
                pBoard->occupied[y][x] = 0;
            }
        }
    }
    bool hit = TetrinoCollisionCheck(pBoard);
    for (int y = 0; y < 4 && !hit; y++)
    {
        for (int x = 0; x < 4 && !hit; x++)
        {
            int activeTetrinoBlockValue = pBoard->activeTetrino.orientations[pBoard->activeTetrino.orientationIndex][y][x];

            if (activeTetrinoBlockValue > 0)
            {
                pBoard->occupied[y + y0][x + x0] = activeTetrinoBlockValue + 10;
            }
        }
    }
}
void MoveSideways(Board *pBoard, int dx)
{
    pBoard->activeTetrino.x += dx;
#if DEBUG
    printf("MOVE %d: ", dx);
#endif
    if (TetrinoCollisionCheck(pBoard))
    {
#if DEBUG
        printf("REVERT");
#endif
        pBoard->activeTetrino.x += -dx;
    }
    else
    {
        pBoard->onGroundTime = 0;
        pBoard->onGround = false;
    }
#if DEBUG
    printf("\n");
#endif
}
void MoveDown(Board *pBoard)
{
    pBoard->activeTetrino.y += 1;
    bool onGround = TetrinoCollisionCheck(pBoard);
    if (onGround)
    {
        pBoard->activeTetrino.y += -1;
        pBoard->onGround = true;
    }
}

void RotateClockwise(Board *pBoard)
{
    int before = pBoard->activeTetrino.orientationIndex;
    pBoard->activeTetrino.orientationIndex = (pBoard->activeTetrino.orientationIndex + 1) % 4;
#if DEBUG
    printf("ROTATE CLOCKWISE: %d->%d ", before, pBoard->activeTetrino.orientationIndex);
#endif
    if (TetrinoCollisionCheck(pBoard))
    {
        pBoard->activeTetrino.orientationIndex = (pBoard->activeTetrino.orientationIndex - 1 > -1) ? pBoard->activeTetrino.orientationIndex - 1 : 3;
#if DEBUG
        printf("REVERT %d", pBoard->activeTetrino.orientationIndex);
#endif
    }
#if DEBUG
    printf("\n");
#endif
}
void RotateAntiClockwise(Board *pBoard)
{
#if DEBUG
    printf("ROTATE ANTI CLOCKWISE: ");
#endif
    pBoard->activeTetrino.orientationIndex = (pBoard->activeTetrino.orientationIndex - 1 > -1) ? pBoard->activeTetrino.orientationIndex - 1 : 3;
    if (TetrinoCollisionCheck(pBoard))
    {
#if DEBUG
        printf("REVERT");
#endif
        pBoard->activeTetrino.orientationIndex = (pBoard->activeTetrino.orientationIndex + 1) % 4;
    }
#if DEBUG
    printf("\n");
#endif
}
void ConvertToStatic(Board *pBoard)
{
    pBoard->activeTetrino = pBoard->nextTetrino;
    RandomPiece(&pBoard->nextTetrino);
    for (int y = 0; y < BOARD_ROWS; y++)
    {
        for (int x = 0; x < BOARD_COLUMNS; x++)
        {
            if (pBoard->occupied[y][x] > 10)
            {
                pBoard->occupied[y][x] -= 10;
            }
        }
    }
}

void ShiftRowsDown(Board *pBoard, int y)
{

    for (int row = 0; row < BOARD_COLUMNS; row++)
    {
        for (int j = y; j > 1; j--)
        {
            pBoard->occupied[j][row] = pBoard->occupied[j - 1][row];
        }
        pBoard->occupied[2][row] = 0;
    }
}
int ClearCompleteRows(Board *pBoard)
{
    int lines = 0;
    for (int y = 0; y < BOARD_ROWS; y++)
    {
        bool completeRow = true;
        for (int x = 0; x < BOARD_COLUMNS && completeRow == true; x++)
        {
            if (pBoard->occupied[y][x] == 0)
            {
                completeRow = false;
            }
        }
        if (completeRow == true)
        {
            lines++;
            ShiftRowsDown(pBoard, y);
        }
    }
    return lines;
}

void ShowNextPiece(Board *pBoard, int x0, int y0)
{

    SDL_Rect rect;
    rect.h = rect.w = TETRINOSIZE;
    for (int y = 0; y < 4; y++)
    {
        if (pBoard->nextTetrino.width == 3)
        {
            rect.y = y0 + TETRINOSIZE + y * TETRINOSIZE;
        }
        else
        {
            rect.y = y0 + y * TETRINOSIZE;
        }
        for (int x = 0; x < 4; x++)
        {
            int currentPixel = pBoard->nextTetrino.orientations[0][y][x];

            rect.x = x0 + x * TETRINOSIZE;
            if (currentPixel > 0)
            {
                if (currentPixel > 10)
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

bool isTetrinoOnGround(Board *pBoard)
{
    UpdateOnGroundTime(pBoard);
    return (pBoard->onGround && pBoard->onGroundTime > ONGROUND_TIME_MIN);
}

void NextRound(Board *pBoard, int *pScore, int *pLevel, int *pLines)
{
    ConvertToStatic(pBoard);
    int linesCleared = ClearCompleteRows(pBoard);
    *pScore += AddPoints(*pLevel, linesCleared);
    *pLines += linesCleared;
    if (*pLines > 1 && *pLines % 5 == 0)
    {
        *pLevel += 1;
#if DEBUG
        printf("Level UP: ");
        printf("%d\n", *pLevel);
#endif
    }
    pBoard->onGround = false;
}
void UpdateOnGroundTime(Board *pBoard)
{
    pBoard->prev_t = pBoard->now_t;
    pBoard->now_t = clock();
    if (pBoard->onGround)
    {
        pBoard->onGroundTime += (pBoard->now_t - pBoard->prev_t) / CLOCKS_PER_SEC;
    }
    else
    {
        pBoard->onGroundTime = 0;
    }
}
bool GameOverCheck(Board *pBoard)
{
    return pBoard->activeTetrino.y < 1 && TetrinoCollisionCheck(pBoard);
}
int AddPoints(int level, int lines)
{
    int scoreToAdd;
    switch (lines)
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