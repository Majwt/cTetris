#ifndef board_h
#define board_h
#include <stdbool.h>

typedef struct board Board_t;

struct board
{
	int x, y;
	int width, height;
	SDL_Renderer *pRenderer;
	int occupied[BOARD_ROWS][BOARD_COLUMNS];
	SDL_Color colors[7];
	Tetromino_t activeTetromino;
	Tetromino_t nextTetromino;
	double onGroundTime;
	bool onGround;
	double now_t, prev_t;
    Text_t *pScoreText;
    Text_t *pLevelText;
    Text_t *pLinesText;
    Text_t *pNextText;
};

Board_t *CreateBoard(SDL_Renderer *pRenderer);
void DrawOccupied(Board_t *board);
void drawTetromino(Board_t *pBoard);
void drawBoard(Board_t* pBoard, Score_t* player);

int TetrominoCollisionCheck(Board_t *pBoard,int offsetX,int offsetY, int orientation);

void updateOnGroundTime(Board_t *pBoard);

void movePiece(Board_t *pBoard, int dx,int dy);
bool rotatePiece(Board_t *pBoard, int direction);

void convertToStatic(Board_t *pBoard);
void beginNextRound(Board_t *pBoard,Score_t *player);
bool isTetrominoOnGround(Board_t *pBoard);
int addPoints(int level, int lines);
int RemoveAllCompleteRows(Board_t *pBoard);
void MoveRowsDown1(Board_t *pBoard, int y);
void showNextPiece(Board_t *pBoard, int x, int y);
bool gameOverCheck(Board_t *pBoard);
#endif