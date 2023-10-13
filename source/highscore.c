#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "defines.h"
#include "highscore.h"
#include "special.h"

Score_t createScore ( char name[], int score )
{

    Score_t tmp;
    tmp.value = score;
    strcpy ( tmp.name, name );
    return tmp;
}

void createHighscoresTxtFile ()
{
    printfd ( "CREATING HIGHSCORE FILE\n" );
    char filename[100] = HIGHSCORE_FILENAME;
    strcat ( filename, ".txt" );
    printfd ( "filename: %s\n", filename );
    FILE* fp = fopen ( filename, "wb" );
    Highscores_t tmp;
    tmp.size = 0;
    fwrite ( &tmp, sizeof ( Highscores_t ), 1, fp );
    fclose ( fp );
}

bool saveHighscoresTxt ( Highscores_t* highscores )
{
    sortScores ( highscores );
    char filename[100] = HIGHSCORE_FILENAME;
    strcat ( filename, ".txt" );
    FILE* fp = fopen ( filename, "w" );
    if (fp == NULL) {
        printfd ( "Error opening file for writing.\n" );
        return false;
    }

    for (int i = 0; i < highscores->size; i++) {
        fprintf ( fp, "%d %s\n", highscores->scores[i].value, highscores->scores[i].name );
    }

    fclose ( fp );
    return true;
}

bool loadHighscoresTxt ( Highscores_t* highscores )
{
    char filename[100] = HIGHSCORE_FILENAME;
    strcat ( filename, ".txt" );
    FILE* fp = fopen ( filename, "r" );

    if (fp == NULL) {
        printfd ( "Error opening file for reading.\n" );
        return false;
    }

    highscores->size = 0;
    while (fscanf ( fp, "%d %s", &highscores->scores[highscores->size].value, highscores->scores[highscores->size].name ) == 2) {
        highscores->size++;
    }

    fclose ( fp );
    return true;
}

void createHighscoresBinFile ()
{
    printfd ( "CREATING HIGHSCORE FILE\n" );
    char filename[100] = HIGHSCORE_FILENAME;
    strcat ( filename, ".bin" );
    printfd ( "filename: %s\n", filename );
    FILE* fp = fopen ( filename, "wb" );
    Highscores_t tmp;
    tmp.size = 0;
    fwrite ( &tmp, sizeof ( Highscores_t ), 1, fp );
    fclose ( fp );
}

bool SaveHighscoresBin ( Highscores_t* highscores )
{
    sortScores ( highscores );
    char filename[100] = HIGHSCORE_FILENAME;
    strcat ( filename, ".bin" );
    printfd ( "filename: %s\n", filename );
    FILE* fp = fopen ( filename, "wb" );
    if (fp == NULL)
    {
        printfd ( "Error!" );
        return false;
    }
    if (fwrite ( highscores, sizeof ( Highscores_t ), 1, fp ) != 1) {

        fclose ( fp );
        return false;
    }
    fclose ( fp );
    return true;
}

bool LoadHighscoresBin ( Highscores_t* highscores )
{
    char filename[100] = HIGHSCORE_FILENAME;
    strcat ( filename, ".bin" );
    printfd ( "filename: %s\n", filename );
    FILE* fp = fopen ( filename, "rb" );
    if (fp == NULL)
    {
        printfd ( "NULL: %s\n", filename );
        return false;
    }

    if (fread ( highscores, sizeof ( Highscores_t ), 1, fp ) != 1)
    {
        fclose ( fp );
        return false;
    }

    fclose ( fp );
    return true;
}

void sortScores ( Highscores_t* highscores )
{
    for (int i = 0; i < highscores->size; i++)
    {
        for (int j = 0; j < highscores->size - i - 1; j++)
        {
            if (highscores->scores[j].value < highscores->scores[j + 1].value)
            {
                swapScore ( &highscores->scores[j], &highscores->scores[j + 1] );
            }
        }
    }
}
bool insertScore ( Highscores_t* highscores, char name[], int value )
{

    if (highscores->size + 1 > HIGHSCORE_MAX_SAVES)
    {
        return 1;
    }

    Score_t tmp = createScore ( name, value );
    highscores->scores[highscores->size] = tmp;
    sortScores ( highscores );
    printfd ( "size: %d\n", highscores->size );
    highscores->size++;
    return 0;
}
void swapScore ( Score_t* A, Score_t* B )
{
    Score_t tmp = *A;
    *A = *B;
    *B = tmp;
}

