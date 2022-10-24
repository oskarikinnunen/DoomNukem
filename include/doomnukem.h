/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/24 15:59:52 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOMNUKEM_H
# define DOOMNUKEM_H

# include "../SDL_built/include/SDL2/SDL.h" //TODO: make this work without relative path?
# include "libft.h"
# include "error_log.h"
# include "limits.h" //TODO: remove mby? just define INT_MAX yourself
# include <math.h>
# include <fcntl.h>
# include "vectors.h"
# include <stdbool.h>

# define WINDOW_W 1920
# define WINDOW_H 1080
# define TILESIZE 32 //EDITOR tilesize
# define GRIDSIZE 64 //EDITOR gridsize (how many addressable coordinates we have)

# define PI	3.14159265359
# define FULLRAD PI * 2.0
# define RAD90		1.57079633f //90 degrees in radian

# define X 0
# define Y 1
# define Z 2

# define MOUSE_LEFT 1
# define MOUSE_MDL 2
# define MOUSE_RIGHT 3

# define CLR_PRPL 14231500
# define CLR_TURQ 5505010
# define CLR_GRAY 4868682
# define CLR_GREEN 3002977

#define DEBUG_ON 1

// Playmode defines
# define OVERHEADCAMSPEED 0.028f
# define PLAYERRADIUS 16
# define COLLISION_ON //Comment/uncomment to toggle experimental collision

# define MOVESPEED 10.010f
# define MAXMOVEMENTSPEED 0.08f
# define ROTATESPEED 0.002f
# define MOUSESPEED 0.0002f


typedef struct s_mouse
{
	t_point	pos;
	t_point	delta;
	int		scroll;
	int		scroll_delta;
	bool	click;
	int		held;
}	t_mouse;

typedef struct s_mousedrag
{
	t_point	pos;
	t_point	pos_origin;
	t_point	drag;
	t_point	drag_origin;
}	t_mousedrag;

typedef	struct s_line
{
	t_point	start;
	t_point	end;
}	t_line;

typedef struct s_wall
{
	t_line	line;
	int		height;
}	t_wall;

typedef enum e_editorstate
{
	place_start,
	place_end,
	display3d
}	t_editorstate;


typedef struct s_img
{
	t_point		size;
	char		name[128];
	uint32_t	*data;
	uint32_t	length;
}	t_img;

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
	SDL_Renderer			*renderer; //TODO: for testing remove.
	t_img					*images;
	uint					imagecount;
	float					*zbuffer;
}	t_sdlcontext;

# define PERFGRAPH_SAMPLES 64

typedef struct s_perfgraph
{
	t_img		image;
	uint32_t	deltas[PERFGRAPH_SAMPLES];
}	t_perfgraph;

typedef struct s_obj //TODO: move obj/fdf related stuff to separate header?
{
	char		**mtlnames;
	uint32_t	*mtlcolors;
	uint8_t		*colors; //Points to colors in mtlcolors
	int32_t		**verts;
	uint32_t	**faces;
	uint32_t	m_count;
	uint32_t	v_count;
	uint32_t	f_count;
}	t_obj;

//TODO: add ping-pong, repeat
typedef enum	e_anim_mode
{
	anim_forwards,
	anim_backwards
} t_anim_mode;

typedef struct s_clock
{
	Uint32	prev_time;
	Uint32	delta;
} t_clock;

typedef struct s_anim
{
	bool		active;
	uint32_t	time;
	int32_t		frame;
	int32_t		lastframe;
	uint8_t		framerate;
	float		lerp;
	t_anim_mode	mode;
}	t_anim;

typedef struct s_editor
{
	t_editorstate	state;
	t_line			line; //the line that is being edited right now
	t_list			*linelist;
	t_mousedrag		mousedrag[2]; //First one is right click drag, 2nd is for middle click
	t_mouse			mouse;
	float			threedee_zoom;
	t_anim			transition;
	t_clock			clock;
}	t_editor;

/* Playmode */

typedef struct s_player
{
	t_vector3	position; //TODO: might be changed to int[2], don't know yet
	t_vector2	angle;
	t_vector3	lookdir;
}	t_player;

typedef enum	e_cam_mode
{
	player_view,
	overhead_follow
}	t_cam_mode;

typedef enum e_gamereturn
{
	game_error,
	game_continue,
	game_exit,
	game_switchmode
} t_gamereturn;

typedef struct s_texture
{
	float	u;
	float	v;
	float	w;
} t_texture;

typedef struct	s_triangle
{
	t_quaternion	p[3];
	t_texture		t[3];
	uint32_t	clr;
}	t_triangle;

typedef struct s_game
{
	int				tri_count;
	t_triangle		*triangles;
	t_list			*linelst;
	t_clock			clock;
	t_mouse			mouse;
	t_player		player;
	int32_t			keystate;
	t_cam_mode		cam_mode;
	t_vector2		overheadcam_pos;
} t_game;

typedef struct s_zbuff
{
	int	w;
	int	*zbuff;
}	t_zbuff;

/* EDITOR.C */
int		editorloop(t_sdlcontext sdl);

/* EDITOR_EVENTS.C */
int		editor_events(t_editor *ed);
bool	iskey(SDL_Event e, int keycode);

/* EDITOR_RENDER.C */
void	renderlines(t_sdlcontext *sdl, t_editor *ed); //TODO:  better name?

/* EDITOR_MOUSE.C */
t_point	mousetoworldspace(t_editor *ed);
t_point	screentoworldspace(t_point point);
void	mouse_event(SDL_Event e, t_editor *ed);

/* EDITOR_SAVELINE.C */
void	saveline(t_editor *ed);

/* EDITOR_MAP_IO.C */
t_list	*loadmap(char *filename);
void	savemap(t_editor *ed, char *filename);

/* IMG.C */
void	alloc_image(t_img *img, int width, int height);
t_img	get_image_by_name(t_sdlcontext sdl, char *name);

/* INPUTHELPER.C */
bool	iskey(SDL_Event e, int keycode);

/* DELTATIME.C */
void	update_deltatime(t_clock *c);

/* ANIM.C */
void	update_anim(t_anim *anim, uint32_t delta);
void	start_anim(t_anim *anim, t_anim_mode mode);

/* DRAW.C */
void	draw(uint32_t *pxls, t_point pos, uint32_t clr);
void	drawline(uint32_t *pxls, t_point from, t_point to, uint32_t clr);
void	drawcircle(uint32_t *pxls, t_point pos, int size, uint32_t clr);

//Draws image 'img' to pixels 'pxls', offset by point 'pos' and scaled to 'scale'
void	draw_image(uint32_t *pxls, t_point pos, t_img img, int scale);

/* PERFGRAPH.C */
void	drawperfgraph(t_perfgraph *graph, uint32_t delta, t_sdlcontext *sdl);

/* PLAYMODE.C */
int		playmode(t_sdlcontext sdl);
void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle);
void	engine3d(t_sdlcontext sdl, t_game game);

/* PHYSICS.C */
bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r);
bool	linecirclecollision(t_line line, t_vector2 cp, float r);


/* PLAYMODE_OVERHEAD.C */
void	render_overhead(t_game *game, t_sdlcontext *sdl);
void	move_overhead(t_game *game);

/* MOVEPLAYER.C */
void	moveplayer(t_game *game);

/* ERRORS.C */
void	error_log(int error_code);

/* SDL */
void	quit_sdl(t_sdlcontext *sdl);

/*DEBUG FILES*/
void printf_tri(t_triangle tri);
void printf_quat(t_quaternion v);
void printf_vec(t_vector3 v);
void printf_matrix(t_mat4x4 m);
void printf_point(t_point p);

#endif
/*
fill triangle max is 0
w 626.917114 x 1020.786072 y 159.740646 z 0.998807
w 1.000000 x 856.572571 y 0.000000 z 0.994737
w 0.000000 x 886.548523 y 0.000000 z 1.000000
x 1020 y 159
x 856 y 0
x 886 y 0
*/