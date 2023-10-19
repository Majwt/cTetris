#ifndef highscore_h
#define highscore_h
#include "text.h"

typedef struct
{
    char name[4];
    int score;
    int level;
    int lines;
    int startingLevel;
    bool passedThreshold;
} Score_t;

typedef struct
{
    int size;
    Score_t scores[HIGHSCORE_MAX_SAVES + 1];
    Text_t* pHighscoreText;
    Text_t* pScoreText;
    SDL_Renderer* pRenderer;
    SDL_Rect position;
} Highscores_t;

Highscores_t initHighscore(SDL_Renderer* pRenderer, SDL_Rect position);

Score_t createScore(char name[], int score, int level, int lines, int startingLevel);
char* scoreToString(Score_t score);
void createHighscoresTxtFile();
bool saveHighscoresTxt(Highscores_t* highscores);
bool loadHighscoresTxt(Highscores_t* highscores);

void createHighscoresBinFile();
bool saveHighscoresBin(Highscores_t* highscores);
bool loadHighscoresBin(Highscores_t* highscores);

void sortScores(Highscores_t* highscores);
bool insertScore(Highscores_t* highscores, Score_t score);

void swapScore(Score_t* A, Score_t* B);

void displayScoreboard(Highscores_t highscore, Score_t player);
#endif