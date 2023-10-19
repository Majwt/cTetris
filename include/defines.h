#ifndef defines_h
#define defines_h


#define DEBUG 0
#define DEBUG_BOARD 0
#define DEBUG_PIECEID 0
#define USE_SRS 1

#define DRAW_GRID 1

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 600

#define TETROMINOSIZE 20

#define BOARD_COLUMNS 10
#define BOARD_ROWS 20

#define BOARD_WIDTH BOARD_COLUMNS * TETROMINOSIZE
#define BOARD_HEIGHT BOARD_ROWS * TETROMINOSIZE

#define BOARD_X (WINDOW_WIDTH / 2 - BOARD_WIDTH/2)
#define BOARD_Y (WINDOW_HEIGHT / 2 - BOARD_HEIGHT/2)

#define ONGROUND_TIME_MIN 0.05

#define HIGHSCORE_FILENAME "highscores.txt"
#define HIGHSCORE_MAX_SAVES 10

// including the null terminator
#define HIGHSCORE_NAME_MAX_LENGTH 4

#define FONT_SIZE 25

#endif