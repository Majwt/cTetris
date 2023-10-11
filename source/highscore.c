#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "highscore.h"
#include "defines.h"
#include "special.h"

Pair createPair(char name[], int score)
{

	Pair tmp;
	tmp.score = score;
	strcpy(tmp.name, name);
	return tmp;
}

void SaveHighscore(Pair highscores[], int size)
{
	sortScores(highscores,size);
	if (size == HIGHSCORE_MAX_SAVES) {
		size-=1;
	}
	FILE *ptr = fopen(HIGHSCORE_SAVE_FILENAME, "w");
	if (ptr == NULL)
	{
		printfd("Error!");
		exit(1);
	}
	fprintf(ptr, "scores:\n");
	printPairs(highscores,size);
	for (int i = 0; i < size; i++)
	{
		fprintf(ptr, "%s %d\n", highscores[i].name, highscores[i].score);
	}

	fclose(ptr);
}

void LoadHighscore(Pair highscores[], int *size)
{
	FILE *ptr = fopen(HIGHSCORE_SAVE_FILENAME, "r");
	if (ptr == NULL)
	{
		printfd("CREATING HIGHSCORE FILE\n");
		
		SaveHighscore(highscores, *size);
		fclose(ptr);
		LoadHighscore(highscores, size);
		return;
	}
	char name[4];
	int readScore = 0;
	char line[100];
	char *result;
	while ((result = fgets(line,100,ptr)) != NULL)
	{
		int len = strlen(result);
		if ((len > 0 && result[len-1] == '\n') && !strcmp(result,"scores:\n") == 0) {
			result[len-1]=0;
			sscanf(result,"%s %d",name,&readScore);
			printfd("%s %d\n",name,readScore);
			if (readScore > 0) {
				InsertScore(highscores,size,name,readScore);
			}
		}
	}
	fclose(ptr);

	sortScores(highscores,*size);
}

void sortScores(Pair highscores[], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (highscores[j].score < highscores[j + 1].score)
			{
				swapPair(&highscores[j], &highscores[j + 1]);
			}
		}
	}
}
bool InsertScore(Pair highscores[], int *size, char name[], int score)
{

	if (*size + 1 > HIGHSCORE_MAX_SAVES)
	{
		return 1;
	}
	
	Pair tmp = createPair(name, score);
	highscores[*size] = tmp;
	sortScores(highscores, *size);
	printfd("size: %d\n",*size);
	(*size)++;
	return 0;
}
void swapPair(Pair *A, Pair *B)
{
	Pair tmp = *A;
	*A = *B;
	*B = tmp;
}

void printPairs(Pair highscores[],int size)
{
	printfd("print\n");
	for (int i = 0; i < size; i++)
	{
		printfd("%s %d\n", highscores[i].name, highscores[i].score);
	}
	printfd("\n");
}
void printPair(Pair score)
{
	printfd("name: %s, score: %d\n", score.name, score.score);
}
