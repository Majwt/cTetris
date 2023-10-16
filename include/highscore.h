#ifndef highscore_h
#define highscore_h
#include "text.h"

typedef struct 
{
	char name[4];
	int value;
} Score_t;

typedef struct {
	int size;
	Score_t scores[HIGHSCORE_MAX_SAVES+1];
    Text_t *pHighscoreText;
    Text_t *pScoreText;
} Highscores_t;

Score_t createScore(char name[],int score);
void createHighscoresTxtFile();
bool saveHighscoresTxt(Highscores_t *highscores);
bool loadHighscoresTxt(Highscores_t *highscores);

void createHighscoresBinFile();
bool saveHighscoresBin(Highscores_t *highscores);
bool loadHighscoresBin(Highscores_t *highscores);

void sortScores(Highscores_t *highscores);
bool insertScore(Highscores_t *highscores, char name[], int score);

void swapScore(Score_t *A,Score_t *B);

void displayScoreboard(Highscores_t highscore,SDL_Renderer* pRenderer,SDL_Rect position);
#endif