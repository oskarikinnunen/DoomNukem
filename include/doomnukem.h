/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/26 16:03:41 by okinnune         ###   ########.fr       */
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
# include "shapes.h"

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
# define CLEARSCREEN "\e[1;1H\e[2J"

#define DEBUG_ON 1

// Playmode defines
# define OVERHEADCAMSPEED 0.028f
# define PLAYERRADIUS 16
# define COLLISION_ON //Comment/uncomment to toggle experimental collision


# define EDITOR_MOVESPEED 1.2f
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
	bool	click_unhandled;
	int		click_button;
	int		held;
}	t_mouse;

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

typedef enum e_entityID
{
	player
}	t_entityID;

typedef struct s_entity
{
	t_entityID	id;
	t_vector2	position;
	char		test1;
}	t_entity;

typedef enum e_editorstate
{
	e_place_start,
	e_place_end,
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
	uint32_t				*zbuffer;
	SDL_Renderer			*renderer; //TODO: for testing remove.
	t_img					*images;
	uint					imagecount;
	uint32_t				window_w;
	uint32_t				window_h;
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
	t_list			*entitylist;
	t_mouse			mouse;
	float			threedee_zoom;
	t_anim			transition;
	t_clock			clock;
	t_point			offset;
	struct s_tool	*tool;
	uint8_t			tool_selected;
	uint32_t		keystate;
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

typedef struct	s_triangle
{
	t_quaternion p[3];
	uint32_t	clr;
}	t_triangle;

typedef struct s_game
{
	int				tri_count;
	t_triangle		*triangles;
	t_list			*linelist;
	t_list			*entitylist;
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
t_point	mousetogridspace(t_editor *ed);
t_point	screentogridspace(t_point point);
void	mouse_event(SDL_Event e, t_editor *ed);

/* SPACECONVERSIONS.C */
t_point	worldtoeditorspace(t_editor *ed, t_vector2 worldcrd);

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
void	draw(t_sdlcontext sdl, t_point pos, uint32_t clr);
void	drawline(t_sdlcontext sdl, t_point from, t_point to, uint32_t clr);
void	drawcircle(t_sdlcontext sdl, t_point pos, int size, uint32_t clr);
void	drawrectangle(t_sdlcontext, t_rectangle rect, uint32_t clr);

/* EDITOR_BUTTONS.C */
void	draw_editor_buttons(t_sdlcontext sdl, uint8_t tool_selected); //TODO: MOVE TO EDITOR_TOOLS
void	check_tool_change_click(t_point cursor, t_editor *ed); //TODO: MOVE TO EDITOR_TOOLS

//Draws image 'img' to pixels 'pxls', offset by point 'pos' and scaled to 'scale'
void	draw_image(t_sdlcontext sdl, t_point pos, t_img img, t_point scale);

/* PERFGRAPH.C */
void	drawperfgraph(t_perfgraph *graph, uint32_t delta, t_sdlcontext sdl);

/* PLAYMODE.C */
int		playmode(t_sdlcontext sdl);
void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle);
void	engine3d(t_sdlcontext sdl, t_game game);

/* PHYSICS.C */
bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r);
bool	linecirclecollision(t_line line, t_vector2 cp, float r);

/* PLAYMODE_OVERHEAD.C */
void	render_overhead(t_game *game, t_sdlcontext sdl);
void	move_overhead(t_game *game);

/* MOVEPLAYER.C */
void	moveplayer(t_game *game);

/* ERRORS.C */
void	error_log(int error_code);

/* SDL */
void	quit_game(t_sdlcontext *sdl);

/* LIST_HELPER.C */
void	list_push(t_list **head, void *content, size_t content_size);
//TODO: documentation here
void	list_remove(t_list **head, void *match, size_t content_size);

/*DEBUG FILES*/
void printf_tri(t_triangle tri);
void printf_quat(t_quaternion v);
void printf_vec(t_vector3 v);
void printf_matrix(t_mat4x4 m);
void printf_point(t_point p);

#endif
