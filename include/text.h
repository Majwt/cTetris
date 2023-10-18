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
    SDL_Surface *pSurface;
    SDL_Texture *pTexture;
    SDL_Color color;
    int fontsize;
    TTF_Font* font;
    char templateString[100];
    char printString[100];
    SDL_Rect rect;
    bool centered;
};

Text_t* initText(const char* path_to_font, int fontsize, SDL_Renderer* pRenderer, SDL_Rect rect, SDL_Color color, bool centered, const char* text );
void textInput(SDL_Event event, char buffer[]);
void drawText(Text_t *pText);
SDL_Rect updateText(Text_t *pText, ...);
void destroyText(Text_t *pText);
// SDL_Rect drawText(SDL_Renderer *pRenderer, SDL_Color color, int fontsize, SDL_Rect rect, bool centered,const char* text, ...);


#endif