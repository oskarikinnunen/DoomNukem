#include "doomnukem.h"
#include "inputhelp.h"

//	Good resource for remembering bitwise operations:
//			https://stackoverflow.com/a/47990/1725220
//	TODO: move playermode events to separate file
static int key_events(SDL_Event e, t_game *game)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (iskey(e, SDLK_ESCAPE))
			return (game_exit);
		if (iskey(e, SDLK_TAB))
			return (game_switchmode);
		game->keystate |= keyismoveleft(e);
		game->keystate |= keyismoveright(e) << KEYS_RGHTMASK;
		game->keystate |= keyismoveup(e) << KEYS_UPMASK;
		game->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
		if (iskey(e, SDLK_SPACE))
		{
			game->cam_mode = !game->cam_mode; //Flips from 0->1 and 1->0
			if (game->cam_mode == overhead_absolute)
			{
				game->overheadcam_pos[X] = -game->player.position[X];
				game->overheadcam_pos[Y] = -game->player.position[Y];
				game->overheadcam_pos[X] += WINDOW_W / 2;
				game->overheadcam_pos[Y] += WINDOW_H / 2;
			}
		}
	}
	else if(e.type == SDL_KEYUP)
	{
		game->keystate &= ~(keyismoveleft(e));
		game->keystate &= ~(keyismoveright(e) << KEYS_RGHTMASK);
		game->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		game->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
	}
	return (game_continue);
}

//	Updates mouse delta position
static void updatemouse(t_mouse *mouse)
{
	SDL_GetRelativeMouseState(&mouse->p_delta[X], &mouse->p_delta[Y]);
}

/*check for keyboard/mouse input*/
static int handleinput(t_game *game)
{
	static SDL_Event	e;
	t_gamereturn		gr;

	updatemouse(&game->mouse);
	while (SDL_PollEvent(&e))
	{
		gr = key_events(e, game);
		if (gr != game_continue)
			return (gr);
	}
	return(game_continue);
}

/*main game loop*/
static int gameloop(t_sdlcontext sdl, t_game game)
{
	t_gamereturn	gr;

	while (1)
	{
		update_deltatime(&game.clock);
		bzero(sdl.surface->pixels, sizeof(uint32_t) * WINDOW_H * WINDOW_W);
		gr = handleinput(&game);
		if (gr != game_continue)
			return(gr);
		if (game.cam_mode == overhead_follow)
			moveplayer(&game);
		else
			move_overhead(&game);
		render_overhead(&game, &sdl);
		SDL_UpdateWindowSurface(sdl.window);
	}
	return(game_exit); // for now
}

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_gamereturn	gr;

	bzero(&game, sizeof(t_game));
	loadmap(&game.linelst, "mapfile1");
	game.player.position[X] = 30.0f * TILESIZE; //TODO: player position should be in game coordinates, not screenspace
	game.player.position[Y] = 30.0f * TILESIZE;
	//Locks mouse
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//Do game loop until exit or error
	gr = gameloop(sdl, game);
	//Unlocks mouse
	SDL_SetRelativeMouseMode(SDL_FALSE);
	return (gr);
}