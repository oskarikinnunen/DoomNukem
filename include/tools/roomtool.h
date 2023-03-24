/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roomtool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:42:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 17:49:31 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOMTOOL_H
# define ROOMTOOL_H

#include "walltool.h"
#include "editor_tools.h"
#include "raycast.h"

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
	t_area			*room;
	t_vector2		*held_edge;
	t_vector2		*temp_edges;
	uint32_t		temp_height;
	bool			room_should_recalculate;
	t_floorcalc		fc;
}	t_roomtooldata;

void		room_tool_update(t_editor *ed, t_sdlcontext *sdl);

bool		isconnect(t_vector2 v2, t_area *room);
t_vector2	vector2_flipxy(t_vector2 vec);
void		highlight_room(t_editor *ed, t_sdlcontext *sdl, t_area *room, uint32_t color);
//void		highlight_roomwalls(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color);
t_meshtri	*selectedfloor(t_editor *ed, t_sdlcontext sdl, t_area *room);
t_wall		*selectedwall(t_editor *ed, t_sdlcontext sdl, t_area *room);
t_area		*selectedroom(t_editor *ed, t_sdlcontext sdl);

/*ROOM_TOOL_PAINT.C*/
void		room_tool_paint(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat);

/* ROOM_TOOL_ADDNODE.C */
bool	potentialnode(t_vector3 cursor, t_roomtooldata *dat, t_editor *ed);

/* ROOM_TOOL_NODE.C */
void		room_tool_node(t_vector3 cursor, t_roomtooldata *dat, t_editor *ed);
t_vector2	*closest_edge(t_area *room, t_vector3 rc);

//modifymode
void	modifymode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat);

t_area	*get_raycast_room(t_raycastinfo info, t_world *world);


# endif