#ifndef highscore_h
#define highscore_h
struct pairType
{
	int score;
	char name[10];
};
typedef struct pairType Pair;

Pair createPair(char name[],int score);
void SaveHighscore(Pair highscores[]);
void LoadHighscore(Pair highscores[]);
void sortScores(Pair highscores[]);
bool InsertScore(Pair highscores[], char name[], int score);
void swapPair(Pair *A,Pair *B);
void printPairs(Pair highscores[]);
void printPair(Pair score);
#endif