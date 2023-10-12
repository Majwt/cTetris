#ifndef highscore_h
#define highscore_h
struct score
{
	char name[4];
	int value;
};
typedef struct score Score;

Score createScore(char name[],int score);
void createHighscoreFile();
bool SaveHighscore(Score highscores[],int size);
bool LoadHighscore(Score highscores[],int *size);
void sortScores(Score highscores[],int size);
bool InsertScore(Score highscores[],int *size, char name[], int score);

void swapScore(Score *A,Score *B);

#endif