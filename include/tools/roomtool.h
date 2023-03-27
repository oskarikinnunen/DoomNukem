/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roomtool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:42:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 19:03:59 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOMTOOL_H
# define ROOMTOOL_H

# include "walltool.h"
# include "editor_tools.h"
# include "raycast.h"

//defines the current state of roomtool
typedef enum e_room_tool_mode
{
	rtm_none,
	rtm_create,
	rtm_modify,
	rtm_move,
	rtm_split,
	rtm_paint,
	rtm_connect
}	t_room_tool_mode;

t_vector2	flipped_uv(t_vector2 og);

typedef struct s_room_tool_data
{
	t_room_tool_mode	rtm;
	t_autogui			maingui;
	t_autogui			modroom_gui;
	t_autogui			edge_gui;
	t_autogui			newroom_gui;
	t_autogui			paint_gui;
	t_raycast_info		raycast_info;
	t_area				*room;
	t_vector2			*held_edge;
	t_vector2			*temp_edges;
	uint32_t			temp_height;
	bool				room_should_recalculate;
	t_floorcalc			fc;
}	t_room_tool_data;

void		room_tool_update(t_editor *ed, t_sdlcontext *sdl);
void		room_tool_remove_room(t_editor *ed, t_room_tool_data *dat);
bool		isconnect(t_vector2 v2, t_area *room);
t_vector2	vector2_flipxy(t_vector2 vec);
void		highlight_room(t_editor *ed, t_sdlcontext *sdl, \
							t_area *room, uint32_t color);
t_meshtri	*selected_floor(t_editor *ed, t_sdlcontext sdl, t_area *room);
t_wall		*selected_wall(t_editor *ed, t_sdlcontext sdl, t_area *room);
t_area		*selecte_droom(t_editor *ed, t_sdlcontext sdl);
t_vector2	next_edge(t_area *room, int i);

bool		is_joined(t_vector2 edge, t_area	*room, t_world *world);

/*ROOM_TOOL_RAYCAST.C*/
void		room_tool_raycast(t_editor *ed, t_room_tool_data *dat);

/*ROOM_TOOL_PAINT.C*/
void		paint_room(t_entity *hit_ent, t_img *tex, t_world *world);
void		room_tool_paint(t_editor *ed, t_sdlcontext *sdl, \
							t_room_tool_data *dat);

/* ROOM_TOOL_ADDNODE.C */
bool		potential_node(t_vector3 cursor, t_room_tool_data *dat, \
							t_editor *ed);

/* ROOM_TOOL_NODE.C */
void		draw_node_indicator(t_vector3 cursor, t_room_tool_data *dat, \
								t_editor *ed);
void		room_tool_node(t_vector3 cursor, t_room_tool_data *dat, \
							t_editor *ed);
t_vector2	*closest_edge(t_area *room, t_vector3 rc);

//modifymode
void		modifymode(t_editor *ed, t_sdlcontext sdl, t_room_tool_data *dat);
bool		gui_room_presets(t_area *room, t_autogui *gui, t_world *world);

t_area		*get_raycast_room(t_raycast_info info, t_world *world);

void		recalculate_rooms_with_edge(t_editor *ed, t_vector2 edge);

void		remove_edge(t_world *world, t_area *room, t_vector2 *edge);

int			_room_find_edge_index(t_vector2 edge, t_area *room);
t_vector2	*_room_find_edge(t_vector2 edge, t_area *room);

void		create_mode(t_editor *ed, t_sdlcontext *sdl, \
						t_room_tool_data *dat);
int			next_index(int cur, t_area *room);

void		apply_edge_drag_solo(t_vector2 *edge, t_vector2 snap, \
									t_area *room, t_world *world);
void		apply_edge_drag(t_vector2 *edge, t_vector2 cursor, \
							t_area *room, t_world *world);
void		triangulate(t_floorcalc *fc);

#endif