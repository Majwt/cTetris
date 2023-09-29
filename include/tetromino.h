#ifndef tetrino_h
#define tetrino_h
typedef struct tetromino Tetromino;


enum pieceType {I=1,O,T,J,L,S,Z};
struct tetromino
{
    // top left
    int x;
    int y;
    int orientationIndex;
    int width;
    int orientations[4][4][4];
};

Tetromino CreatePiece(int n);
void fillTetrinoOrientation(Tetromino *pTetromino, int orientation[4][4][4]);
void RandomPiece(Tetromino *piece);

#endif