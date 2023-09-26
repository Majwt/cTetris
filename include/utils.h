#ifndef utils_h
#define utils_h

extern const SDL_Color White;
extern const SDL_Color Black;
extern TTF_Font* font;

void test(const char* format, ...);
int InitFont(const char* path_to_font);
void TextInput(SDL_Event event, char buffer[]);
SDL_Rect ShowText(SDL_Renderer *pRenderer, SDL_Color color, int fontsize, SDL_Rect rect, bool centered,char* text, ...);
bool PointRectCollision(int x, int y, SDL_Rect rect);


#endif