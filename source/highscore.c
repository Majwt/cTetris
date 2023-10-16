#include <SDL.h>
#include <SDL_ttf.h>
#include "text.h"
#include "defines.h"
#include "standard.h"
#include "highscore.h"

Score_t createScore ( char name[], int score, int level, int lines, int startingLevel )
{

    Score_t tmp;
    tmp.score = score;
    tmp.level = level;
    tmp.lines = lines;
    tmp.startingLevel = startingLevel;
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
    // fwrite ( &tmp, sizeof ( Highscores_t ), 1, fp );
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
        if (highscores->scores[i].score > 0)
            fprintf ( fp, "%s\n", scoreToString ( highscores->scores[i] ) );
    }

    fclose ( fp );
    return true;
}

bool loadHighscoresTxt ( Highscores_t* highscores )
{
    if (highscores == NULL) {
        printfd ( "highscores is NULL\n" );
        highscores = malloc ( sizeof ( Highscores_t ) );
        highscores->size = 0;
        highscores->pHighscoreText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", 15, NULL, (SDL_Rect) { 0, 0, 0, 0 }, White, true, "HIGHSCORES" );
        highscores->pScoreText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", 12, NULL, (SDL_Rect) { 0, 0, 0, 0 }, White, false, "%3s %-7d" );
        return false;
    }
    char filename[100] = HIGHSCORE_FILENAME;
    strcat ( filename, ".txt" );
    FILE* fp = fopen ( filename, "r" );

    if (fp == NULL) {
        printfd ( "Error opening file for reading.\n" );
        return false;
    }

    highscores->size = 0;
    while (fscanf ( fp, "%d %s %d %d %d",
        &highscores->scores[highscores->size].score,
        highscores->scores[highscores->size].name,
        &highscores->scores[highscores->size].level,
        &highscores->scores[highscores->size].lines,
        &highscores->scores[highscores->size].startingLevel ) == 5) {
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

bool saveHighscoresBin ( Highscores_t* highscores )
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

bool loadHighscoresBin ( Highscores_t* highscores )
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
            if (highscores->scores[j].score < highscores->scores[j + 1].score)
            {
                swapScore ( &highscores->scores[j], &highscores->scores[j + 1] );
            }
        }
    }
}
bool insertScore ( Highscores_t* highscores, Score_t score )
{

    if (highscores->size + 1 > HIGHSCORE_MAX_SAVES)
    {
        return 1;
    }

    highscores->scores[highscores->size] = score;
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
void displayScoreboard ( Highscores_t highscore, Score_t player, SDL_Renderer* pRenderer, SDL_Rect position )
{

    highscore.pHighscoreText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", 15, pRenderer, position, White, true, "HIGHSCORES" );
    highscore.pScoreText = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", 12, pRenderer, position, White, false, "%3s %-7d" );
    if (player.score > 0 && strcmp ( player.name, "YOU" ) == 0) {
        highscore.scores[highscore.size] = player;
        highscore.size++;
        sortScores ( &highscore );
    }

    showText ( highscore.pHighscoreText );
    highscore.pScoreText->rect.x = position.x;
    highscore.pScoreText->rect.y = position.y + 20;
    for (int i = 0; i < highscore.size; i++)
    {
        if (highscore.scores[i].score > 0) {
            highscore.pScoreText->rect.y += showText ( highscore.pScoreText, highscore.scores[i].name, highscore.scores[i].score ).h + 10;
        }
    }

}
char* scoreToString ( Score_t score )
{
    char* tmp = malloc ( sizeof ( char ) * 100 );
    sprintf ( tmp, "%d %s %d %d %d", score.score, score.name, score.level, score.lines, score.startingLevel );
    return tmp;
}

