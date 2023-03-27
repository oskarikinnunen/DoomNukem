/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walltool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:17:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:43:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WALLTOOL_H
# define WALLTOOL_H

#include "room.h"

typedef	enum e_walltoolmode
{
	place_first,
	place_other,
	place_height
}	t_walltoolmode;

#define MAXSELECTED 32

typedef struct s_ear
{
	t_vector2	first;
	t_vector2	center;
	t_vector2	second;
}	t_ear;

typedef struct s_floorcalc
{
	t_vector2	edges[1024];
	t_face		faces[1024];
	t_line		normals[1024];
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
struct s_room;

void		makefloor(struct s_editor *ed, struct s_roomtooldata dat, struct s_sdlcontext *sdl);
//void		makefloor_room(struct s_editor *ed, struct s_sdlcontext *sdl, struct s_room *room, int debug);

#endif