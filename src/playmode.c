#include "doomnukem.h"

/*action up keys here*/
static int key_up(SDL_Event g)
{
	printf("key up\n");
	return(game_continue);
}

/*action down keys here*/
static int key_down(SDL_Event e)
{
	printf("key down\n");
	if (iskey(e, SDLK_ESCAPE))
		return(game_exit);
	if (iskey(e, SDLK_TAB))
		return(game_switchmode);
	return(game_continue);
}

/*check for keyboard/mouse input*/
static int game_events(t_game *game)
{
	static SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
			return(key_down(e));
		if (e.type == SDL_KEYUP)
			return(key_up(e));
	}
	return(game_continue);
}

/*main game loop*/
static int gameloop(t_sdlcontext sdl, t_game *game)
{
	t_gamereturn	gr;

	while (1)
	{
		bzero(sdl.surface->pixels, sizeof(uint32_t) * WINDOW_H * WINDOW_W);
		gr = game_events(game);
		if (gr != game_continue)
			return(gr);
		SDL_UpdateWindowSurface(sdl.window);
	}
	return(game_exit); // for now
}

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game	game;

	bzero(&game, sizeof(t_game));
	loadmap(&game.linelst, "mapfile1");
	return(gameloop(sdl, &game));
}