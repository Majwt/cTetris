#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "defines.h"
#include "highscore.h"
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

	int zero = 0;
	fwrite(&zero, sizeof(int), 1, fp);
	fclose(fp);
}

bool SaveHighscore(Highscores_t highscores)
{
	sortScores(&highscores);
	if (highscores.size == HIGHSCORE_MAX_SAVES)
	{
		highscores.size -= 1;
	}
	FILE *fp = fopen(HIGHSCORE_SAVE_FILENAME, "wb");
	if (fp == NULL)
	{
		printfd("Error!");
		fclose(fp);
		return false;
	}
	if (fwrite(&highscores.size, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return false;
	}
	if (fwrite(highscores.scores, sizeof(Score), highscores.size, fp) != highscores.size)
	{
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;
}

bool LoadHighscore(Highscores_t *highscores)
{
	FILE *fp = fopen(HIGHSCORE_SAVE_FILENAME, "rb");
	if (fp == NULL)
	{
		fclose(fp);
		return false;
	}

	if (fread(&highscores->size, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return false;
	}

	if (highscores->size > 0 && fread(&highscores->scores, sizeof(Score), highscores->size, fp) != highscores->size)
	{
		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}

void sortScores(Highscores_t *highscores)
{
	for (int i = 0; i < highscores->size; i++)
	{
		for (int j = 0; j < highscores->size - i - 1; j++)
		{
			if (highscores->scores[j].value < highscores->scores[j + 1].value)
			{
				swapScore(&highscores->scores[j], &highscores->scores[j + 1]);
			}
		}
	}
}
bool InsertScore(Highscores_t *highscores, char name[], int value)
{

	if (highscores->size + 1 > HIGHSCORE_MAX_SAVES)
	{
		return 1;
	}

	Score tmp = createScore(name, value);
	highscores->scores[highscores->size] = tmp;
	sortScores(highscores);
	printfd("size: %d\n", highscores->size);
	highscores->size++;
	return 0;
}
void swapScore(Score *A, Score *B)
{
	Score tmp = *A;
	*A = *B;
	*B = tmp;
}

