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

#include "input.h"
static void debug_ramps(t_game *game)
{
	t_list			*l;
	t_vector3_tri	*tri;
	int				i;

	l = game->world.ramps;
	i = 0;
	while (l != NULL)
	{
		tri = (t_vector3_tri *)l->content;
		if (i % 2 == 0)
			render_ray3D(game->world.sdl, tri->c, tri->a, CLR_RED);
		else
			render_ray3D(game->world.sdl, tri->b, tri->c, CLR_BLUE);
		i++;
		l = l->next;
	}
}
/*main game loop*/
static int gameloop(t_sdlcontext sdl, t_game game)
{
	t_gamereturn	gr;
	bool		audio = 0;

	gr = game_continue;
	sdl.lighting_toggled = false;
	game.world.lighting.calculated = false;
	sdl.render.occlusion.occlusion = false;
	game.world.sdl = &sdl;
	sdl.audio.sfx_volume = 1.0f;
	game.world.player = &game.player;
	protagonist_play_audio(&game.player, &game.world, "protag_letsdo.wav");
	play_music(&sdl, "music_arp1_action.wav");
	while (gr == game_continue)
	{
		if (game.player.health > 0)
		{
			update_deltatime(&game.world.clock);
			gr = playmode_events(&game);
			moveplayer(&game.player, &game.hid.input, &game.world);
			update_world3d(&game.world, &sdl.render);
			//FPS COUNTER
			print_text(&sdl, s_itoa(game.world.clock.fps), (t_point){sdl.window_w - 80, 10});
			debug_ramps(&game);
			draw_player_hud(&game.world);
			memcpy(sdl.window_surface->pixels, sdl.surface->pixels, sizeof(uint32_t) * sdl.window_w * sdl.window_h);
			if (SDL_UpdateWindowSurface(sdl.window) < 0)
				doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
			update_audio(&game.world);
			game_random(&game.world);
		}
		else
		{
			playmode_death(&game);
		}
	}
	if (gr == game_exit)
	{
		free_render(sdl.render);
		quit_game(&sdl);
	}
		
	return (gr);
}

static t_vector3 find_playerspawn(t_world *world)
{
	int				i;
	t_entitycache	*cache;
	t_entity		*ent;
	t_vector3		result;

	cache = &world->entitycache;
	result = vector3_zero();
	i = 0;
	while (i < cache->alloc_count) 
	{
		ent = &cache->entities[i];
		if (ent->status == es_active && ent->component.type == COMP_PLAYERSPAWN)
		{
			result = ent->transform.position;
			result.z -= 10.0f;
			destroy_entity(world, ent);
		}
		i++;
	}
	return (result);
}

//TODO: this can use entity->worldvertices once that is in the main branch, for now this'll use this
//Defined in editor_raycast.c
t_vector3_tri	worldspace_tri(t_entity *entity, int index);

static void world_add_ramp(t_world *world, t_entity *entity)
{
	int				i;
	t_vector3_tri	tri;

	i = 0;
	while (i < entity->obj->face_count)
	{
		if (i == 4 || i == 7)
		{
			tri = worldspace_tri(entity, i);
			list_push(&world->ramps, &tri, sizeof(tri));
		}
		i++;
	}
}

//Find Ramp colliders?
static void ramps_create(t_world *world)
{
	t_entitycache	*cache;
	t_object		*ramp;

	int				i;
	//Go through all the objects, if the name has "_ramp", then go through the triangles
	cache = &world->entitycache;
	i = 0;
	ramp = get_object_by_name(*world->sdl, "concrete_ramp.obj");
	while (i < cache->alloc_count)
	{
		if (cache->entities[i].obj == ramp)
		{
			world_add_ramp(world, &cache->entities[i]);
		}
		i++;
	}
}
//Resetlevel??
/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_gamereturn	gr;

	bzero(&game, sizeof(t_game));
	//Locks mouse
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
		doomlog(LOG_EC_SDL_SETRELATIVEMOUSEMODE, NULL);
	game.world = load_world_args("leveltest_bu", &sdl, LOAD_ARG_FULL);
	game.world.gamemode = MODE_PLAY;
	create_navmesh(&game.world);
	ramps_create(&game.world);
	player_init(&game.player, &sdl, &game.world);
	game.player.transform.position = find_playerspawn(&game.world);
	sdl.fog = true;
	gr = gameloop(sdl, game);
	//Unlocks mouse
	if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0)
		doomlog(LOG_EC_SDL_SETRELATIVEMOUSEMODE, NULL);
	return (gr);
}
