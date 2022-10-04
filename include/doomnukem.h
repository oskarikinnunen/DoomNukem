/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 10:05:22 by okinnune         ###   ########.fr       */
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
# define V2NOTSET -42



typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
	//t_simpleimg				*images;
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

typedef enum e_editorstate
{
	place_start,
	place_end
}	t_editorstate;

typedef struct s_editor
{
	t_editorstate	state;
	t_line			line;
	t_list			*linelist;
	t_mousedrag		mousedrag;
	t_mouse			mouse;
}	t_editor;

/* V2.C */
void	v2add(int v[2], int ov[2]);
void	v2mul(int v[2], int mul);

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


/* DRAW.C */
void	draw(uint32_t *pxls, int pos[2], uint32_t clr);
void	drawline(uint32_t *pxls, int from[2], int to[2], uint32_t clr);
void	drawcircle(uint32_t *pxls, int crd[2], int size, uint32_t clr);