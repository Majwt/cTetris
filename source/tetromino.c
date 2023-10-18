#include "defines.h"
#include "standard.h"
#include "tetromino.h"

void fillTetrominoOrientation(Tetromino_t* pTetromino, int orientation[4][4][4])
{
    pTetromino->orientationIndex = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                pTetromino->orientations[i][j][k] = orientation[i][j][k];
            }
        }
    }
}

// TODO: Läs in från fil istället för hårdkodat
Tetromino_t createPiece(int n)
{
    // Ser dåligt ut
    static int pieceOrientations[7][4][4][4] = {
        // I
        {{{0, 0, 0, 0},
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
        // O
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
        // T
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
        // J
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
        // L
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
        // S
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
        // Z
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
            {0, 0, 0, 0}}} };


    // FILE* pieces = fopen("pieces","wb");

    // pieces = fo
    // Super Rotation System
    // https://tetris.wiki/Super_Rotation_System
    // https://web.archive.org/web/20071027030342/http://www.the-shell.net/img/j_piece.html
    static int srs[2][8][5][2] = {
        // I
        {{{0, 0}, {-2, 0}, {+1, 0}, {+1, +2}, {-2, -1}},  // 1 from 0
         {{0, 0}, {+2, 0}, {-1, 0}, {+2, +1}, {-1, -2}},  // 0 from 1 
         {{0, 0}, {-1, 0}, {+2, 0}, {-1, +2}, {+2, -1}},  // 2 from 1
         {{0, 0}, {-2, 0}, {+1, 0}, {-2, +1}, {+1, -1}},  // 1 from 2 
         {{0, 0}, {+2, 0}, {-1, 0}, {+2, +1}, {-1, -2}},  // 3 from 2 
         {{0, 0}, {+1, 0}, {-2, 0}, {+1, -2}, {-2, +1}},  // 2 from 3 
         {{0, 0}, {-2, 0}, {+1, 0}, {-2, +1}, {+1, -2}},  // 0 from 3 
         {{0, 0}, {+2, 0}, {-1, 0}, {-1, +2}, {+2, -1}}}, // 3 from 0 
        // J, L, S, T, Z
        {{{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},   // 1 from 0
         {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},   // 0 from 1
         {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},   // 2 from 1
         {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},   // 1 from 2
         {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}},   // 3 from 2
         {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},   // 2 from 3
         {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},   // 0 from 3
         {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}}} }; // 3 from 0

    Tetromino_t piece;
    piece.type = n;
    copySRS(srs[(n > 1) ? 1 : 0], piece.srsTests);
    fillTetrominoOrientation(&piece, pieceOrientations[n]);
    piece.y = 0;
    piece.x = 4;
    piece.width = 3;
    if(n + 1 == I || n + 1 == O)
    {
        piece.width = 4;
        piece.y--;
        piece.x--;
    }
    return piece;
}
void randomPiece(Tetromino_t* piece)
{
    *piece = createPiece(rand() % 7);
}
int getSRSindex(int from, int to)
{
    printfd("I%d from I%d\n", to + 1, from + 1);
    switch(from)
    {
    case 0:
        if(to == 1)
        {
            return 0;
        }
        else if(to == 3)
        {
            return 7;
        }
        return -1;
        break;
    case 1:
        if(to == 0)
        {
            return 1;
        }
        else if(to == 2)
        {
            return 2;
        }
        return -1;

        break;
    case 2:
        if(to == 1)
        {
            return 3;
        }
        else if(to == 3)
        {
            return 4;
        }
        return -1;

        break;
    case 3:
        if(to == 0)
        {

            return 6;
        }
        else if(to == 2)
        {

            return 5;
        }
        return -1;

        break;

    default:
        return -1;
        break;
    }
}
void copySRS(int in[8][5][2], int out[8][5][2])
{
    for(int i = 0; i < 8; i++)
    {

        for(int j = 0; j < 5; j++)
        {
            out[i][j][0] = in[i][j][0];
            out[i][j][1] = in[i][j][1];
        }
    }
}