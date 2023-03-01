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

t_point screenlerp(float xlerp, float ylerp, t_sdlcontext *sdl)
{
	return ((t_point){sdl->screensize.x * xlerp, sdl->screensize.y * ylerp});
}

static void draw_gun_hud(t_world *world)
{
	t_point		pos_ammo;
	t_rectangle	rect_ammo;
	//world->player.gun.
	//world->sdl->font.text
	pos_ammo = (t_point){20, world->sdl->screensize.y - 40};
	rect_ammo.position = point_sub(pos_ammo, (t_point){3,2});
	rect_ammo.size = (t_point) {50, 28};
	draw_rectangle_filled(*world->sdl, rect_ammo, 0);
	drawrectangle(*world->sdl, rect_ammo, AMBER_1);
	world->sdl->font.color = color32_to_sdlcolor(AMBER_2);
	print_text(world->sdl, "Ammo:", pos_ammo);
	pos_ammo.y += 14;
	if (world->player->gun->reload_anim.active)
		print_text(world->sdl, "R", pos_ammo);
	else
		print_text(world->sdl, s_itoa(world->player->gun->bullets), pos_ammo);
	pos_ammo.x += 20;
	print_text(world->sdl, "/", pos_ammo);
	pos_ammo.x += 10;
	uint8_t ammocount = world->player->ammo_union.ammo_arr[world->player->gun->stats.ammomask];
	print_text(world->sdl, s_itoa(ammocount), pos_ammo);
	world->sdl->font.font = world->sdl->font.font_sizes[0];

	t_point pos_reload;
	t_rectangle rect_reload;

	pos_reload = screenlerp(0.75f, 0.75f, world->sdl);
	if (world->player->input.aim)
		pos_reload = screenlerp(0.55f, 0.5f, world->sdl);
	if (world->player->gun->bullets == 0)
	{
		if (world->player->input.shoot && sin(world->clock.prev_time * 0.0185f) < 0.0f)
			world->sdl->font.color = color32_to_sdlcolor(CLR_RED);
		print_text_boxed(world->sdl, "![R]eload", pos_reload);
		world->sdl->font.color = color32_to_sdlcolor(AMBER_2);
	}
		
		
		

	//Draw player hud
	t_rectangle rect_health;
	rect_health.size = (t_point){100, 15};
	rect_health.position = point_add(rect_ammo.position, (t_point) {60, rect_ammo.size.y - rect_health.size.y});
	draw_rectangle_filled(*world->sdl, rect_health, 0);
	drawrectangle(*world->sdl, rect_health, AMBER_1);
	rect_health.size.x = ft_max(0, ((float)world->player->health / (float)MAXHEALTH) * 100);
	draw_rectangle_filled(*world->sdl, rect_health, CLR_RED);
}
#include "input.h"
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
	while (gr == game_continue)
	{
		update_deltatime(&game.world.clock);
		gr = playmode_events(&game);
		moveplayer(&game.player, &game.hid.input, &game.world);
		update_render(&sdl.render, &game.player);
		screen_blank(sdl);
		render_start(&sdl.render);
		update_world3d(&game.world, &sdl.render);
		char *fps = ft_itoa(game.world.clock.fps);
		print_text(&sdl, fps, (t_point){sdl.window_w - 80, 10});
		if (!game.player.input.aim)
			drawcircle(sdl, point_div(sdl.screensize, 2), 4, CLR_BLUE);
		free(fps);
		draw_gun_hud(&game.world);
		game.hid.mouse.click_unhandled = false;
		memcpy(sdl.window_surface->pixels, sdl.surface->pixels, sizeof(uint32_t) * sdl.window_w * sdl.window_h);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			doomlog(LOGEC_SDL_UPDATEWINDOWSURFACE, NULL);
		update_audio(&game.world);
		game_random(&game.world);
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

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_gamereturn	gr;

	bzero(&game, sizeof(t_game));
	//Locks mouse
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
		doomlog(LOGEC_SDL_SETRELATIVEMOUSEMODE, NULL);
	game.world = load_world_args("leveltest_bu", &sdl, LOAD_ARG_FULL);
	game.world.gamemode = MODE_PLAY;
	create_navmesh(&game.world);
	player_init(&game.player, &sdl, &game.world);
	game.player.transform.position = find_playerspawn(&game.world);
	gr = gameloop(sdl, game);
	//Unlocks mouse
	if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0)
		doomlog(LOGEC_SDL_SETRELATIVEMOUSEMODE, NULL);
	return (gr);
}
