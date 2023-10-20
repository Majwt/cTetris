

#include "game.h"


int main(int argv, char** args)
{
	srand(time(NULL));
	rand();
	Game_t g = { 0 };
	if (!initGame(&g))
		return 1;
	while (g.state != QUIT)
	{
		if (g.state == PLAY)
		{
			initBoard(&g);
			runGame(&g);
		}
		else if (g.state == GAMEOVER)
		{
			gameOverView(&g);
		}
		else if (g.state == MENU)
		{
			mainMenuView(&g);
		}
	}

	closeGame(&g);

	return 0;
}

