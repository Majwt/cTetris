#ifndef text_h
#define text_h

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

extern const SDL_Color White;
extern const SDL_Color Black;

typedef struct text Text_t;
struct text
{
    SDL_Renderer *pRenderer;
    SDL_Color color;
    int fontsize;
    TTF_Font* font;
    char string[100];
    SDL_Rect rect;
    bool centered;
};

Text_t* initText(const char* path_to_font, int fontsize, SDL_Renderer* pRenderer, SDL_Rect rect, SDL_Color color, bool centered, const char* text );
void textInput(SDL_Event event, char buffer[]);
SDL_Rect showText(Text_t *pText, ...);
void destroyText(Text_t *pText);
// SDL_Rect showText(SDL_Renderer *pRenderer, SDL_Color color, int fontsize, SDL_Rect rect, bool centered,const char* text, ...);


#endif