#ifndef game_h
#define game_h

#include <SDL.h>
#include <SDL_ttf.h>
#include "standard.h"
#include "defines.h"
#include "highscore.h"
#include "text.h"
#include "tetromino.h"
#include "board.h"

enum gamestate
{
	PLAY,
	GAMEOVER,
	QUIT,
	MENU
};
typedef enum gamestate GameState;


struct game
{
	SDL_Window *pWindow;
	SDL_Renderer *pRenderer;
    SDL_Event event;
	Board *pBoard;
	Highscores_t highscores;
	int score;
	int level;
	int lines;
	int gravity;
	GameState state;
    Text_t *pTetrisText;
    Text_t *pStartingLevelText;
    Text_t *pStartText;
    Text_t *pScoreText;
    Text_t *pGameOverText;
    Text_t *pNameText;
    const uint8_t *keysPressed;
    
};
typedef struct game Game;


int initGame(Game *pGame);
void runGame(Game *pGame);
void closeGame(Game *pGame);

void handleInput(Game *pGame);
void initBoard(Game *pGame);
void mainMenuView(Game *pGame);
void drawGameUI(Game *pGame);
void gameOverView(Game *pGame);



#endif