#include "doomnukem.h"
 
#include "file_io.h"
#include "objects.h"
//	Good resource for remembering bitwise operations:
//			https://stackoverflow.com/a/47990/1725220
//	TODO: move playermode events to separate file
/*place holder for inits*/
void	move(t_game *game);

static int key_events(SDL_Event e, t_hid_info *hid)
{
	t_vector2 temp;
	if (e.type == SDL_KEYDOWN)
	{
		if (iskey(e, SDLK_ESCAPE))
			return (game_exit);
		if (iskey(e, SDLK_TAB))
			return (game_switchmode);
		hid->keystate |= keyismoveleft(e) << KEYS_LEFTMASK;
		hid->keystate |= keyismoveright(e) << KEYS_RIGHTMASK;
		hid->keystate |= keyismoveup(e) << KEYS_UPMASK;
		hid->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
		hid->keystate |= iskey(e, SDLK_SPACE) << KEYS_SPACEMASK;
		hid->keystate |= iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK;
		hid->keystate |= iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK;
		hid->keystate |= iskey(e, SDLK_m) << KEYS_MMASK;
		/*if (iskey(e, SDLK_m))
		{
			game->cam_mode = !game->cam_mode;
		}*/
	}
	else if(e.type == SDL_KEYUP)
	{
		hid->keystate &= ~(keyismoveleft(e) << KEYS_LEFTMASK);
		hid->keystate &= ~(keyismoveright(e) << KEYS_RIGHTMASK);
		hid->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		hid->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
		hid->keystate &= ~(iskey(e, SDLK_SPACE) << KEYS_SPACEMASK);
		hid->keystate &= ~(iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK);
		hid->keystate &= ~(iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK);
		hid->keystate &= ~(iskey(e, SDLK_m) << KEYS_MMASK);
	}
	return (game_continue);
}

//	Updates mouse delta position
static void updatemouse(t_mouse *mouse)
{
	SDL_GetRelativeMouseState(&mouse->delta.x, &mouse->delta.y);
}

/*check for keyboard/mouse/joystick input*/
static int handleinput(t_hid_info	*hid)
{
	static SDL_Event	e;
	t_gamereturn		gr;

	updatemouse(&hid->mouse);
	while (SDL_PollEvent(&e))
	{
		mouse_event(e, &hid->mouse);
		gr = key_events(e, hid);
		if (gr != game_continue)
			return (gr);
		//gr = controller_events(e, game);
		if (gr != game_continue)
			return (gr);
	}
	updateinput(&hid->input, hid->keystate, hid->mouse, &hid->controller[0]);
	return(game_continue);
}

/*main game loop*/
static int gameloop(t_sdlcontext sdl, t_game game)
{
	t_gamereturn	gr;
	t_render		render;
	bool			sound = 1;

	//alloc_image(&pgraph.image, PERFGRAPH_SAMPLES + 1, PERFGRAPH_SAMPLES + 1);
	gr = game_continue;
	render = init_render(sdl, &game.world);
	game.world = load_world("world1", &sdl);
	*(game.world.debug_gui) = init_gui(&sdl, &game.hid, &game.player, sdl.screensize, "Debugging menu (F2)");
	game.world.debug_gui->rect.size.y = 120;
	game.world.debug_gui->hidden = true;
	game.world.debug_gui->rect.position.y = sdl.window_h / 2;
	game.world.debug_gui->rect.size.y = sdl.window_h / 2;
	player_init(&game.player, &sdl, &game.world);
	initialize_controllers(&game.hid);
	while (gr == game_continue)
	{
		update_deltatime(&game.clock);
		update_deltatime(&game.world.clock);
		gr = handleinput(&game.hid);

		if (sound)
		{
			sound = false;
			play_sound(&sdl.audio, "rock-music.wav");
		}

		moveplayer(&game.player, &game.hid.input, game.clock);
		update_render(&render, &game.player);
		screen_blank(sdl); //Combine with render_start?
		render_start(&render);
		update_world3d(&game.world, &render);
		print_text(&sdl, "PLAYMODE", (t_point){5, 5});
		/*game.player.gun->transform.position = vector3_add(game.player.position, (t_vector3){.z = -25.5f});
		game.player.gun->transform.rotation.x = game.player.angle.x + ft_degtorad(100.0f);*/
		render_entity(&sdl, &render, &game.player.gun->entity);
		//DRAWPERFGRAPH
		rescale_surface(&sdl);
		join_surfaces(sdl.window_surface, sdl.surface);
		join_surfaces(sdl.window_surface, sdl.ui_surface);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
		
		
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
