/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roomtool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:42:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/10 15:50:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOMTOOL_H
# define ROOMTOOL_H

#include "walltool.h"
#include "editor_tools.h"

//defines the current state of roomtool
typedef enum e_roomtoolmode
{
	rtm_none,
	rtm_create,
	rtm_modify,
	rtm_move,
	rtm_split,
	rtm_paint,
	rtm_connect
} t_roomtoolmode;

t_vector2	flipped_uv(t_vector2 og);

typedef struct s_roomtooldata
{
	t_roomtoolmode	rtm;
	t_autogui		maingui;
	t_autogui		modroom_gui;
	t_autogui		edge_gui;
	t_autogui		newroom_gui;
	t_autogui		paint_gui;
	t_raycastinfo	raycastinfo;
	t_room			*room;
	t_wall			wall;
	t_wall			*ed_wall;
	t_vector2		*held_edge;
	t_vector2		*temp_edges;
	uint32_t		temp_height;
	bool			room_should_recalculate;
	t_floorcalc		fc;
}	t_roomtooldata;

bool		isconnect(t_vector2 v2, t_room *room);
t_vector2	vector2_flipxy(t_vector2 vec);
void		highlight_room(t_editor *ed, t_sdlcontext *sdl, t_room *room, uint32_t color);
//void		highlight_roomwalls(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color);
t_meshtri	*selectedfloor(t_editor *ed, t_sdlcontext sdl, t_room *room);
t_wall		*selectedwall(t_editor *ed, t_sdlcontext sdl, t_room *room);
t_room		*selectedroom(t_editor *ed, t_sdlcontext sdl);


# endif