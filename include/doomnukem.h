/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 20:54:09 by raho             ###   ########.fr       */
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
# define CLR_DARKGRAY 0x292929
# define CLR_GREEN 3002977
# define CLEARSCREEN "\e[1;1H\e[2J"

# define DEBUG_ON 1

# define IMGPATH "assets/images/"
# define OBJPATH "assets/objects/"

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

typedef enum e_entityID
{
	player
}	t_entityID;

typedef struct s_img
{
	t_point		size;
	char		name[128];
	uint32_t	*data;
	uint32_t	length;
}	t_img;

typedef struct s_font_chars
{
	char			id;
	t_point			pos;
	t_point			size;
	t_point			offset;
	int				xadvance;
}	t_font_chars;

typedef struct s_font
{
	char			*name;
	char			*file_name;
	int				size;
	int				padding;
	int				spacing;
	int				line_height;
	t_point			scale;
	int				char_count;
	t_font_chars	*chars;
	t_img			*bitmap;
	t_img			*texts[20];
}	t_font;

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
	float					*zbuffer;
	SDL_Renderer			*renderer; //TODO: for testing remove.
	t_img					*images;
	uint32_t				imagecount;
	struct s_object			*objects;
	uint32_t				objectcount;
	t_font					*font;
	uint32_t				window_w;
	uint32_t				window_h;
}	t_sdlcontext;

# define PERFGRAPH_SAMPLES 64

typedef struct s_perfgraph
{
	t_img		image;
	uint32_t	deltas[PERFGRAPH_SAMPLES];
}	t_perfgraph;

typedef struct s_obj //TODO: this will be deprecated, t_object defined in 'objects.h' is the current object struct
{
	char		**mtlnames;
	uint32_t	*mtlcolors; //obj color type 3
	uint8_t		*colors; //Points to colors in mtlcolors
	int32_t		**verts;
	uint32_t	**faces;
	uint32_t	m_count;
	uint32_t	v_count;
	uint32_t	f_count;
}	t_obj;


typedef struct s_clock
{
	Uint32	prev_time;
	Uint32	delta;
} t_clock;

typedef struct s_editor
{
	t_line			line; //the line that is being edited right now //TODO: this should be moved to point_tool
	t_list			*linelist;
	t_list			*entitylist;
	t_list			*buttonlist;
	t_mouse			mouse;
	t_clock			clock;
	t_vector3		offset;
	struct s_tool	*tool;
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

typedef struct s_transform
{
	t_vector3	location;
	t_vector3	rotation;
	t_vector3	scale;
}	t_transform;

typedef struct s_entity
{
	t_transform		transform;
	struct s_object	*obj;
}	t_entity;

typedef struct s_bot
{
	t_entity	entity;
}	t_bot;

typedef struct s_wall
{
	t_entity	entity;
}	t_wall;

typedef struct s_item
{
	t_entity	entity;
}	t_item;

typedef struct s_render
{
	t_list			*listwall;
	t_list			*listbot;
	t_list			*listitem;
	t_vector3		vtarget;
	t_mat4x4		matcamera;
	t_mat4x4		matview;
	t_mat4x4		matworld;
	t_mat4x4		matproj;
	t_vector3		position;
	t_vector3		lookdir;
	t_triangle		*draw_triangles;
	t_triangle		*calc_triangles;
	uint32_t		draw_tri_count;
	uint32_t		calc_tri_count;
	t_img			*img;
	t_img			*debug_img;
	t_quaternion	*q;
}	t_render;

typedef struct s_game
{
	t_list			*listwall;
	t_list			*listbot;
	t_list			*listitem;
	//t_entity		entity[1000];
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
void	renderlines(t_sdlcontext *sdl, t_editor *ed); //TODO:  LEGACY, remove

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
t_img	*get_image_by_index(t_sdlcontext sdl, int index);
t_img	*get_image_by_name(t_sdlcontext sdl, char *name);

/* DELTATIME.C */
void	update_deltatime(t_clock *c);

/* INIT_RENDER.C */
t_render	init_render(t_sdlcontext sdl);

/* DRAW.C */
void	draw(t_sdlcontext sdl, t_point pos, uint32_t clr);
void	drawline(t_sdlcontext sdl, t_point from, t_point to, uint32_t clr);
void	drawcircle(t_sdlcontext sdl, t_point pos, int size, uint32_t clr);
void	drawrectangle(t_sdlcontext, t_rectangle rect, uint32_t clr);

// Draws a text that has been saved to font->texts[i] with save_text()
void	draw_saved_text(t_sdlcontext *sdl, t_img *text, t_point pos);

// Draws a text without saving it anywhere.
// Fonts: 0 = 11, 1 = 12, 2 = 14, 3 = 16, 4 = 18, 5 = 20, 6 = 22
void	draw_text(t_sdlcontext *sdl, const char *str, t_point pos, int font_size);

/* EDITOR_BUTTONS.C */
void	draw_editor_buttons(t_sdlcontext sdl, uint8_t tool_selected); //TODO: MOVE TO EDITOR_TOOLS
void	check_tool_change_click(t_point cursor, t_editor *ed); //TODO: MOVE TO EDITOR_TOOLS

//Draws image 'img' to pixels 'pxls', offset by point 'pos' and scaled to 'scale'
void	draw_image(t_sdlcontext sdl, t_point pos, t_img img, t_point scale);

/* FONT.C */
void	load_fonts(t_sdlcontext *sdl);

/* PERFGRAPH.C */
void	drawperfgraph(t_perfgraph *graph, uint32_t delta, t_sdlcontext sdl);

/* PLAYMODE.C */
int		playmode(t_sdlcontext sdl);
void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle, t_img img);
void	engine3d(t_sdlcontext sdl, t_render render);

/* PHYSICS.C */
bool	pointrectanglecollision(t_point p, t_rectangle rect);
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

/* TEXT.C */
// font[0] = 11, [1] = 12, [2] = 14, [3] = 16, [4] = 18, [5] = 20, [6] = 22
void	save_text(t_font *font, const char *str);

/* LIST_HELPER.C */
void	list_push(t_list **head, void *content, size_t content_size);
void	*list_to_ptr(t_list *source, uint32_t *set_length);
//TODO: documentation here
void	list_remove(t_list **head, void *match, size_t content_size);

/*DEBUG FILES*/
void printf_tri(t_triangle tri);
void printf_quat(t_quaternion v);
void printf_vec(t_vector3 v);
void printf_matrix(t_mat4x4 m);
void printf_point(t_point p);
void printf_texture(t_texture t);
void printf_face(void *face);

#endif
