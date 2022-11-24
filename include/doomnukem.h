/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/24 13:28:54 by raho             ###   ########.fr       */
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
# include "physics.h"
# include "animation.h" //PLAYER USES THIS, MOVE PLAYER TO SEPARATE HEADER
# include "inputhelp.h"

# define TILESIZE 32 //EDITOR tilesize
# define GRIDSIZE 64 //EDITOR gridsize (how many addressable coordinates we have)

# define PI	3.14159265359
# define FULLRAD M_PI * 2.0
# define RAD90		1.57079633f //90 degrees in radian

# define X 0
# define Y 1
# define Z 2

# define MOUSE_LEFT 1
# define MOUSE_MDL 2
# define MOUSE_RIGHT 3

# define CLR_PRPL 14231500
# define CLR_TURQ 5505010
# define CLR_BLUE 255
# define CLR_GRAY 4868682
# define CLR_DARKGRAY 0x292929
# define CLR_GREEN 3002977
# define CLR_RED 0xFF2919
# define CLEARSCREEN "\e[1;1H\e[2J"

# define DEBUG_ON 1

# define IMGPATH "assets/images/"
# define OBJPATH "assets/objects/"

// Playmode defines
# define OVERHEADCAMSPEED 0.028f
# define PLAYERRADIUS 16
# define COLLISION_ON //Comment/uncomment to toggle experimental collision

# define EDITOR_MOVESPEED 0.2f
# define MOVESPEED 0.2f
# define MAXMOVEMENTSPEED 0.08f
# define ROTATESPEED 0.002f
# define MOUSESPEED 0.0002f

# define TEXTBACKGROUND_BORDERPADDING 6

typedef struct s_mouse
{
	t_point	pos;
	t_point	delta;
	bool	relative;
	int		scroll;
	int		scroll_delta;
	bool	click_unhandled;
	int		click_button;
	int		held;
}	t_mouse;

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

typedef enum e_platform
{
	os_mac,
	os_linux,
	os_unsupported
}	t_platform;

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
	t_platform				platform;
	float					*zbuffer;
	SDL_Renderer			*renderer; //TODO: for testing remove.
	t_img					*images;
	uint32_t				imagecount;
	struct s_object			*objects;
	uint32_t				objectcount;
	t_font					*font;
	t_font					*all_fonts;
	uint32_t				window_w;
	uint32_t				window_h;
	t_point					screensize;
}	t_sdlcontext;

void	screen_blank(t_sdlcontext sdl);
# define PERFGRAPH_SAMPLES 64

typedef struct s_perfgraph
{
	t_img		image;
	uint32_t	deltas[PERFGRAPH_SAMPLES];
}	t_perfgraph;

typedef struct s_clock
{
	Uint32	prev_time;
	Uint32	delta;
	Uint32	fps;
} t_clock;

/* Playmode */

typedef struct s_player
{
	t_vector3	position; //TODO: might be changed to int[2], don't know yet
	t_vector2	angle;
	t_vector3	lookdir;
	t_anim		jump;
	float		height;
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
	uint32_t		clr;
}	t_triangle;

typedef struct s_transform
{
	t_vector3	location;
	t_vector3	rotation;
	t_vector3	scale;
}	t_transform;

typedef struct s_bound
{
	float	min;
	float	max;
}	t_bound;


/*typedef struct s_animframe
{
	struct deltamove = 
	{
		int			index;
		t_vector3	delta;
	};
	int deltaverticecount;
}	t_animframe;*/

typedef struct s_entity
{
	t_transform		transform;
	uint32_t		object_index;
	char			object_name[64];
	t_bound			z_bound;
	struct s_object	*obj;
}	t_entity;

typedef struct s_bot
{
	t_entity	entity;
}	t_bot;

typedef struct s_item
{
	t_entity	entity;
}	t_item;

typedef struct s_render
{
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
	bool			wireframe;
	uint32_t		gizmocolor;
}	t_render;

typedef struct s_world
{
	t_physics	physics;
	t_list		*entitylist;
	t_list		*meshlist;
	t_list		*wall_list;
	t_list		*roomlist;
	t_entity	skybox;
}	t_world;

void	calculate_colliders_for_entities(t_world *world);
void	render_world3d(t_sdlcontext sdl, t_world world, t_render *render);
t_world	load_world(char *filename, t_sdlcontext sdl);
void	save_world(char *filename, t_world world);

typedef struct s_editor
{
	t_world			world;
	t_clock			clock;
	t_mouse			mouse;
	uint32_t		keystate;
	t_gamereturn	gamereturn;
	/* typedef s_camera { */
	t_vector3		position;
	t_vector3		forward;
	t_vector2		forward_offset; //TODO: remove
	t_vector2		angle;
	/*} t_camera */
	t_list			*buttonlist;
	t_render		render;
	struct s_tool	*tool;
	
}	t_editor;

typedef struct s_game
{
	t_world			world;
	t_clock			clock;
	t_mouse			mouse;
	uint32_t		keystate;
	t_controller	controller[2];
	t_player		player;
	t_input			input;
	t_cam_mode		cam_mode; //Unused but will be reimplemented?
} t_game;

/* EDITOR.C */
int		editorloop(t_sdlcontext sdl);

/* EDITOR_EVENTS.C */
t_gamereturn	editor_events(t_editor *ed);
bool			iskey(SDL_Event e, int keycode);
void			force_mouseunlock(t_editor *ed);
void			move_editor(t_editor *ed);

/* EDITOR_MOUSE.C */
t_point			mousetoworldspace(t_editor *ed);
t_point			mousetogridspace(t_editor *ed);
t_point			screentogridspace(t_point point);
t_quaternion	transformed_vector3(t_transform transform, t_vector3 v);
void			mouse_event(SDL_Event e, t_editor *ed);

/* SPACECONVERSIONS.C */
t_point	worldto_editorspace(t_editor *ed, t_vector2 worldcrd);
t_point vector3_to_screenspace(t_render r, t_vector3 vec, t_sdlcontext sdl);

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
void		free_render(t_render render);
void		render_start(t_render *render);

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

/* CONTROLLER.C */
void	initialize_controllers(t_game *game);
int		controller_events(SDL_Event e, t_game *game);

/* PLAYMODE.C */
int		playmode(t_sdlcontext sdl);
void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle, t_img img);
void	z_fill_tri_solid(t_sdlcontext sdl, t_triangle triangle);
void	render_entity(t_sdlcontext sdl, t_render render, t_entity *entity);
void	render_object(t_sdlcontext sdl, t_render render, struct s_object *obj);
void	render_gizmo(t_sdlcontext sdl, t_render render, t_vector3 pos, int size);
void	render_ray(t_sdlcontext sdl, t_render render, t_vector3 from, t_vector3 to);
int		triangle_clipagainstplane(t_vector3 plane_p, t_vector3 plane_n, t_triangle *in_tri, t_triangle out_tri[2]);
void	draw_screen_to_worldspace_ray(t_sdlcontext sdl, t_render render, t_point origin, t_vector2 angle);

/* PHYSICS.C */
bool	pointrectanglecollision(t_point p, t_rectangle rect);
bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r);
//bool	linecirclecollision(t_line line, t_vector2 cp, float r);

/* MOVEPLAYER.C */
void	moveplayer(t_game *game);

/* ERRORS.C */
void	error_log(int error_code);

/* SDL */
void	quit_game(t_sdlcontext *sdl);

/* FONT.C */

void	load_fonts(t_sdlcontext *sdl);

void	draw_black_background(t_sdlcontext *sdl, t_point pos, t_point size);

// Fonts: 0 = 11, 1 = 12, 2 = 14, 3 = 16, 4 = 18, 5 = 20, 6 = 22
void	set_font_size(t_sdlcontext *sdl, int font_size);

/* TEXT.C */

// Saves the text to font->texts[i] in sequential order.
// Can be drawn with draw_saved_text()
void	save_text(t_font *font, const char *str);

// Draws a text that has been saved to font->texts[i] with save_text()
void	draw_saved_text(t_sdlcontext *sdl, t_img *text, t_point pos);

// Draws text without saving it anywhere
// Uses a pre-set font size that can be changed by calling set_font_size()
// Fonts: 0 = 11, 1 = 12, 2 = 14, 3 = 16, 4 = 18, 5 = 20, 6 = 22
void	draw_text(t_sdlcontext *sdl, const char *str, t_point pos, t_point boundaries);

// Draws text, with a black box in the background, without saving it anywhere
// Uses a pre-set font size that can be changed by calling set_font_size()
// Fonts: 0 = 11, 1 = 12, 2 = 14, 3 = 16, 4 = 18, 5 = 20, 6 = 22
// Returns the rectangle of the drawed textbox
t_rectangle	draw_text_boxed(t_sdlcontext *sdl, const char *str, t_point pos, t_point boundaries);

/* LIST_HELPER.C */
t_list	*ptr_to_list(void	*src, uint32_t len, size_t size);
void	list_push(t_list **head, void *content, size_t content_size);
void	*list_findlast(t_list *head);
void	*list_to_ptr(t_list *source, uint32_t *set_length);
void	*list_find(t_list *head, void *match, size_t content_size);
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
