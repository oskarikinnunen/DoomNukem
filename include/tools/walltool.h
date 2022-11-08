/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walltool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:17:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/08 04:50:38 by okinnune         ###   ########.fr       */
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
	uint8_t		edgecount;
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

t_floorcalc	generate_floor(t_walltooldata *dat);
void		floorcalc_debugdraw(t_editor *ed, t_sdlcontext *sdl, t_floorcalc fc);

#endif