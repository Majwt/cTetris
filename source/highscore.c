#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "highscore.h"
#include "defines.h"
#include "special.h"

Score createScore(char name[], int score)
{

	Score tmp;
	tmp.value = score;
	strcpy(tmp.name, name);
	return tmp;
}
void createHighscoreFile()
{
	printfd("CREATING HIGHSCORE FILE\n");
	FILE *fp = fopen(HIGHSCORE_SAVE_FILENAME, "wb");
	if (fp == NULL)
	{
		printfd("ERROR CREATING FILE\n");
		fclose(fp);
		return;
	}
	// FILE *fp = fopen(HIGHSCORE_SAVE_FILENAME, "wb");

	int zero = 0;
	fwrite(&zero, sizeof(int), 1, fp);
	fclose(fp);
}

bool SaveHighscore(Score highscores[], int size)
{
	sortScores(highscores, size);
	if (size == HIGHSCORE_MAX_SAVES)
	{
		size -= 1;
	}
	FILE *fp = fopen(HIGHSCORE_SAVE_FILENAME, "wb");
	if (fp == NULL)
	{
		printfd("Error!");
		fclose(fp);
		return false;
	}
	if (fwrite(&size, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return false;
	}
	if (fwrite(highscores, sizeof(Score), size, fp) != size)
	{
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;
}

bool LoadHighscore(Score highscores[], int *size)
{
	FILE *fp = fopen(HIGHSCORE_SAVE_FILENAME, "rb");
	if (fp == NULL)
	{
		fclose(fp);
		return false;
	}

	if (fread(size, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return false;
	}

	if (*size > 0 && fread(highscores, sizeof(Score), *size, fp) != *size)
	{
		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}

void sortScores(Score highscores[], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (highscores[j].value < highscores[j + 1].value)
			{
				swapScore(&highscores[j], &highscores[j + 1]);
			}
		}
	}
}
bool InsertScore(Score highscores[], int *size, char name[], int value)
{

	if (*size + 1 > HIGHSCORE_MAX_SAVES)
	{
		return 1;
	}

	Score tmp = createScore(name, value);
	highscores[*size] = tmp;
	sortScores(highscores, *size);
	printfd("size: %d\n", *size);
	(*size)++;
	return 0;
}
void swapScore(Score *A, Score *B)
{
	Score tmp = *A;
	*A = *B;
	*B = tmp;
}

