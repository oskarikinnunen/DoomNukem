/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_modify.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 17:48:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 11:45:35 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

static void	modifywallheights(t_area *room, int scrolldelta, t_world *world)
{
	int	i;

	i = 0;
	room->ceiling_height += scrolldelta * 10;
	room->ceiling_height = ft_clamp(room->ceiling_height, 0, 2500);
	room_init(room, world);
}

static void	modifymode_modifyheights(t_editor *ed, t_roomtooldata *dat)
{
	if (ed->hid.mouse.scroll_delta != 0)
	{
		if ((ed->hid.keystate >> KEYS_LALTMASK) & 1)
		{
			dat->room->height += ed->hid.mouse.scroll_delta * 10;
			dat->room->height = ft_clamp(dat->room->height, 0, 1000);
			room_init(dat->room, &ed->world);
			room_recalculate_joined_rooms(&ed->world, dat->room);
		}
		else
		{
			modifywallheights(dat->room,
				ed->hid.mouse.scroll_delta, &ed->world);
			room_recalculate_joined_rooms(&ed->world, dat->room);
		}
	}
}

static void	modifymode_gui(t_editor *ed, t_roomtooldata *dat)
{
	t_autogui	*gui;

	if (dat->room == NULL)
		return ;
	gui = &dat->modroom_gui;
	gui_start(gui);
	gui_starthorizontal(gui);
	gui_label("Modifying: ", gui);
	gui_label(dat->room->name, gui);
	gui_endhorizontal(gui);
	gui_labeled_bool("Legal: ", room_is_legal(&ed->world, dat->room), gui);
	gui_labeled_int("Edges: ", dat->room->edgecount, gui);
	gui_labeled_int("Walls: ", dat->room->wallcount, gui);
	if (gui_room_presets(dat->room, gui, &ed->world))
		room_init(dat->room, &ed->world);
	if (gui_shortcut_button("Delete", KEYS_DELETEMASK, gui))
	{
		room_tool_remove_room(ed, dat);
		gui_end(gui);
		return ;
	}
	gui_end(gui);
}

static t_vector3	modifymode_get_cursor(t_editor *ed, t_roomtooldata *dat)
{
	t_vector3		cursor;
	t_raycastinfo	planecastinfo;
	t_ray			plane_ray;

	cursor = vector3_zero();
	ft_bzero(&planecastinfo, sizeof(planecastinfo));
	plane_ray.origin = ed->player.headposition;
	plane_ray.dir = ed->player.lookdir;
	if (raycast_plane(plane_ray, &planecastinfo, dat->room->height))
	{
		cursor = planecastinfo.hit_pos;
		cursor.x = vector2_snap((t_vector2){cursor.x, cursor.y}, 10).x;
		cursor.y = vector2_snap((t_vector2){cursor.x, cursor.y}, 10).y;
	}
	return (cursor);
}

void	modifymode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	t_vector3		cursor;

	highlight_room(ed, &sdl, dat->room, AMBER_3);
	cursor = modifymode_get_cursor(ed, dat);
	modifymode_gui(ed, dat);
	if (dat->room == NULL)
		return ;
	room_tool_node(cursor, dat, ed);
	if (dat->held_edge == NULL)
		potential_node(cursor, dat, ed);
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		dat->room = NULL;
		dat->rtm = rtm_none;
		return ;
	}
	modifymode_modifyheights(ed, dat);
}
