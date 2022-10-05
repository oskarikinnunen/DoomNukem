#include "doomnukem.h"

void playmode(t_sdlcontext sdl)
{
	t_game	game;

	bzero(&game, sizeof(t_game));
	loadmap(&game.linelst, "mapfile1");

	
}