/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walltool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:17:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/14 15:59:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WALLTOOL_H
# define WALLTOOL_H

#include "objects.h"

typedef	enum e_walltoolmode
{
	place_first,
	place_other,
	place_height
}	t_walltoolmode;

#define MAXSELECTED 32

typedef struct s_floortri
{
	t_vector2	p[3];
}	t_floortri;

typedef struct s_floorcalc
{
	t_vector2	edges[32];
	t_face		faces[32];
	uint8_t		edgecount;
	uint8_t		facecount;
}	t_floorcalc;

typedef struct s_walltooldata
{
	t_wall			wall;
	t_wall			*hover;
	t_wall			*selected[MAXSELECTED]; //can be increased later
	t_walltoolmode	mode;
	t_floorcalc		fc;
	uint8_t			fc_debug;
}	t_walltooldata;

struct s_sdlcontext;
struct s_editor;
struct s_roomtooldata;

t_floorcalc	generate_floor(t_walltooldata *dat);
t_floorcalc	generate_floor_room(struct s_roomtooldata dat);
void		makefloor(struct s_editor *ed, struct s_roomtooldata dat, struct s_sdlcontext *sdl);
void		floorcalc_debugdraw(struct s_editor *ed, struct s_sdlcontext *sdl, t_floorcalc fc, int tri_i, float rad);
void		render_snapgrid(struct s_editor *ed, struct s_sdlcontext *sdl, t_vector2 wallpos, bool shift, bool hover); //TODO: move somewhere

#endif