#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "highscore.h"
#include "defines.h"

Pair createPair(char name[], int score)
{

    Pair tmp;
    tmp.score = score;
    strcpy(tmp.name,name);
    return tmp;
}

void SaveHighscore(Pair highscores[])
{
    sortScores(highscores);
    FILE *ptr = fopen(HIGHSCORE_SAVE_FILENAME, "w");
    if (ptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(ptr,"scores:\n");
    for (int i = 0; i < HIGHSCORE_MAX_SAVES; i++)
    {
        fprintf(ptr, "%s : %d\n", highscores[i].name, highscores[i].score);
    }

    fclose(ptr);
}

void LoadHighscore(Pair highscores[])
{
    FILE *ptr = fopen(HIGHSCORE_SAVE_FILENAME, "r");
    if (ptr == NULL)
    {
        printf("CREATING HIGHSCORE FILE\n");
        for (int i = 0; i < HIGHSCORE_MAX_SAVES; i++)
        {
            highscores[i] = createPair("nan",0);
        }
        SaveHighscore(highscores);
        fclose(ptr);
        LoadHighscore(highscores);
        return;
    }
    char name[100];
    int readScore = 0;
    int i = 0;
    char test[10];
    fscanf(ptr,"%s\n",&test);
    if (strcmp(test,"scores:")) {
        printf("FILE ERROR\n");
        exit(1);
    }
    while (fscanf(ptr, "%s : %d", name, &readScore) == 2)
    {
        highscores[i] = createPair(name,readScore);
        i++;
    }
    fclose(ptr);
    if (i < HIGHSCORE_MAX_SAVES) {
        for (int j = i; j < HIGHSCORE_MAX_SAVES; j++)
        {
            highscores[j] = createPair("nan",0);
        }
        
    }
    sortScores(highscores);
}

void sortScores(Pair highscores[])
{
    for (int i = 0; i < HIGHSCORE_MAX_SAVES; i++)
    {
        for (int j = 0; j < HIGHSCORE_MAX_SAVES - i - 1; j++)
        {
            if (highscores[j].score < highscores[j + 1].score)
            {
                swapPair(&highscores[j],&highscores[j+1]);
            }
        }
    }
}
bool InsertScore(Pair highscores[], char name[], int score)
{

    Pair tmp = createPair(name,score);
    bool inserted = false;
    for (int i = 0; i < HIGHSCORE_MAX_SAVES; i++)
    {
        if (inserted == false && tmp.score > highscores[i].score)
        {
            inserted = true;
            swapPair(&tmp,&highscores[i]);
        }
        else if (inserted == true && i < HIGHSCORE_MAX_SAVES) {
            swapPair(&tmp,&highscores[i]);
        }
    }
    return inserted;
}
void swapPair(Pair *A,Pair *B)
{
    Pair tmp = *A;
    *A = *B;
    *B = tmp;
}

void printPairs(Pair highscores[]) {
    printf("print\n");
    for (int i = 0; i < HIGHSCORE_MAX_SAVES; i++)
    {
        printf("%s : %d\n",highscores[i].name,highscores[i].score);
    }
    printf("\n");
    
}
void printPair(Pair score) {
    printf("name: %s, score: %d\n",score.name,score.score);
}