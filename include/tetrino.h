#ifndef tetrino_h
#define tetrino_h
typedef struct tetrino Tetrino;

struct tetrino
{
    // top left
    int x;
    int y;
    int orientationIndex;
    int width; // oanvänd men går inte att ta bort
    int orientations[4][4][4];
};

Tetrino CreatePiece(int n);
Tetrino CreateIpiece();
Tetrino CreateTpiece();
Tetrino CreateLpiece();
Tetrino CreateJpiece();
Tetrino CreateSpiece();
Tetrino CreateZpiece();
Tetrino CreateOpiece();
void fillTetrinoOrientation(Tetrino *pTetrino, int orientation[4][4][4]);
void RandomPiece(Tetrino *piece);

#endif