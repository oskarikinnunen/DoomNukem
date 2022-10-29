#include "doomnukem.h"
#include "inputhelp.h"
#include "file_io.h"
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
	t_perfgraph		pgraph;
	uint32_t		*pixels = malloc(sizeof(uint32_t) * sdl.window_h * sdl.window_w);

	alloc_image(&pgraph.image, PERFGRAPH_SAMPLES + 1, PERFGRAPH_SAMPLES + 1);
	gr = game_continue;
	while (gr == game_continue)
	{
		update_deltatime(&game.clock);
		bzero(sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		bzero(sdl.zbuffer, sizeof(float) * sdl.window_h * sdl.window_w);
		gr = handleinput(&game);
		moveplayer(&game);
		if (game.cam_mode == player_view)
			engine3d(sdl, game, pixels);
		else
			render_overhead(&game, sdl);
			//bzero(sdl.pxls, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
//		for (int i = 0; i < sdl.window_h * sdl.window_w; i++)
	//		if (pixels[i] != 0)
		//		printf("first index %d\n", (pixels)[i]);
		//sdl.surface->pixels = (uint32_t *)pixels;
		//ft_memcpy(((uint32_t *)sdl.surface->pixels), pixels, sizeof(float) * sdl.window_h * sdl.window_w);
		drawperfgraph(&pgraph, game.clock.delta, sdl);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	if (gr == game_exit)
		quit_game(&sdl);
	return(gr); // for now
}

static t_triangle set_tri(int *p1, int *p2, int *p3)
{
	return((t_triangle){
		(t_quaternion){p1[X], p1[Y], p1[Z], 1},
		(t_quaternion){p2[X], p2[Y], p2[Z], 1},
		(t_quaternion){p3[X], p3[Y], p3[Z], 1}
		});
}

static void set_tex1(t_triangle *tri)
{
	tri->t[0].u = 0.0f;
	tri->t[0].v = 0.0f;

	tri->t[1].u = 1.0f;
	tri->t[1].v = 0.0f;

	tri->t[2].u = 0.0f;
	tri->t[2].v = 1.0f;

	tri->t[0].w = 1.0f;
	tri->t[1].w = 1.0f;
	tri->t[2].w = 1.0f;
}

static void set_tex2(t_triangle *tri)
{
	tri->t[0].u = 0.0f;
	tri->t[0].v = 1.0f;

	tri->t[1].u = 1.0f;
	tri->t[1].v = 0.0f;

	tri->t[2].u = 1.0f;
	tri->t[2].v = 1.0f;

	tri->t[0].w = 1.0f;
	tri->t[1].w = 1.0f;
	tri->t[2].w = 1.0f;
}

static void set_tri_array(t_game *game, t_obj *obj)
{
	int		i;
	int		len;
	int32_t	**verts;

	i = 0;
	verts = obj->verts;
	len = obj->v_count;
	game->triangles = malloc(sizeof(t_triangle) * (len / 2));
	game->tri_count = len / 2;
	while (i < len)
	{
		game->triangles[i / 2] = set_tri(verts[i], verts[i + 1], verts[i + 2]);
		game->triangles[(i / 2) + 1] = set_tri(verts[i + 2], verts[i + 1], verts[i + 3]);
		set_tex1(&game->triangles[i / 2]);
		set_tex2(&game->triangles[(i / 2) + 1]);
		i += 4;
	}
}

static void linefaces(t_obj *obj, uint32_t i)
{
	//TODO: Protect allocations plz
	int	fi;

	fi = i;
	if (i > 0)
		fi = i / 2;
	obj->faces[fi] = ft_memalloc(3 * sizeof (uint32_t));
	obj->faces[fi][0] = i;
	obj->faces[fi][1] = i + 1;
	obj->faces[fi][2] = i + 2;
	fi++;
	obj->faces[fi] = ft_memalloc(3 * sizeof (uint32_t));
	obj->faces[fi][0] = i + 1;
	obj->faces[fi][1] = i + 2;
	obj->faces[fi][2] = i + 3;
}

static void	i3_mul(int *i3, int mul)
{
	i3[X] = i3[X] * mul;
	i3[Y] = i3[Y] * mul;
	i3[Z] = i3[Z] * mul;
}

static void copyverts(t_obj *obj, t_line line, int i) //Ghetto, TODO: make cpyv2fromv3 function?
{
	obj->verts[i][X] = line.start.x;
	obj->verts[i][Y] = line.start.y;
	obj->verts[i + 1][X] = line.end.x;
	obj->verts[i + 1][Y] = line.end.y;
	obj->verts[i + 2][X] = line.start.x;
	obj->verts[i + 2][Y] = line.start.y;
	obj->verts[i + 3][X] = line.end.x;
	obj->verts[i + 3][Y] = line.end.y;
}

static void lines_to_obj(t_obj *obj, t_list *linelist)
{
	t_list		*l;
	int			len;
	uint32_t	i;

	l = linelist;
	len = ft_listlen(l);
	obj->v_count = len * 4;
	obj->verts = ft_memalloc(obj->v_count * sizeof(int32_t *));
	obj->f_count = len * 2;
	obj->faces = ft_memalloc(obj->f_count * sizeof(uint32_t *));
	i = 0;
	if (obj->mtlcolors)
		free(obj->mtlcolors);
	obj->mtlcolors = ft_memalloc(sizeof(uint32_t));
	obj->mtlcolors[0] = CLR_PRPL;
	while (l != NULL)
	{
		obj->verts[i] = ft_memalloc(3 * sizeof(int32_t *));
		obj->verts[i + 1] = ft_memalloc(3 * sizeof(int32_t *));
		obj->verts[i + 2] = ft_memalloc(3 * sizeof(int32_t *));
		obj->verts[i + 3] = ft_memalloc(3 * sizeof(int32_t *));
		copyverts(obj, *((t_line *)l->content), i);
		obj->verts[i + 2][Z] = 5; //HEIGHT OF THE WALL
		obj->verts[i + 3][Z] = 5;
		i3_mul(obj->verts[i], TILESIZE);
		i3_mul(obj->verts[i + 1], TILESIZE);
		i3_mul(obj->verts[i + 2], TILESIZE);
		i3_mul(obj->verts[i + 3], TILESIZE);
		linefaces(obj, i);
		l = l->next;
		i += 4;
	}
}

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_gamereturn	gr;
	t_obj			obj;

	bzero(&game, sizeof(t_game));
	bzero(&obj, sizeof(t_obj));
	sdl.zbuffer = malloc(sdl.window_w * sdl.window_h * sizeof(float));
	sdl.pxls = (uint32_t *)malloc(sdl.window_w * sdl.window_h * sizeof(uint32_t));
	game.linelist = load_chunk("map_test1", "WALL");
	game.entitylist = load_chunk("map_test1", "ENT_");
	if (game.entitylist != NULL) //player position is set from first entitys position, TODO: use entityID to determine which one is the player
	{
		game.player.position.x = (*(t_entity *)game.entitylist->content).position.x;
		game.player.position.y = (*(t_entity *)game.entitylist->content).position.y;
		game.player.angle.x = 0;
		game.player.angle.y = 0;
	}

	lines_to_obj(&obj, game.linelist);
	set_tri_array(&game, &obj);
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
