#include "doomnukem.h"
#include "inputhelp.h"
#include "file_io.h"
#include "objects.h"
//	Good resource for remembering bitwise operations:
//			https://stackoverflow.com/a/47990/1725220
//	TODO: move playermode events to separate file
/*place holder for inits*/
void	move(t_game *game);

static int key_events(SDL_Event e, t_game *game)
{
	t_vector2 temp;
	if (e.type == SDL_KEYDOWN)
	{
		if (iskey(e, SDLK_ESCAPE))
			return (game_exit);
		if (iskey(e, SDLK_TAB))
			return (game_switchmode);
		game->keystate |= keyismoveleft(e) << KEYS_LEFTMASK;
		game->keystate |= keyismoveright(e) << KEYS_RIGHTMASK;
		game->keystate |= keyismoveup(e) << KEYS_UPMASK;
		game->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
		game->keystate |= iskey(e, SDLK_SPACE) << KEYS_SPACEMASK;
		game->keystate |= iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK;
		game->keystate |= iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK;
		game->keystate |= iskey(e, SDLK_m) << KEYS_MMASK;
		if (iskey(e, SDLK_m))
		{
			game->cam_mode = !game->cam_mode;
		}
	}
	else if(e.type == SDL_KEYUP)
	{
		game->keystate &= ~(keyismoveleft(e)) << KEYS_LEFTMASK;
		game->keystate &= ~(keyismoveright(e) << KEYS_RIGHTMASK);
		game->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		game->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
		game->keystate &= ~(iskey(e, SDLK_SPACE) << KEYS_SPACEMASK);
		game->keystate &= ~(iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK);
		game->keystate &= ~(iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK);
		game->keystate &= ~(iskey(e, SDLK_m) << KEYS_MMASK);
	}
	return (game_continue);
}

//	Updates mouse delta position
static void updatemouse(t_mouse *mouse)
{
	SDL_GetRelativeMouseState(&mouse->delta.x, &mouse->delta.y);
	if (mouse->delta.x > 200 || mouse->delta.y > 200)
		mouse->delta = point_zero();
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

static void update_render(t_render *render, t_player player)
{
	render->lookdir = player.lookdir;
	render->position = player.position;
}

/*main game loop*/
static int gameloop(t_sdlcontext sdl, t_game game)
{
	t_gamereturn	gr;
	t_render		render;

	//alloc_image(&pgraph.image, PERFGRAPH_SAMPLES + 1, PERFGRAPH_SAMPLES + 1);
	gr = game_continue;
	render = init_render(sdl);
	game.world = load_world("world1", sdl);
	game.player.position = (t_vector3) {500.0f, 500.0f, 500.0f};
	game.player.angle = (t_vector2){-RAD90, -RAD90 * 0.99f};
	while (gr == game_continue)
	{
		update_deltatime(&game.clock);
		gr = handleinput(&game);
		moveplayer(&game);
		//printf_vec(game.player.lookdir);
		///printf_vec(game.player.position);
		//game.player.lookdir = (t_vector3){-0.391241, 0.904104, -0.171831};
		//game.player.position = (t_vector3){869.611389, 73.996613, 50.339905};
		update_render(&render, game.player);
		screen_blank(sdl); //Combine with render_start?
		render_start(&render);
		render_world3d(sdl, game.world, &render);
		draw_text_boxed(&sdl, "PLAYMODE", (t_point){5, 5}, (t_point){sdl.window_w, sdl.window_h});
		//DRAWPERFGRAPH
		char *fps = ft_itoa(game.clock.delta);
		draw_text_boxed(&sdl, fps, (t_point){sdl.window_w - 80, 10}, (t_point){sdl.window_w, sdl.window_h});
		free(fps);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
		//gr = game_switchmode;
	}
	free_render(render);
	if (gr == game_exit)
		quit_game(&sdl);
	return(gr); // for now
}

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_gamereturn	gr;

	bzero(&game, sizeof(t_game));
	//Locks mouse
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
		error_log(EC_SDL_SETRELATIVEMOUSEMODE);
	//Do game loop until exit or error
	gr = gameloop(sdl, game);
	//Unlocks mouse
	if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0)
		error_log(EC_SDL_SETRELATIVEMOUSEMODE);
	return (gr);
}
