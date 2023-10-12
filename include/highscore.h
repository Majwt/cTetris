#ifndef highscore_h
#define highscore_h
struct score
{
	char name[4];
	int value;
};
typedef struct score Score;

typedef struct _highscore {
	int size;
	Score scores[HIGHSCORE_MAX_SAVES+1];
} Highscores_t;

Score createScore(char name[],int score);
void createHighscoreFile();
bool SaveHighscore(Highscores_t highscores);
bool LoadHighscore(Highscores_t *highscores);
void sortScores(Highscores_t *highscores);
bool InsertScore(Highscores_t *highscores, char name[], int score);

void swapScore(Score *A,Score *B);

#endif