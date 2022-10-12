#include "doomnukem.h"
#include "inputhelp.h"

/*place holder for inits*/
t_mat4x4 Init();
t_vec3d	Initv3();
t_triangle Inittri();

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
		game->keystate |= keyismoveright(e) << KEYS_RIGHTMASK;
		game->keystate |= keyismoveup(e) << KEYS_UPMASK;
		game->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
		game->keystate |= keyarrowright(e) << KEYS_ARROWRIGHT;
		game->keystate |= keyarrowdown(e) << KEYS_ARROWDOWN;
		game->keystate |= keyarrowleft(e) << KEYS_ARROWLEFT;
		game->keystate |= keyarrowup(e) << KEYS_ARROWUP;
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
		game->keystate &= ~(keyismoveright(e) << KEYS_RIGHTMASK);
		game->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		game->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
		game->keystate &= ~(keyarrowright(e) << KEYS_ARROWRIGHT);
		game->keystate &= ~(keyarrowleft(e) << KEYS_ARROWLEFT);
		game->keystate &= ~(keyarrowdown(e) << KEYS_ARROWDOWN);
		game->keystate &= ~(keyarrowup(e) << KEYS_ARROWUP);
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
static int gameloop(t_sdlcontext sdl, t_game *game)
{
	t_gamereturn	gr;

	while (1)
	{
		update_deltatime(&game->clock);
		bzero(sdl.surface->pixels, sizeof(uint32_t) * WINDOW_H * WINDOW_W);
		gr = handleinput(game);
		/*
		if (gr != game_continue)
			return(gr);
		render_overhead(game, &sdl);*/
		if (gr != game_continue)
			return(gr);
		if (game->cam_mode == overhead_follow)
			moveplayer(game);
		else
			move_overhead(game);
		engine3d(sdl, game);
		SDL_UpdateWindowSurface(sdl.window);
	}
	return(game_exit); // for now
}

static int start_gameloop(t_sdlcontext sdl, t_game game)
{
	t_math	*math;
	int		gr;

	math = &game.math;
	bzero(&game.player, sizeof(t_player));
	math->vcamera = (t_vec3d){0, 0, 0, 1}; // change these to v3init
	math->vlookdir = (t_vec3d){0, 0, 0, 1}; // 
	math->matproj = Init();

	return(gameloop(sdl, &game));
}

static t_triangle set_tri(int *p1, int *p2, int *p3)
{
	return((t_triangle){
		(t_vec3d){p1[X], p1[Y], p1[Z], 1},
		(t_vec3d){p2[X], p2[Y], p2[Z], 1},
		(t_vec3d){p3[X], p3[Y], p3[Z], 1}
		});
}

void printf_tri(t_triangle tri);

static void set_tri_array(t_math *math, t_obj *obj)
{
	int		i;
	int		len;
	int32_t	**verts;

	i = 0;
	verts = obj->verts;
	len = obj->v_count; //4vert = 2 triangles
	math->triangles = malloc(sizeof(t_triangle) * (len / 2));
	math->tri_count = len / 2;
	while (i < len)
	{
		math->triangles[i / 2] = set_tri(verts[i], verts[i + 1], verts[i + 2]);
		math->triangles[(i / 2) + 1] = set_tri(verts[i + 2], verts[i + 1], verts[i + 3]);
		printf_tri(math->triangles[i / 2]);
		printf_tri(math->triangles[(i / 2) + 1]);
		i += 4;
	}
	//exit(0);
}

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_obj			obj;
	t_gamereturn	gr;

	bzero(&game, sizeof(t_game));
	bzero(&obj, sizeof(t_obj));
	loadmap(&game.linelst, "mapfile1");
	lines_to_obj(&obj, game.linelst);
	set_tri_array(&game.math, &obj);
	game.player.position[X] = 30.0f * TILESIZE;
	game.player.position[Y] = 30.0f * TILESIZE;
	//Locks mouse
	SDL_SetRelativeMouseMode(SDL_TRUE);		
	//Do game loop until exit or error
	gr = start_gameloop(sdl, game);
	//Unlocks mouse
	SDL_SetRelativeMouseMode(SDL_FALSE);
	return (gr);
}