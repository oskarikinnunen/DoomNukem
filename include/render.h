#ifndef RENDER_H
# define RENDER_H

# include "../libs/installed_libs/include/SDL2/SDL.h" //TODO: make this work without relative path?
# include "../libs/installed_libs/include/SDL2/SDL_ttf.h"
# include "vectors.h"
# include "shapes.h"
# include "entity.h"

# define CLR_PRPL 14231500
# define CLR_TURQ 5505010
# define CLR_BLUE 255
# define CLR_GRAY 4868682
# define CLR_DARKGRAY 0x292929
# define CLR_GREEN 3002977
# define CLR_RED 0xFF2919
# define CLEARSCREEN "\e[1;1H\e[2J"

typedef enum e_platform
{
	os_mac,
	os_linux,
	os_unsupported
}	t_platform;

typedef struct s_img
{
	t_point		size;
	char		name[128];
	uint32_t	*data;
	uint32_t	length;
}	t_img;

typedef struct s_fontcolors
{
	SDL_Color	black;
	SDL_Color	white;
	SDL_Color	red;
	SDL_Color	green;
	SDL_Color	blue;
	SDL_Color	yellow;
	SDL_Color	crimson;
	SDL_Color	orange;
	SDL_Color	skyblue;
}	t_fontcolors;

typedef struct s_font
{
	t_fontcolors	font_colors;
	SDL_Color		color;
	TTF_Font		*font_sizes[4];
	TTF_Font		*font;
	char			*text;
}	t_font;

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
	t_font					font;
	uint32_t				window_w;
	uint32_t				window_h;
	t_point					screensize;
}	t_sdlcontext;


void	alloc_image(t_img *img, int width, int height);
t_img	*get_image_by_index(t_sdlcontext sdl, int index); //TODO: add comments
t_img	*get_image_by_name(t_sdlcontext sdl, char *name);

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

//Draws image 'img' to pixels 'pxls', offset by point 'pos' and scaled to 'scale'
void	draw_image(t_sdlcontext sdl, t_point pos, t_img img, t_point scale);

/* DRAW.C */
void	draw(t_sdlcontext sdl, t_point pos, uint32_t clr);
void	drawline(t_sdlcontext sdl, t_point from, t_point to, uint32_t clr);
void	drawcircle(t_sdlcontext sdl, t_point pos, int size, uint32_t clr);
void	drawrectangle(t_sdlcontext, t_rectangle rect, uint32_t clr);

/* INIT_RENDER.C */
t_render	init_render(t_sdlcontext sdl);
void		free_render(t_render render);
void		render_start(t_render *render);

/* RENDER */
void	z_fill_tri(t_sdlcontext sdl, t_triangle triangle, t_img img);
void	z_fill_tri_solid(t_sdlcontext sdl, t_triangle triangle);
void	render_entity(t_sdlcontext sdl, t_render render, t_entity *entity);
void	render_object(t_sdlcontext sdl, t_render render, struct s_object *obj);
void	render_gizmo(t_sdlcontext sdl, t_render render, t_vector3 pos, int size);
void	render_ray(t_sdlcontext sdl, t_render render, t_vector3 from, t_vector3 to);
int		triangle_clipagainstplane(t_vector3 plane_p, t_vector3 plane_n, t_triangle *in_tri, t_triangle out_tri[2]);
void	draw_screen_to_worldspace_ray(t_sdlcontext sdl, t_render render, t_point origin, t_vector2 angle);

#endif