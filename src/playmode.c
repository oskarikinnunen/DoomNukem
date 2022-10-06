#include "doomnukem.h"

/* testing renders */
void sage_render(t_sdlcontext sdl, t_game *game);
void init(t_player *player, t_game *game);

/*action up keys here*/
static int key_up(SDL_Event e, t_player *player)
{
	printf("key up\n");
	int val = 0;
	if (iskey(e, SDLK_w))
		player->wasd[0] = val;
	if (iskey(e, SDLK_a))
		player->wasd[1] = val;
	if (iskey(e, SDLK_s))
		player->wasd[2] = val;
	if (iskey(e, SDLK_d))
		player->wasd[3] = val;
	if (iskey(e, SDLK_LEFT))
		player->sl = val;
	if (iskey(e, SDLK_RIGHT))
		player->sr = val;
	if (iskey(e, SDLK_m))
		player->m = val;
	return(game_continue);
}

/*action down keys here*/
static int key_down(SDL_Event e, t_player *player)
{
	printf("key down %d\n", player->wasd[0]);
	if (iskey(e, SDLK_ESCAPE))
		return(game_exit);
	if (iskey(e, SDLK_TAB))
		return(game_switchmode);
	int val = 1;
	if (iskey(e, SDLK_w))
		player->wasd[0] = val;
	if (iskey(e, SDLK_a))
		player->wasd[1] = val;
	if (iskey(e, SDLK_s))
		player->wasd[2] = val;
	if (iskey(e, SDLK_d))
		player->wasd[3] = val;
	if (iskey(e, SDLK_LEFT))
		player->sl = val;
	if (iskey(e, SDLK_RIGHT))
		player->sr = val;
	if (iskey(e, SDLK_m))
		player->m = val;
	if (iskey(e, SDLK_RETURN))
		player->slowspeed = 1;
	return(game_continue);
}

/*check for keyboard/mouse input*/
static int game_events(t_game *game)
{
	static SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
			return(key_down(e, game->player));
		if (e.type == SDL_KEYUP)
			return(key_up(e, game->player));
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
		sage_render(sdl, game);
		SDL_UpdateWindowSurface(sdl.window);
	}
	return(game_exit); // for now
}

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_gamereturn	gr;
	t_player		player;

	bzero(&game, sizeof(t_game));
	bzero(&player, sizeof(t_player));
	game.player = &player;
	loadmap(&game.linelst, "mapfile1");
	init(&player, &game);
	gr = gameloop(sdl, &game);
	free_lst(game.linelst);
	return(gr);
}
/*
array of triangles
triangle is a size 3 array of points
points is 3 int;*/