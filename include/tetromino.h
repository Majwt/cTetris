#ifndef tetromino_h
#define tetromino_h



enum pieceType {I=1,O,T,J,L,S,Z};
typedef struct tetromino
{
	// top left
	int x;
	int y;
	int orientationIndex;
	int width;
	int orientations[4][4][4];
	int srsTests[8][5][2];
    enum pieceType type;
} Tetromino_t;

Tetromino_t createPiece(int n);
void fillTetrinoOrientation(Tetromino_t *pTetromino, int orientation[4][4][4]);
void randomPiece(Tetromino_t *piece);
int getSRSindex(int from, int to);
void copySRS(int in[8][5][2],int out[8][5][2]);

#endif