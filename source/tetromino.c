#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "tetromino.h"
#include "special.h"

void changeSignSRS(int in[5][2], int out[5][2])
{
	for (int i = 0; i < 5; i++)
	{
		out[i][0] = -in[i][0];
		out[i][1] = -in[i][1];
	}
}
void fillTetrominoOrientation(Tetromino *pTetromino, int orientation[4][4][4])
{
	pTetromino->orientationIndex = 0;
	pTetromino->oldorientationIndex = 0;
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
		  {0, 0, 0, 0}}}};

	// srsPair srs[2][5][8];
	static int srs[2][8][5][2] = {
		// I
		{{{0, 0}, {-2, 0}, {+1, 0}, {-2, -1}, {+1, +2}},  // 0->1
		 {{0, 0}, {+2, 0}, {-1, 0}, {+2, +1}, {-1, -2}},  // 1->0 = -(0->1)
		 {{0, 0}, {-1, 0}, {+2, 0}, {-1, +2}, {+2, -1}},  // 1->2
		 {{0, 0}, {+1, 0}, {-2, 0}, {+1, -2}, {-2, +1}},  // 2->1 = -(1->2)
		 {{0, 0}, {+2, 0}, {-1, 0}, {+2, +1}, {-1, -2}},  // 2->3 = -(0->1)
		 {{0, 0}, {-2, 0}, {+1, 0}, {-2, -1}, {+1, +2}},  // 3->2 =   0->1
		 {{0, 0}, {+1, 0}, {-2, 0}, {+1, -2}, {-2, +1}},  // 3->0 = -(1->2)
		 {{0, 0}, {-1, 0}, {+2, 0}, {-1, +2}, {+2, -1}}}, // 0->3 =   1->2
		// J, L, S, T, Z
		{{{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},   // 0->1
		 {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},   // 1->0 = -(0->1)
		 {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},   // 1->2 = -(0->1)
		 {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},   // 2->1 =   0->1
		 {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}},   // 2->3
		 {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},   // 3->2 = -(2->3)
		 {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},   // 3->0 = -(2->3)
		 {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}}}}; // 0->3 =   2->3

	Tetromino piece;
	copySRS(srs[(n>1) ? 1 : 0],piece.srsTests);
	fillTetrominoOrientation(&piece, pieceOrientations[n]);
	piece.x = 3;
	piece.y = 0;
	if (n + 1 == I || n + 1 == O)
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
int getSRSindex(int from, int to)
{
	switch (from)
	{
	case 0:
		if (to == 1)
		{
			return 0;
		}
		else if (to == 3)
		{
			return 7;
		}
		return -1;
		break;
	case 1:
		if (to == 0)
		{
			return 1;
		}
		else if (to == 2)
		{
			return 2;
		}
		return -1;

		break;
	case 2:
		if (to == 1)
		{
			return 3;
		}
		else if (to == 3)
		{
			return 4;
		}
		return -1;

		break;
	case 3:
		if (to == 0)
		{
			return 6;
		}
		else if (to == 2)
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
	for (int i = 0; i < 8; i++)
	{

		for (int j = 0; j < 5; j++)
		{
			out[i][j][0] = in[i][j][0];
			out[i][j][1] = in[i][j][1];
		}
	}
}