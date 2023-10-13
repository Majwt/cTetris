#ifndef board_h
#define board_h

typedef struct board Board;

struct board
{
	int x, y;
	int width, height;
	SDL_Renderer *pRenderer;
	int occupied[BOARD_ROWS][BOARD_COLUMNS];
	SDL_Color colors[7];
	Tetromino activeTetromino;
	Tetromino nextTetromino;
	double onGroundTime;
	bool onGround;
	double now_t, prev_t;
};

Board *CreateBoard(SDL_Renderer *pRenderer);
void DrawOccupied(Board *board);
void DrawTetromino(Board *pBoard);
int TetrominoCollisionCheck(Board *pBoard,int offsetX,int offsetY, int orientation);

void UpdateOnGroundTime(Board *pBoard);

void Move(Board *pBoard, int dx,int dy);
bool Rotation(Board *pBoard, int direction);

void ConvertToStatic(Board *pBoard);
void NextRound(Board *pBoard, int *score, int *level, int *lines);
bool isTetrominoOnGround(Board *pBoard);
int AddPoints(int level, int lines);
int RemoveAllCompleteRows(Board *pBoard);
void MoveRowsDown1(Board *pBoard, int y);
void ShowNextPiece(Board *pBoard, int x, int y);
bool GameOverCheck(Board *pBoard);
#endif