
#include "defines.h"
#include "standard.h"
#include "text.h"

const SDL_Color White = { 255, 255, 255, 255 };
const SDL_Color Black = { 0,   0,   0, 255 };
Text_t* initText(const char* path_to_font, int fontsize, SDL_Renderer* pRenderer, SDL_Rect rect, SDL_Color color, bool centered, const char* text)
{
    if(TTF_Init() == -1)
    {
        printfd("FONT ERROR\n");
        return 0;
    }
    TTF_Font* font = TTF_OpenFont(path_to_font, fontsize);
    if(!font)
    {
        printfd("COULD NOT LOAD FONT\n");
        return NULL;
    }
    Text_t* pText = malloc(sizeof(Text_t));
    pText->pRenderer = pRenderer;
    pText->color = color;
    pText->fontsize = fontsize;
    pText->font = font;
    pText->rect = rect;
    pText->centered = centered;
    strcpy(pText->templateString, text);
    if (strcspn(text, "%") == strlen(text))
        strcpy(pText->printString, text);
    else
        strcpy(pText->printString, "");
    // strcpy(pText->printString, text);
    pText->pSurface = TTF_RenderText_Solid_Wrapped(pText->font, pText->printString, pText->color, 0);
    pText->pTexture = SDL_CreateTextureFromSurface(pText->pRenderer, pText->pSurface);
    return pText;
}



// SDL_Rect drawText ( SDL_Renderer* pRenderer, SDL_Color color, int fontsize, SDL_Rect rect, bool centered, const char* text, ... )
// {
//     TTF_Font* font = initText ( "./assets/BigBlueTermPlusNerdFont-Regular.ttf", fontsize );
//     if (strlen ( text ) > 0)
//     {
//         va_list ap;
//         va_start ( ap, text );
//         char buffer[100];
//         vsprintf ( buffer, text, ap );
//         TTF_SetFontSize ( font, fontsize );
//         SDL_Surface* surface = TTF_RenderText_Solid ( font, buffer, color );
//         SDL_Texture* texture = SDL_CreateTextureFromSurface ( pRenderer, surface );
//         rect.w = surface->w;
//         rect.h = surface->h;
//         if (centered)
//         {
//             rect.x -= rect.w / 2;
//         }
//         SDL_FreeSurface ( surface );
//         SDL_RenderCopy ( pRenderer, texture, NULL, &rect );
//         SDL_DestroyTexture ( texture );
//     }
//     return rect;
// }
void drawText(Text_t* pText)
{
    if(strlen(pText->printString) == 0)
    {
        return;

    }
    SDL_Rect rect = pText->rect;
    rect.w = pText->pSurface->w;
    rect.h = pText->pSurface->h;
    if(pText->centered)
    {
        rect.x -= rect.w / 2;
        rect.y -= rect.h / 2;
    }

    SDL_RenderCopy(pText->pRenderer, pText->pTexture, NULL, &rect);
}
SDL_Rect updateText(Text_t* pText, ...)
{
    TTF_SetFontSize(pText->font, pText->fontsize);
    if(pText->pSurface)
        SDL_FreeSurface(pText->pSurface);
    if(pText->pSurface)
        SDL_DestroyTexture(pText->pTexture);
    va_list args;
    va_start(args, pText);
    char buffer[200];
    vsprintf(pText->printString, pText->templateString, args);
    va_end(args);
    pText->pSurface = TTF_RenderText_Solid_Wrapped(pText->font, pText->printString, pText->color, 0);
    pText->pTexture = SDL_CreateTextureFromSurface(pText->pRenderer, pText->pSurface);
    SDL_Rect rect = pText->rect;
    rect.w = pText->pSurface->w;
    rect.h = pText->pSurface->h;
    if(pText->centered)
    {
        rect.x -= rect.w / 2;
        rect.y -= rect.h / 2;
    }
    return rect;
// strcpy(pText->printString,buffer );
}
void destroyText(Text_t* pText)
{
    if(pText->pSurface)
        SDL_FreeSurface(pText->pSurface);
    if(pText->pTexture)
        SDL_DestroyTexture(pText->pTexture);
    if(pText->font)
        TTF_CloseFont(pText->font);
    free(pText);
}

void textInput(SDL_Event event, char buffer[])
{
    const char* keyName = SDL_GetKeyName(event.key.keysym.sym);
    int bufferlen = strlen(buffer);
    if(bufferlen < 3 && strlen(keyName) == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z')
    {
        strcat(buffer, keyName);
    }
    if(strcmp(keyName, "Backspace") == 0 && bufferlen > 0)
    {
        int len = strlen(buffer);
        buffer[len - 1] = '\0';
    }
}

