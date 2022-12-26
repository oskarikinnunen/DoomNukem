#ifndef RENDER_H
# define RENDER_H

# include "../libs/installed_libs/include/SDL2/SDL.h" //TODO: make this work without relative path?
# include "../libs/installed_libs/include/SDL2/SDL_ttf.h"
# include "vectors.h"
# include "shapes.h"
# include "objects.h" // only one function is using currently can be moved to doomnukem.h if needed
# include "lighting.h"
# include "occlusion.h"

# include "fmod.h"

# define CLR_PRPL 14231500
# define CLR_TURQ 5505010
# define CLR_BLUE 255
# define CLR_GRAY 4868682
# define CLR_DARKGRAY 0x292929
# define CLR_GREEN 3002977
# define CLR_RED 0xFF2919
# define AMBER_0 0x551501
# define AMBER_1 0x832d01
//# define AMBER_2 0x831f00
# define AMBER_2 0xbf7702
# define AMBER_3 0xff9b05
# define AMBER_4 0xf5a845
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

typedef struct s_backgroundcolors
{
	uint32_t	black;
	uint32_t	white;
	uint32_t	dark_grey;
	uint32_t	light_grey;
	uint32_t	brown;
}	t_backgroundcolors;

typedef struct s_font
{
	t_fontcolors		font_colors;
	t_backgroundcolors	background_colors;
	SDL_Color			color;
	uint32_t			box_color;
	TTF_Font			*font_sizes[4];
	TTF_Font			*font;
	char				*text;
}	t_font;

typedef struct s_audiosample
{
	char			name[64];
	FMOD_SOUND		*sound;
	FMOD_CHANNEL	*channel;
}	t_audiosample;

typedef struct s_audio
{
	FMOD_SYSTEM			*system;
	float				max_volume;
	uint32_t			samplecount;
	t_audiosample		sample[10];
}	t_audio;

typedef struct	s_triangle
{
	t_quaternion	p[3];
	t_texture		t[3];
	uint32_t		clr;
}	t_triangle;

typedef struct s_point_triangle
{
	t_point			p[3];
	t_texture		t[3];
	uint32_t		clr;
}	t_point_triangle;

typedef struct s_render_statistics
{
	bool		statistics; // turns on, off
	uint32_t	triangle_count;	// triangle draw count per frame
	uint32_t	render_count; // render_entity called count per frame
	uint32_t	frustrum_cull_amount; // amount of objects culled per frame
	uint32_t	peripheral_cull_amount; // amount of objects culled per frame
	uint32_t	occlusion_cull_amount; // amount of objects culled per frame
	uint32_t	occluder_count; // amount of occluders per frame
}	t_render_statistics;

typedef struct s_debug_occlusion
{
	bool		occlusion; // turns on, off
	bool		occluder_box; // turns on occluder boxes blue;
	bool		cull_box;	// turns on cull boxes green not occluded, red occluded;
}	t_debug_occlusion;

typedef struct s_camera
{
	t_vector3			vtarget;
	t_mat4x4			matcamera;
	t_mat4x4			matview;
	t_mat4x4			matworld;
	t_mat4x4			matproj;
	t_vector3			position;
	t_vector3			lookdir;
}	t_camera;

typedef struct s_render
{
	t_camera			camera;
	t_triangle			*occ_draw_tris;
	t_triangle			*occ_calc_tris;
	t_point_triangle	*worldspace_ptris;
	t_point_triangle	*screenspace_ptris;
	uint32_t			worldspace_ptri_count;
	uint32_t			screenspace_ptri_count;
	uint32_t			occ_tri_count;
	uint32_t			occ_calc_tri_count;
	t_img				*img;
	t_img				*debug_img;
	t_quaternion		*q;
	bool				wireframe;
	uint32_t			gizmocolor;
	t_render_statistics	rs;
	struct s_world		*world;
	t_debug_occlusion	occlusion;
	t_map				map;
	bool				is_wrap;
	struct s_sdlcontext	*sdl;
}	t_render;

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*window_surface;
	SDL_Surface				*surface;
	SDL_Surface				*ui_surface;
	SDL_Surface				*testsurf;
	t_render				render;
	t_platform				platform;
	float					*zbuffer;
	float					resolution_scaling;
	SDL_Renderer			*renderer; //TODO: for testing remove.
	t_img					*images;
	uint32_t				imagecount;
	struct s_object			*objects;
	int						ps1_tri_div;
	bool					global_wireframe;
	uint32_t				objectcount;
	t_font					font;
	t_audio					audio;
	uint32_t				window_w;
	uint32_t				window_h;
	t_point					screensize;
	uint8_t					ambient_light;
	t_bitmask				bitmask;
	uint32_t				*cpy_pixel;
	float					*buf1;
	float					*buf2;
	float					*draw_buffer;
	float					*copy_buffer;
}	t_sdlcontext;

void	alloc_image(t_img *img, int width, int height);
t_img	*get_image_by_index(t_sdlcontext sdl, int index); //TODO: add comments
t_img	*get_image_by_name(t_sdlcontext sdl, char *name);

void	rescale_surface(t_sdlcontext *sdl);


//Draws image 'img' to pixels 'pxls', offset by point 'pos' and scaled to 'scale'
void	draw_image(t_sdlcontext sdl, t_point pos, t_img img, t_point scale);

/* DRAW.C */
void	draw(t_sdlcontext sdl, t_point pos, uint32_t clr);
void	draw_alpha(t_sdlcontext sdl, t_point pos, uint32_t clr);
void	drawline(t_sdlcontext sdl, t_point from, t_point to, uint32_t clr);
void	drawcircle(t_sdlcontext sdl, t_point pos, int size, uint32_t clr);
void	drawrectangle(t_sdlcontext, t_rectangle rect, uint32_t clr);
void	draw_rectangle_filled(t_sdlcontext sdl, t_rectangle rect, uint32_t clr);
void	draw_rectangle_raster(t_sdlcontext sdl, t_rectangle rect, uint32_t clr);
void	draw_triangle(t_sdlcontext *sdl, t_point p1, t_point p2, t_point p3, uint32_t clr);

/* INIT_RENDER.C */
t_render	init_render(t_sdlcontext sdl, struct s_world *world);
void		free_render(t_render render);
void		render_start(t_render *render);

/* RENDER */
void				render_gizmo(t_sdlcontext sdl, t_render render, t_vector3 pos, int size);
void				render_ray(t_sdlcontext sdl, t_render render, t_vector3 from, t_vector3 to);
int					triangle_clipagainstplane(t_vector3 plane_p, t_vector3 plane_n, t_triangle *in_tri, t_triangle out_tri[2]);
void				draw_screen_to_worldspace_ray(t_sdlcontext sdl, t_render render, t_point origin, t_vector2 angle);
void				clipped_point_triangle(t_render *render, t_sdlcontext sdl);
void				render_buffer(t_sdlcontext *sdl, t_render *render);
t_triangle			triangle_to_viewspace(t_triangle tritransformed, t_mat4x4 matview);
t_point_triangle	triangle_to_screenspace_point_triangle(t_mat4x4 matproj, t_triangle clipped, t_sdlcontext sdl);

/* RASTERIZER */
void				render_triangle(t_sdlcontext *sdl, t_render *render, int index);
void				render_triangle_wrap(t_sdlcontext *sdl, t_render *render, int index);
void				render_triangle_uv(t_lighting l, t_triangle_polygon triangle);
bool				render_triangle_bit(t_sdlcontext *sdl, t_bit_triangle triangle);

bool fill_point_bit_tri_top(t_sdlcontext *sdl, t_point *p);
bool fill_point_bit_tri_bot(t_sdlcontext *sdl, t_point *p);

/* AUDIO */

void	load_audio(t_audio *sudio);

void	play_sound(t_audio *audio, const char *name);

void	pause_audio(t_audio *audio);

void	play_music(t_audio *audio);

void	close_audio(t_audio *audio);

int		clip_triangle_against_occluder_plane(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle out_tri[2]);
int		clip_triangle_against_plane(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle out_tri[2]);
void	clipped(t_render *render, t_sdlcontext sdl);

int		point_clip_triangle_against_plane(t_vector2 plane_p, t_vector2 plane_n, t_point_triangle in_tri, t_point_triangle out_tri[2]);

/* SURFACE TOOLS */

void	join_surfaces(SDL_Surface *dest, SDL_Surface *src);
void	join_text_to_surface(SDL_Surface *dest, SDL_Surface *src, t_point pos, uint8_t alpha);
void	join_text_boxed_to_surface(t_sdlcontext *sdl, SDL_Surface *src, t_point pos, t_point padding);

/*occlusion*/
void get_min_max_from_triangles(t_vector2 *min, t_vector2 *max, t_triangle *t, int count);

/*Render helper*/
t_point_triangle	wf_tri(t_point_triangle in, float scaling);
t_texture			calc_step_texture(t_texture *t, float delta);
void				calc_points_step(float x_step[2], t_texture t_step[2], t_point *p, t_texture *t, float delta);
void				sort_point_uv_tri(t_point *p, t_texture *t);
void				sort_point_tri(t_point *p);
void				sort_polygon_tri(t_point *p2, t_vector2 *t, t_vector3 *p3);
void				ft_swap(void * a, void * b, size_t len);
t_point_triangle	ps1(t_point_triangle in, int div);
uint32_t			flip_channels(uint32_t clr);


#endif