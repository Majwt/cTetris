#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "tetromino.h"

void fillTetrominoOrientation(Tetromino *pTetromino, int orientation[4][4][4])
{
    pTetromino->orientationIndex = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                pTetromino->orientations[i][j][k] = orientation[i][j][k];
            }
        }
    }
}

// TODO: Läs in från fil istället för hårdkodat
Tetromino CreatePiece(int n)
{
    // Ser dåligt ut
    int pieceOrientations[7][4][4][4] = {{{{0, 0, 0, 0},
                                           {I, I, I, I},
                                           {0, 0, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, I, 0},
                                           {0, 0, I, 0},
                                           {0, 0, I, 0},
                                           {0, 0, I, 0}},
                                          {{0, 0, 0, 0},
                                           {0, 0, 0, 0},
                                           {I, I, I, I},
                                           {0, 0, 0, 0}},
                                          {{0, I, 0, 0},
                                           {0, I, 0, 0},
                                           {0, I, 0, 0},
                                           {0, I, 0, 0}}},
                                         {{{0, 0, 0, 0},
                                           {0, O, O, 0},
                                           {0, O, O, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, 0, 0},
                                           {0, O, O, 0},
                                           {0, O, O, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, 0, 0},
                                           {0, O, O, 0},
                                           {0, O, O, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, 0, 0},
                                           {0, O, O, 0},
                                           {0, O, O, 0},
                                           {0, 0, 0, 0}}},
                                         {{{0, T, 0, 0},
                                           {T, T, T, 0},
                                           {0, 0, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, T, 0, 0},
                                           {0, T, T, 0},
                                           {0, T, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, 0, 0},
                                           {T, T, T, 0},
                                           {0, T, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, T, 0, 0},
                                           {T, T, 0, 0},
                                           {0, T, 0, 0},
                                           {0, 0, 0, 0}}},
                                         {{{J, 0, 0, 0},
                                           {J, J, J, 0},
                                           {0, 0, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, J, J, 0},
                                           {0, J, 0, 0},
                                           {0, J, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, 0, 0},
                                           {J, J, J, 0},
                                           {0, 0, J, 0},
                                           {0, 0, 0, 0}},
                                          {{0, J, 0, 0},
                                           {0, J, 0, 0},
                                           {J, J, 0, 0},
                                           {0, 0, 0, 0}}},
                                         {{{0, 0, L, 0},
                                           {L, L, L, 0},
                                           {0, 0, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, L, 0, 0},
                                           {0, L, 0, 0},
                                           {0, L, L, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, 0, 0},
                                           {L, L, L, 0},
                                           {L, 0, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{L, L, 0, 0},
                                           {0, L, 0, 0},
                                           {0, L, 0, 0},
                                           {0, 0, 0, 0}}},
                                         {{{0, S, S, 0},
                                           {S, S, 0, 0},
                                           {0, 0, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, S, 0, 0},
                                           {0, S, S, 0},
                                           {0, 0, S, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, 0, 0},
                                           {0, S, S, 0},
                                           {S, S, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{S, 0, 0, 0},
                                           {S, S, 0, 0},
                                           {0, S, 0, 0},
                                           {0, 0, 0, 0}}},
                                         {{{Z, Z, 0, 0},
                                           {0, Z, Z, 0},
                                           {0, 0, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, Z, 0},
                                           {0, Z, Z, 0},
                                           {0, Z, 0, 0},
                                           {0, 0, 0, 0}},
                                          {{0, 0, 0, 0},
                                           {Z, Z, 0, 0},
                                           {0, Z, Z, 0},
                                           {0, 0, 0, 0}},
                                          {{0, Z, 0, 0},
                                           {Z, Z, 0, 0},
                                           {Z, 0, 0, 0},
                                           {0, 0, 0, 0}}}};

    Tetromino piece;
    fillTetrominoOrientation(&piece, pieceOrientations[n]);
    piece.x = 3;
    piece.y = 0;
    if (n+1 == I || n+1 == O)
    {
        piece.width = 4;
        piece.y--;
    }
    else
    {
        piece.width = 3;
    }
    return piece;
}
void RandomPiece(Tetromino *piece)
{
    *piece = CreatePiece(rand() % 7);
}
