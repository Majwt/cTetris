#ifndef game_h
#define game_h

#include <SDL.h>
#include <SDL_ttf.h>
#include "common.h"
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
	Board_t *pBoard;
	Highscores_t highscores;
    Score_t player;
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
typedef struct game Game_t;


int initGame(Game_t *pGame);
void runGame(Game_t *pGame);
void closeGame(Game_t *pGame);

void handleEvents(Game_t *pGame);
void handleInput(Game_t *pGame);
void initBoard(Game_t *pGame);
void mainMenuView(Game_t *pGame);
void gameOverView(Game_t *pGame);



#endif