/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 13:14:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../SDL_built/include/SDL2/SDL.h" //TODO: make this work without relative path?
#include "libft.h"
#include "limits.h" //TODO: remove probably, used for INT_MAX
#include "math.h"
#include <fcntl.h>
#include <stdbool.h>

# define WINDOW_W 800
# define WINDOW_H 600
# define TILESIZE 32 //EDITOR tilesize
# define GRIDSIZE 64 //EDITOR tilesize
# define PI	3.14159265359
# define FULLRAD PI * 2.0

# define X 0
# define Y 1
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
	place_end
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
	uint32_t	m_count;
	int32_t		**verts;
	uint32_t	**faces;
	uint32_t	v_count;
	uint32_t	f_count;
}	t_obj;

typedef struct s_fdf
{
	struct s_simpleimg	*img;
	t_obj				obj;
	int					curframe;
	float				*depth;
	float				**verts;
	float				matrices[2][3][3];
	float				view[2];
}	t_fdf;

typedef struct s_editor
{
	t_editorstate	state;
	t_line			line;
	t_list			*linelist;
	t_mousedrag		mousedrag;
	t_mouse			mouse;
	t_fdf			fdf;
}	t_editor;

/* V2.C */
void	v2add(int v[2], int ov[2]);
void	v2mul(int v[2], int mul);
bool	v2cmp(int v[2], int ov[2]);

/* EDITOR.C */
void	editorloop(t_sdlcontext sdl);

/* EDITOR_EVENTS.C */
int		editor_events(t_editor *ed);

/* EDITOR_RENDER.C */
void	renderlines(t_sdlcontext *sdl, t_editor *ed); //TODO: invent better name?

/* EDITOR_MOUSE.C */
void	mousetoworldspace(int v2[2], t_editor *ed);
void	mouse_event(SDL_Event e, t_editor *ed);

/* EDITOR_SAVELINE.C */
void	saveline(t_editor *ed);

/* EDITOR_MAP_IO.C */
void	loadmap(t_editor *ed, char *filename);
void	savemap(t_editor *ed, char *filename);

/* EDITOR_3D.C */
void	gridto_obj(t_obj *obj);


/* DRAW.C */
void	draw(uint32_t *pxls, int pos[2], uint32_t clr);
void	drawline(uint32_t *pxls, int from[2], int to[2], uint32_t clr);
void	drawcircle(uint32_t *pxls, int crd[2], int size, uint32_t clr);