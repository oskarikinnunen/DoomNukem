/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/05 14:01:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../SDL_built/include/SDL2/SDL.h" //TODO: make this work without relative path?
#include "libft.h"
#include "limits.h" //TODO: remove mby? just define INT_MAX yourself
#include "math.h"
#include <fcntl.h>
#include <stdbool.h>

# define WINDOW_W 800
# define WINDOW_H 600
# define TILESIZE 32 //EDITOR tilesize
# define GRIDSIZE 64 //EDITOR gridsize (how many addressable coordinates we have)

# define PI	3.14159265359
# define FULLRAD PI * 2.0

# define X 0
# define Y 1
# define Z 2

# define MOUSE_LEFT 1
# define MOUSE_MDL 2
# define MOUSE_RIGHT 3

# define CLR_PRPL 14231500
# define CLR_TURQ 5505010
# define CLR_GRAY 4868682

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
}	t_sdlcontext;

typedef struct s_mouse
{
	int		p[2];
	int		scroll;
	int		scroll_delta;
	bool	click;
	int		held;
}	t_mouse;

typedef struct s_mousedrag
{
	int	pos[2];
	int	pos_origin[2];
	int	drag[2];
	int	drag_origin[2];
}	t_mousedrag;

typedef	struct s_line
{
	int	start[2];
	int	end[2];
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
	uint32_t	size[2];
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

typedef struct s_fdf
{
	t_img				img;
	t_obj				obj;
	int					curframe;
	float				*depth;
	float				**verts;
	float				matrices[2][3][3];
	float				view[2];
	float				offset[2];
	float				zoom;
}	t_fdf;

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
	t_line			line;
	t_list			*linelist;
	t_mousedrag		mousedrag[2]; //First one is right click drag, 2nd is for middle click
	t_mouse			mouse;
	t_fdf			grid_fdf;
	t_fdf			walls_fdf;
	float			threedee_zoom;
	t_anim			transition;
	t_clock			clock;
}	t_editor;

/* V2.C */
void	f2mul(float f[2], float mul); //TODO: move f2 functions to own file and maybe think of better naming?
void	v2add(int v[2], int ov[2]);
void	v2mul(int v[2], int mul);
void	v2mul_to_f2(int v[2], float mul, float f[2]);
void	v2cpy(int to[2], int from[2]);
bool	v2cmp(int v[2], int ov[2]);

/* EDITOR.C */
void	editorloop(t_sdlcontext sdl);

/* EDITOR_EVENTS.C */
int		editor_events(t_editor *ed);

/* EDITOR_RENDER.C */
void	renderlines(t_sdlcontext *sdl, t_editor *ed); //TODO:  better name?

/* EDITOR_MOUSE.C */
void	mousetoworldspace(int v2[2], t_editor *ed);
void	mouse_event(SDL_Event e, t_editor *ed);

/* EDITOR_SAVELINE.C */
void	saveline(t_editor *ed);

/* EDITOR_MAP_IO.C */
void	loadmap(t_editor *ed, char *filename);
void	savemap(t_editor *ed, char *filename);

/* EDITOR_3D_INITIALIZE.C */
void	gridto_obj(t_obj *obj);
void	lines_to_obj(t_obj *obj, t_editor *ed);

/* EDITOR_3D.C */
void	editor3d(t_sdlcontext sdl, t_editor *ed_ptr);

/* FDF.C */
int		fdf_init(t_fdf *fdf);
void	fdf_draw_wireframe(t_fdf *fdf);

/* IMG.C */
void	alloc_image(t_img *img, int width, int height);

/* DELTATIME.C */
void	update_deltatime(t_clock *c);

/* ANIM.C */
void	update_anim(t_anim *anim, uint32_t delta);
void	start_anim(t_anim *anim, t_anim_mode mode);

/* DRAW.C */
void	draw(uint32_t *pxls, int pos[2], uint32_t clr);
void	drawline(uint32_t *pxls, int from[2], int to[2], uint32_t clr);
void	drawcircle(uint32_t *pxls, int crd[2], int size, uint32_t clr);
void	imgtoscreen(uint32_t *pxls, t_img *img);