#include <SDL.h>
#include <SDL_ttf.h>
#include "text.h"
#include "defines.h"
#include "standard.h"
#include "highscore.h"

Score_t createScore(char name[], int score, int level, int lines, int startingLevel)
{

    Score_t tmp;
    tmp.score = score;
    tmp.level = level;
    tmp.lines = lines;
    tmp.startingLevel = startingLevel;
    strcpy(tmp.name, name);
    return tmp;
}

Highscores_t initHighscore(SDL_Renderer* pRenderer, SDL_Rect position)
{
    Highscores_t highscores;

    highscores.size = 0;
    highscores.pHighscoreText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", 15, pRenderer, position, White, false, "HIGHSCORES");
    highscores.pScoreText = initText("./assets/BigBlueTermPlusNerdFont-Regular.ttf", 12, pRenderer, position, White, false, "%3s %-7d");
    highscores.pRenderer = pRenderer;
    highscores.position = position;
    return highscores;
}

void createHighscoresTxtFile()
{
    printfd("CREATING HIGHSCORE FILE\n");
    const char* filename = HIGHSCORE_FILENAME;
    printfd("filename: %s\n", filename);
    FILE* fp = fopen(filename, "wb");
    Highscores_t tmp;
    tmp.size = 0;
    fclose(fp);
}

bool saveHighscoresTxt(Highscores_t* highscores)
{
    sortScores(highscores);
    const char* filename = HIGHSCORE_FILENAME;
    FILE* fp = fopen(filename, "w");
    if(fp == NULL)
    {
        printfd("Error opening file for writing.\n");
        return false;
    }

    for(int i = 0; i < highscores->size; i++)
    {
        if(highscores->scores[i].score > 0)
        {
            fprintf(fp, "%s\n", scoreToString(highscores->scores[i]));
        }
    }

    fclose(fp);
    return true;
}

bool loadHighscoresTxt(Highscores_t* highscores)
{

    const char* filename = HIGHSCORE_FILENAME;
    FILE* fp = fopen(filename, "r");

    if(fp == NULL)
    {
        printfd("Error opening file for reading.\n");
        return false;
    }

    highscores->size = 0;
    while(fscanf(fp, "%d %s %d %d %d",
                 &highscores->scores[highscores->size].score,
                 highscores->scores[highscores->size].name,
                 &highscores->scores[highscores->size].level,
                 &highscores->scores[highscores->size].lines,
                 &highscores->scores[highscores->size].startingLevel) == 5 && highscores->size < HIGHSCORE_MAX_SAVES)
    {
        highscores->size++;
    }

    fclose(fp);
    return true;
}

void sortScores(Highscores_t* highscores)
{
    for(int i = 0; i < highscores->size; i++)
    {
        for(int j = 0; j < highscores->size - i - 1; j++)
        {
            if(highscores->scores[j].score < highscores->scores[j + 1].score)
            {
                swapScore(&highscores->scores[j], &highscores->scores[j + 1]);
            }
        }
    }
}
/// @brief a>b
/// @param a 
/// @param b 
/// @return true: a>b, false: a<b
bool a_bigger(Score_t a, Score_t b)
{

    if(a.score > b.score)
    {
        return true;
    }
    else if(a.score == b.score)
    {
        if(a.level > b.level)
        {
            return true;
        }
        else if(a.level == b.level)
        {
            if(a.lines > b.lines)
            {
                return true;
            }
            else if(a.lines == b.lines)
            {
                if(a.startingLevel > b.startingLevel)
                {
                    return true;
                }

            }
        }
    }
    return false;
}
bool insertScore(Highscores_t* highscores, Score_t score)
{

    if(highscores->size + 1 >= HIGHSCORE_MAX_SAVES)
    {
        if(!a_bigger(score, highscores->scores[highscores->size - 1]))
        {
            return 1;
        }
        highscores->scores[highscores->size - 1] = score;
    }
    else
    {
        highscores->scores[highscores->size] = score;
        highscores->size++;
    }
    printfd("inserted: %d\n", highscores->size);

    sortScores(highscores);
    return 0;
}
void swapScore(Score_t* A, Score_t* B)
{
    Score_t tmp = *A;
    *A = *B;
    *B = tmp;
}
void displayScoreboard(Highscores_t highscore, Score_t* player)
{
    if(player)
    {
        if(strcmp(player->name, "YOU") == 0 && highscore.size <= HIGHSCORE_MAX_SAVES)
        {
            highscore.scores[highscore.size] = *player;
            highscore.size++;
        }
        else if(a_bigger(*player, highscore.scores[highscore.size - 1]))
        {
            highscore.scores[highscore.size - 1] = *player;
        }
        sortScores(&highscore);
    }
    if(!highscore.pHighscoreText || !highscore.pScoreText)
    {
        printfd("highscore text is null\n");
        return;
    }
    drawText(highscore.pHighscoreText);
    highscore.pScoreText->rect.x = highscore.pHighscoreText->rect.x;
    highscore.pScoreText->rect.y = highscore.pHighscoreText->rect.y + 10;
    for(int i = 0; i < highscore.size; i++)
    {
        if(i == 0)
        {
            highscore.pScoreText->color = (SDL_Color){ 255, 215, 0, 255 };
        }
        else if(i == 1)
        {
            highscore.pScoreText->color = (SDL_Color){ 192, 192, 192, 255 };
        }
        else if(i == 2)
        {
            highscore.pScoreText->color = (SDL_Color){ 205, 127, 50, 255 };
        }
        if(player && highscore.scores[i].score == player->score && strcmp(highscore.scores[i].name, player->name) == 0)
        {
            highscore.pScoreText->color = (SDL_Color){ 0, 255, 0, 255 };
        }
        highscore.pScoreText->rect.y += updateText(highscore.pScoreText, highscore.scores[i].name, highscore.scores[i].score).h + 10;
        drawText(highscore.pScoreText);
        highscore.pScoreText->color = White;
    }

}
char* scoreToString(Score_t score)
{
    char* tmp = malloc(sizeof(char) * 100);
    sprintf(tmp, "%d %s %d %d %d", score.score, score.name, score.level, score.lines, score.startingLevel);
    return tmp;
}

