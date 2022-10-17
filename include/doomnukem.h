/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/17 23:49:52 by okinnune         ###   ########.fr       */
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
# include "shapes.h"
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



// Playmode defines
# define OVERHEADCAMSPEED 0.028f
# define PLAYERRADIUS 16
# define COLLISION_ON //Comment/uncomment to toggle experimental collision

# define MOVESPEED 0.010f
# define MAXMOVEMENTSPEED 0.75f
# define ROTATESPEED 0.002f
# define MOUSESPEED 0.002f

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
}	t_sdlcontext;

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
	uint32_t	*data;
	uint32_t	length;
}	t_img;

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


typedef struct	s_tool
{
	void	(*click_func)(t_point, int);
	void	(*key_func)(int);
	t_mouse	mouse;
}	t_tool; //TODO: make static array of these and use test them in editor_mouse

t_tool	*get_tools();

/*

	tool_click(t_point position, int mousekey);
	tool_click(t_point position, int mousekey);

*/
/*typedef enum	e_tooltype //TODO: Unused right now
{
	tool_points,
	tool_define_room,
	tool_entity,
}	t_tool;*/

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
	t_vector2	position; //TODO: might be changed to int[2], don't know yet
	float		angle;
}	t_player;

typedef enum	e_cam_mode
{
	overhead_follow,
	overhead_absolute
}	t_cam_mode;

typedef struct s_game
{
	t_list			*linelst;
	t_clock			clock;
	t_mouse			mouse;
	t_player		player;
	int32_t			keystate;
	t_cam_mode		cam_mode;
	t_vector2		overheadcam_pos;
} t_game;

typedef enum e_gamereturn
{
	game_error,
	game_continue,
	game_exit,
	game_switchmode
} t_gamereturn;

/* V2.C */

void	v2add(int v[2], int ov[2]);
void	v2mul(int v[2], int mul);
void	v2cpy(int to[2], int from[2]);
bool	v2cmp(int v[2], int ov[2]);
void	v2diff(int v[2], int ov[2], int rv[2]);
void	v2mul_to_f2(int v[2], float mul, float f[2]);

float	f2dist(float f[2], float of[2]);
void	f2mul(float f[2], float mul); //TODO: move f2 functions to own file and maybe think of better naming?
void	f2tov2(float f[2], int v[2]);
void	f2add(float f[2], float of[2]);
void	f2cpy(float to[2], float from[2]);

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
void	loadmap(t_list **head, char *filename);
void	savemap(t_editor *ed, char *filename);

/* IMG.C */
void	alloc_image(t_img *img, int width, int height);

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
void	imgtoscreen(uint32_t *pxls, t_img *img);
void	drawrectangle(uint32_t *pxls, t_rectangle rect, uint32_t clr);

/* EDITOR_BUTTONS.C */
void	draw_editor_buttons(t_sdlcontext sdl);

/* PLAYMODE.C */
int		playmode(t_sdlcontext sdl);

/* PHYSICS.C */
bool	pointrectanglecollision(t_point p, t_rectangle rect);
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

#endif