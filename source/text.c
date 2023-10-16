
#include "defines.h"
#include "standard.h"
#include "text.h"

const SDL_Color White = { 255, 255, 255, 255 };
const SDL_Color Black = { 0,   0,   0, 255 };
Text_t* initText ( const char* path_to_font, int fontsize, SDL_Renderer* pRenderer, SDL_Rect rect, SDL_Color color, bool centered, const char* text )
{
    if (TTF_Init () == -1) {
        printfd ( "FONT ERROR\n" );
        return 0;
    }
    TTF_Font* font = TTF_OpenFont ( path_to_font, fontsize );
    if (!font)
    {
        printfd ( "COULD NOT LOAD FONT\n" );
        return NULL;
    }
    Text_t* pText = malloc ( sizeof ( Text_t ) );
    pText->pRenderer = pRenderer;
    pText->color = color;
    pText->fontsize = fontsize;
    pText->font = font;
    strcpy ( pText->string, text );
    pText->rect = rect;
    pText->centered = centered;
    return pText;
}



// SDL_Rect showText ( SDL_Renderer* pRenderer, SDL_Color color, int fontsize, SDL_Rect rect, bool centered, const char* text, ... )
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
SDL_Rect showText ( Text_t* pText, ... )
{
    SDL_Rect rect = pText->rect;
    if (strlen ( pText->string ) > 0)
    {
        va_list ap;
        va_start ( ap, pText );
        char buffer[200];
        vsprintf ( buffer, pText->string, ap );
        va_end ( ap );
        TTF_SetFontSize ( pText->font, pText->fontsize );
        // TTF_RenderText_Solid_Wrapped
        SDL_Surface* surface = TTF_RenderText_Solid_Wrapped ( pText->font, buffer, pText->color, 0);
        SDL_Texture* texture = SDL_CreateTextureFromSurface ( pText->pRenderer, surface );
        // pText->rect.w = surface->w;
        // pText->rect.h = surface->h;
        rect.w = surface->w;
        rect.h = surface->h;
        if (pText->centered)
        {
            rect.x -= rect.w / 2;
            rect.y -= rect.h / 2;
        }
        SDL_FreeSurface ( surface );
        SDL_RenderCopy ( pText->pRenderer, texture, NULL, &rect );
        SDL_DestroyTexture ( texture );
    }
    return rect;
}
void destroyText ( Text_t* pText )
{
    TTF_CloseFont ( pText->font );
    free ( pText );
}

void textInput ( SDL_Event event, char buffer[] )
{
    const char* keyName = SDL_GetKeyName ( event.key.keysym.sym );
    int bufferlen = strlen ( buffer );
    if (bufferlen < 3 && strlen ( keyName ) == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z')
    {
        strcat ( buffer, keyName );
    }
    if (strcmp ( keyName, "Backspace" ) == 0 && bufferlen > 0)
    {
        int len = strlen ( buffer );
        buffer[len - 1] = '\0';
    }
}

