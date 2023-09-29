#ifndef board_h
#define board_h

typedef struct board Board;

Board *CreateBoard(SDL_Renderer *pRenderer);
void DrawOccupied(Board *board);
void DrawTetromino(Board *pBoard);
int TetrominoCollisionCheck(Board *pBoard);

void UpdateOnGroundTime(Board *pBoard);

void MoveSideways(Board *pBoard, int dx);
void MoveDown(Board *pBoard);
void RotateClockwise(Board *pBoard);
void RotateAntiClockwise(Board *pBoard);

void ConvertToStatic(Board *pBoard);
void NextRound(Board *pBoard, int *score, int *level, int *lines);
bool isTetrominoOnGround(Board *pBoard);
int AddPoints(int level, int lines);
int RemoveAllCompleteRows(Board *pBoard);
void MoveRowsDown1(Board *pBoard, int y);
void ShowNextPiece(Board *pBoard, int x, int y);
bool GameOverCheck(Board *pBoard);
#endif