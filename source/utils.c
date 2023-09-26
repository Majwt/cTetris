#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "defines.h"
#include "util.h"

const SDL_Color White = {255, 255, 255, 255};
const SDL_Color Black = {  0,   0,   0, 255};
TTF_Font* font;
int InitFont(const char* path_to_font) {
    if (TTF_Init() == -1) {
        printf("FONT ERROR\n");
        return 0;
    }
    font = TTF_OpenFont(path_to_font, FONT_SIZE);
    if (!font)
    {
        printf("COULD NOT LOAD FONT\n");
        return 0;
    }
    return 1;
}

void test(const char* format, ...) {
    va_list ap;
    va_start(ap,format);
    
    char buffer[100];
    vsprintf(buffer,format,ap);
    printf(buffer);
}

SDL_Rect ShowText(SDL_Renderer *pRenderer, SDL_Color color, int fontsize, SDL_Rect rect, bool centered,const char* text, ...)
{
    if (strlen(text) > 0)
    {
        va_list ap;
        va_start(ap,text);
        char buffer[100];
        vsprintf(buffer,text,ap);
        TTF_SetFontSize(font,fontsize);
        SDL_Surface *surface = TTF_RenderText_Solid(font, buffer, color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(pRenderer, surface);
        rect.w = surface->w;
        rect.h = surface->h;
        if (centered)
        {
            rect.x -= rect.w / 2;
        }
        SDL_FreeSurface(surface);
        SDL_RenderCopy(pRenderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    }
    return rect;
}

void TextInput(SDL_Event event, char buffer[])
{
    const char *keyName = SDL_GetKeyName(event.key.keysym.sym);
    int bufferlen = strlen(buffer);
    if (bufferlen < 8 && strlen(keyName) == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z')
    {
        strcat(buffer, keyName);
    }
    else if (strcmp(keyName, "Backspace") == 0 && bufferlen > 0)
    {
        int len = strlen(buffer);
        buffer[len - 1] = '\0';
    }
}
bool PointRectCollision(int x, int y, SDL_Rect rect)
{
    return (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h);
}
