#ifndef highscore_h
#define highscore_h
struct pairType
{
	int score;
	char name[4];
};
typedef struct pairType Pair;

Pair createPair(char name[],int score);
void SaveHighscore(Pair highscores[],int size);
void LoadHighscore(Pair highscores[],int *size);
void sortScores(Pair highscores[],int size);
bool InsertScore(Pair highscores[],int *size, char name[], int score);
void swapPair(Pair *A,Pair *B);
void printPairs(Pair highscores[],int size);
void printPair(Pair score);

#endif