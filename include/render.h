#ifndef RENDER_H
# define RENDER_H

# include "../libs/installed_libs/include/SDL2/SDL.h" //TODO: make this work without relative path?
# include "../libs/installed_libs/include/SDL2/SDL_ttf.h"
# include "vectors.h"
# include "shapes.h"
# include "objects.h" // only one function is using currently can be moved to doomnukem.h if needed
# include "lighting.h"
# include "occlusion.h"
# include "colors.h"
# include "render_utils.h"

# include "fmod.h"

# define CLR_BLACK 0xFF000000
# define CLR_WHITE 0xFFFFFFFF
# define CLR_PRPL 14231500
# define CLR_TURQ 5505010
# define CLR_BLUE 255
# define CLR_GRAY 4868682
# define CLR_DARKGRAY 0x292929
# define CLR_BLACKGRAY 0x1a1a1a
# define CLR_LIGHTGRAY 0xFF222222
# define CLR_BROWN 0xFF4C3228
# define CLR_GREEN 3002977
# define CLR_RED 0xFF2919
# define AMBER_0 0x551501
# define AMBER_1 0x832d01
//# define AMBER_2 0x831f00
# define AMBER_2 0xbf7702
# define AMBER_3 0xff9b05
# define AMBER_4 0xf5a845
# define CLEARSCREEN "\e[1;1H\e[2J"
# define DEFAULT_FONT_SIZE 8
# define FONT_SIZE_COUNT 4

typedef struct s_step
{
	float	location;
	float	step;
}	t_step;

typedef struct s_stepv3
{
	t_vector3	step;
	t_vector3	location;
}	t_stepv3;

typedef struct s_stepv2
{
	t_vector2	step;
	t_vector2	location;
}	t_stepv2;

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
	bool		transparency;
}	t_img;

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
	t_backgroundcolors	background_colors;
	SDL_Color			color;
	uint32_t			box_color;
	TTF_Font			*sizes[4];
	uint32_t			size_count;
	TTF_Font			*size_default;
	char				name[128];
	char				*text;
}	t_font;

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
	bool		draw_occlusion;
	bool		slow_render;
}	t_debug_occlusion;

typedef enum e_rend_lightmode
{
	lm_unlit,
	lm_lit,
	lm_dynamic
}	t_rend_lightmode;

typedef struct s_render
{
	t_camera			camera;
	t_map				map;
	t_img				*img;
	t_quaternion		*q;
	t_triangle			*world_triangles;
	uint32_t			start_index;
	uint32_t			end_index;
	t_point_triangle	*screenspace_ptris;
	uint32_t			screenspace_ptri_count;
	t_rend_lightmode	lightmode;
	uint32_t			dynamic_light;
	t_debug_occlusion	occlusion;
	t_v2rectangle		screen_edge;
	bool				wireframe;
	uint32_t			gizmocolor;
	t_img				*debug_img;
	t_render_statistics	rs;
}	t_render;

typedef struct s_audiosample
{
	FMOD_SOUND		*sound;
	char			name[64];
}	t_audiosample;

/*
	entity
		->loadcomponent

	prefab

	audiosource
	npc
	interactable



*/

typedef struct s_music_control
{
	t_audiosample	nextsong;
	bool			active;
	bool			lastfade;
	float			fade;
}	t_music_control;

typedef struct s_audio
{
	FMOD_SYSTEM			*system;
	float				sfx_volume;
	float				music_volume;
	t_audiosample		*samples;
	uint32_t			samplecount;
	t_music_control		music_control;
	t_audiosample		*music;
	uint32_t			music_count;
	FMOD_CHANNEL		*music_channel;
	//FMOD_CHANNEL		*music_channel2;
	/*uint32_t			musiccount;
	t_audiosample		music[5];*/
}	t_audio;

typedef enum e_screenmode
{
	screenmode_windowed,
	screenmode_borderless,
	screenmode_fullscreen
}	t_screenmode;

typedef struct s_graphicprefs //Intermediate struct used for loading settings from a configuration file
{
	t_screenmode	screenmode;
	int32_t		resolution_x;
	int32_t		resolution_y;
	float			resolutionscale;
	float			volume;
}	t_graphicprefs;

/*typedef struct s_renderthreadinfo
{
	//t_rectangle	bounds
}	t_renderthreadinfo;*/

typedef struct s_human_animation
{
	char			name[32];
	uint32_t		startframe;
	uint32_t		endframe;
}	t_human_animation;

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*window_surface;
	SDL_Surface				*surface;
	SDL_Surface				*ui_surface; //Not needed?
	uint32_t				*scaling_buffer;
	t_render				render;
	t_platform				platform;
	float					*zbuffer;
	float					resolution_scaling;
	SDL_Renderer			*renderer; //TODO: for testing remove.
	t_img					*images;
	uint32_t				imagecount;
	t_img					*env_textures;
	uint32_t				env_texturecount;
	struct s_object			*objects;
	struct s_human_animation	*human_anims;
	uint32_t				human_anim_count;
	int						ps1_tri_div;
	bool					global_wireframe;
	bool					blend;
	bool					render_grid;
	bool					lighting_toggled;
	bool					fog;
	uint32_t				objectcount;
	t_font					*fonts;
	uint32_t				fontcount;
	t_font					*font_default;
	t_audio					audio;
	uint32_t				window_w;
	uint32_t				window_h;
	t_point					screensize;
	t_bitmask				bitmask;
}	t_sdlcontext;

void			save_graphics_prefs(t_sdlcontext *sdl);
t_graphicprefs	get_prefs_from_sdl(t_sdlcontext *sdl);
t_graphicprefs	load_graphicsprefs();
void			reset_graphics_prefs();
void			set_sdl_settings(t_sdlcontext *sdl);
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
void	draw_rectangle(t_sdlcontext, t_rectangle rect, uint32_t clr);
void	draw_rectangle_filled(t_sdlcontext sdl, t_rectangle rect, uint32_t clr);
void	draw_rectangle_raster(t_sdlcontext sdl, t_rectangle rect, uint32_t clr);
void	draw_rectangle_raster_few(t_sdlcontext sdl, t_rectangle rect, uint32_t clr);
void	draw_triangle(t_sdlcontext *sdl, t_point p1, t_point p2, t_point p3, uint32_t clr);

/* INIT_RENDER.C */
t_render	init_render(t_sdlcontext sdl);
void		free_render(t_render render);
void		calculate_matview(t_camera *camera);

/* RENDER */
void				render_gizmo(t_sdlcontext sdl, t_render render, t_vector3 pos, int size);
void				render_gizmo3d(t_sdlcontext *sdl, t_vector3 pos, int size, uint32_t color);
void				render_gizmo2d(t_sdlcontext *sdl, t_vector2 pos, int size, uint32_t color);
void				render_ball(t_sdlcontext *sdl, t_vector3 pos, float size, uint32_t clr);
void				render_capsule(t_sdlcontext *sdl, t_vector3 pos, float height, float radius, uint32_t clr);
void				render_circle(t_sdlcontext *sdl, t_vector3 pos, float radius, uint32_t clr);
void				render_ray(t_sdlcontext *sdl, t_vector3 from, t_vector3 to);
void				render_ray3D(t_sdlcontext *sdl, t_vector3 from, t_vector3 to, uint32_t clr);
void				draw_screen_to_worldspace_ray(t_sdlcontext sdl, t_render render, t_point origin, t_vector2 angle);
void				clipped_point_triangle(t_render *render, t_sdlcontext sdl);
void				render_buffer(t_sdlcontext *sdl, t_render *render);
t_triangle			triangle_to_viewspace(t_triangle tritransformed, t_mat4x4 matview);
t_point_triangle	triangle_to_screenspace_point_triangle(t_mat4x4 matproj, t_triangle clipped, t_sdlcontext sdl);
bool				is_triangle_backface(t_triangle tritransformed, t_render *render);
/* RASTERIZER */
void				render_triangle_lit(t_sdlcontext *sdl, t_render *render, int index);
void				render_triangle_uv(t_lighting l, t_point_triangle triangle);
void				render_triangle_unlit(t_sdlcontext *sdl, t_render *render, int index);
void				render_triangle_dynamic(t_sdlcontext *sdl, t_render *render, int index);
void				rasterize_light(t_point_triangle triangle, t_lighting *lighting);

/* AUDIO TOOLS */

int				check_channel_status(FMOD_CHANNEL *channel);
t_audiosample	get_sample(t_sdlcontext *sdl, const char *name);
t_audiosample	get_music(t_sdlcontext *sdl, const char *name);
void			update_maxvolume(t_audio *audio);

/* AUDIO */

void	create_audio(t_audio *audio);
void	editor_load_sounds(t_audio *audio);
void	editor_load_music(t_audio *audio);
void	pause_audio(t_audio *audio, bool pause);
void	close_audio(t_audio *audio);
void	play_music(t_sdlcontext *sdl, char *musicname);
void	change_music(t_sdlcontext *sdl, char *musicname);
//void	play_localsound(t_audio *audio, const char *name); //DEPRECATED
//void	play_worldsound(t_audio *audio, const char *name, t_vector3 *pos);
void	pause_sound(t_audio *audio, const char *name, bool pause);
//void	pause_all_sounds(t_audio *audio, bool pause);
FMOD_MODE	get_mask(char *sound_path);
void	create_sound(int sample_i, char *sound_path, t_audio *audio);
void	create_music(int music_i, char *music_path, t_audio *audio);


void	pause_music(t_audio *audio, bool pause);
void	stop_music(t_audio *audio);

int		clip_triangle_against_plane(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle out_tri[2]);
int		point_clip_triangle_against_plane(t_vector2 plane_p, t_vector2 plane_n, t_point_triangle in_tri, t_point_triangle out_tri[2]);
int		clip_triangle_against_occluder_plane(t_vector2 plane_p, t_vector2 plane_n, t_point_triangle in_tri, t_point_triangle out_tri[2]);
//bitmask
int			clip_bitmask_triangle_against_plane(t_vector3 plane_p, t_vector3 plane_n, t_triangle in_tri, t_triangle out_tri[2]); //TODO: Implement this with gpu rendering
t_bitmask	init_bitmask(t_sdlcontext *sdl);

/* SURFACE TOOLS */

void	join_surfaces(SDL_Surface *dest, SDL_Surface *src);
void	join_text_to_surface(SDL_Surface *dest, SDL_Surface *src, t_point pos, uint8_t alpha);
void	join_text_boxed_to_surface(t_sdlcontext *sdl, SDL_Surface *src, t_point pos, t_point padding);

/*occlusion*/
void	set_square_from_triangles(t_occlusion *occl, t_point_triangle *t, int count);
void	render_bitmask(t_sdlcontext *sdl, t_render *render);

/*Render helper*/
int					triangle_to_flat(t_point_triangle triangle, t_point_triangle tris[2]);
t_point_triangle	wf_tri(t_point_triangle in, float scaling);
t_vector3			calc_step_texture(t_vector3 *t, float delta);
void				calc_points_step(float x_step[2], t_vector3 t_step[2], t_vector2 *p, t_vector3 *t, float delta);
void				sort_point_uv_tri(t_vector2 *p, t_vector3 *t);
void				sort_polygon_tri(t_vector2 *p2, t_vector2 *t, t_vector3 *p3);
void				sort_point_tri(t_vector2 *p2, float *w);
void				ft_swap(void * a, void * b, size_t len);
t_point_triangle	ps1(t_point_triangle in, int div);


#endif