#include "doomnukem.h"
#include "inputhelp.h"

//	Good resource for remembering bitwise operations:
//			https://stackoverflow.com/a/47990/1725220
//	TODO: move playermode events to separate file
/*place holder for inits*/
t_mat4x4 Init();
t_triangle Inittri();
void printf_tri(t_triangle t);
t_mat4x4 Matrix_MakeIdentity();
t_mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
void	move(t_game *game);

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
		if (iskey(e, SDLK_SPACE))
		{
			game->cam_mode = !game->cam_mode; //Flips from 0->1 and 1->0
			if (game->cam_mode == overhead_absolute)
			{
				game->overheadcam_pos = vector2_negative(game->player.position);
				//game->overheadcam_pos.x += WINDOW_W / 2;
				//game->overheadcam_pos.y += WINDOW_H / 2;
			}
		}
	}
	else if(e.type == SDL_KEYUP)
	{
		game->keystate &= ~(keyismoveleft(e));
		game->keystate &= ~(keyismoveright(e) << KEYS_RIGHTMASK);
		game->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		game->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
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
	t_mat4x4 matproj = Init();
	t_mat4x4 matworld = Init();

	matworld = Matrix_MakeIdentity();
	matproj = Matrix_MakeProjection(90.0f, (float)WINDOW_H / (float)WINDOW_W, 2.0f, 1000.0f);
	alloc_image(&pgraph.image, PERFGRAPH_SAMPLES + 1, PERFGRAPH_SAMPLES + 1);
	while (1)
	{
		update_deltatime(&game.clock);
		bzero(sdl.surface->pixels, sizeof(uint32_t) * WINDOW_H * WINDOW_W);
		gr = handleinput(&game);
		if (gr != game_continue)
			return(gr);
		//if (game.cam_mode == overhead_follow)
		moveplayer(&game);
		//else
		//	move_overhead(&game);
		move(&game);
		engine3d(sdl, game.math.triangles, game.math.tri_count, matproj, matworld, game.math.vcamera, game.math.vlookdir);  // tri count, matproj
		//drawperfgraph(&pgraph, game.clock.delta, &sdl);
		//render_overhead(&game, &sdl);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	return(game_exit); // for now
}

static int start_gameloop(t_sdlcontext sdl, t_game game)
{
	t_math	*math;
	int		gr;

	math = &game.math;
	bzero(&game.player, sizeof(t_player));
	math->vcamera = (t_vector3){43.471909, 69.871468, -47.650238}; // change these to v3init
	math->vlookdir = (t_vector3){1, 0, 0}; // 
	math->fyaw = -1.824000f;
	math->fpitch = -0.204000f;
	math->matproj = Init();

	return(gameloop(sdl, game));
}

static t_triangle set_tri(int *p1, int *p2, int *p3)
{
	return((t_triangle){
		(t_quaternion){p1[X], p1[Y], p1[Z], 1.0f},
		(t_quaternion){p2[X], p2[Y], p2[Z], 1.0f},
		(t_quaternion){p3[X], p3[Y], p3[Z], 1.0f}
		});
}

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
	//	printf_tri(math->triangles[i / 2]);
	//	printf_tri(math->triangles[(i / 2) + 1]);
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
	loadmap(&game.linelst, "mapfile1");
	lines_to_obj(&obj, game.linelst);
	set_tri_array(&game.math, &obj);
	game.player.position.x = 0.0f; //TODO: player position should be in game coordinates, not screenspace
	game.player.position.y = 0.0f;
	//Locks mouse
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
		error_log(EC_SDL_SETRELATIVEMOUSEMODE);
	//Do game loop until exit or error
	//gr = startgameloop(sdl, game);
	//gr = start_gameloop(sdl, game);
	gr = gameloop(sdl, game);
	if (gr == game_exit)
	{
		quit_sdl(&sdl);
		exit (0);
	}
	//Unlocks mouse
	if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0)
		error_log(EC_SDL_SETRELATIVEMOUSEMODE);
	return (gr);
}

/*
lines_to_obj(&obj, game.linelst);
	set_tri_array(&game.math, &obj);
	game.player.position[X] = 30.0f * TILESIZE;
	game.player.position[Y] = 30.0f * TILESIZE;
	//Locks mouse
	SDL_SetRelativeMouseMode(SDL_TRUE);		
	//Do game loop until exit or error
	gr = start_gameloop(sdl, game);
	//Unlocks mouse
	SDL_SetRelativeMouseMode(SDL_FALSE);*/