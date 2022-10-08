#include "doomnukem.h"

/* testing 2drenders */
void sage_render(t_sdlcontext sdl, t_game *game);
void init(t_player *player, t_game *game);

/*testing 3drenders*/
void engine3d(t_sdlcontext sdl, t_game *game);
t_mat4x4	Init();
t_vec3d		Initv3();
t_triangle Inittri();

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
	if (iskey(e, SDLK_UP))
		player->arrow[0] = val;
	if (iskey(e, SDLK_LEFT))
		player->arrow[1] = val;
	if (iskey(e, SDLK_DOWN))
		player->arrow[2] = val;
	if (iskey(e, SDLK_RIGHT))
		player->arrow[3] = val;
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
	if (iskey(e, SDLK_UP))
		player->arrow[0] = val;
	if (iskey(e, SDLK_LEFT))
		player->arrow[1] = val;
	if (iskey(e, SDLK_DOWN))
		player->arrow[2] = val;
	if (iskey(e, SDLK_RIGHT))
		player->arrow[3] = val;
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
		update_deltatime(&game->clock);
		bzero(sdl.surface->pixels, sizeof(uint32_t) * WINDOW_H * WINDOW_W);
		gr = game_events(game);
		if (gr != game_continue)
			return(gr);
		engine3d(sdl, game);
		SDL_UpdateWindowSurface(sdl.window);
	}
	return(game_exit); // for now
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

static void set_tri_array(t_game *game)
{
	int		i;
	int		len;
	int32_t	**verts;

	i = 0;
	verts = game->obj->verts;
	len = game->obj->v_count; //4vert = 2 triangles
	game->triangles = malloc(sizeof(t_triangle) * (len / 2));
	game->tri_count = len / 2;
	while (i < len)
	{
		game->triangles[i / 2] = set_tri(verts[i], verts[i + 1], verts[i + 2]);
		game->triangles[(i / 2) + 1] = set_tri(verts[i + 1], verts[i + 2], verts[i + 3]);
		printf_tri(game->triangles[i / 2]);
		printf_tri(game->triangles[(i / 2) + 1]);
		i += 4;
	}
	//exit(0);
}

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_gamereturn	gr;
	t_player		player;
	t_obj			obj;
	t_vec3d			vcamera;
	t_vec3d			vlookdir;
	t_mat4x4		matproj;

	bzero(&game, sizeof(t_game));
	bzero(&player, sizeof(t_player));
	bzero(&obj, sizeof(t_obj));
	vcamera = (t_vec3d){0, 0, 0, 1};
	vlookdir = (t_vec3d){0, 0, 0, 1};
	matproj = Init();

	game.player = &player;
	game.obj = &obj;
	game.vcamera = &vcamera;
	game.vlookdir = &vlookdir;
	game.matproj = &matproj;

	loadmap(&game.linelst, "mapfile1");
	lines_to_obj(&obj, game.linelst);
	set_tri_array(&game);
	
	gr = gameloop(sdl, &game);
	
	free_lst(game.linelst);
	return(gr);
}
/*
array of triangles
triangle is a size 3 array of points
points is 3 int;*/