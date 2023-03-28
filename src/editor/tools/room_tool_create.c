/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:47:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 07:32:19 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

static t_vector3	createmode_raycast(t_editor *ed, t_room_tool_data	*dat)
{
	t_vector3		result;
	t_ray			ray;
	t_raycast_info	info;

	ray.origin = ed->player.head_position;
	ray.dir = ed->player.look_dir;
	if (dat->room->edge_count == 0)
	{	
		if (dat->raycast_info.hit_entity != NULL)
			result = vector3_snap(dat->raycast_info.hit_pos, 10);
		else
		{
			raycast_plane(ray, &info, 0.0f);
			result = vector3_snap(info.hit_pos, 10);
		}
	}
	else
	{
		raycast_plane(ray, &info, dat->room->height);
		result = vector3_snap(info.hit_pos, 10);
	}
	return (result);
}

static void	createmode_drawgui(t_editor *ed,
		t_room_tool_data *dat, t_sdlcontext *sdl)
{
	t_autogui	*gui;

	gui = &dat->newroom_gui;
	gui_start(gui);
	if (dat->room->edge_count == 0)
		gui_label("Place first wall", gui);
	else if (dat->room->edge_count > 1)
	{
		gui_label("[Enter] to finish room", gui);
		gui_start_horizontal(gui);
		gui_label("Edgecount: ", gui);
		gui_label(s_itoa(dat->room->edge_count), gui);
		gui_end_horizontal(gui);
	}
	else
		gui_empty_vertical(20, gui);
	if (gui_room_presets(dat->room, gui, &ed->world))
		;
	gui_end(gui);
}

static void	createmode_step_back(t_editor *ed, t_sdlcontext *sdl,
		t_room_tool_data *dat)
{
	if (dat->room->edge_count >= 1)
		dat->room->edge_count--;
	else
	{
		free(dat->room);
		dat->room = NULL;
		dat->rtm = rtm_none;
		return ;
	}
}

static void	createmode_highlight(t_editor *ed, t_sdlcontext *sdl,
		t_room_tool_data *dat, t_vector3 cursor)
{
	highlight_room(ed, sdl, dat->room, CLR_BLUE);
	if (is_joined(v3tov2(cursor), dat->room, &ed->world))
		render_circle(ed->world.sdl, cursor, 5, CLR_GREEN);
	else
		render_circle(ed->world.sdl, cursor, 5, CLR_BLUE);
}

void	create_mode(t_editor *ed, t_sdlcontext *sdl, t_room_tool_data *dat)
{
	t_vector3		cursor;

	cursor = createmode_raycast(ed, dat);
	if (dat->room->edge_count == 0)
		dat->room->height = cursor.z;
	createmode_drawgui(ed, dat, sdl);
	dat->room->ceiling_height = 50;
	if (((ed->hid.keystate >> KEYS_ENTERMASK) & 1))
	{
		dat->rtm = rtm_modify;
		dat->room = world_add_room(&ed->world, dat->room);
		return ;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT)
		&& dat->room->edge_count < 31)
	{
		dat->room->edges[dat->room->edge_count] = v3tov2(cursor);
		dat->room->edge_count++;
	}
	createmode_highlight(ed, sdl, dat, cursor);
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		createmode_step_back(ed, sdl, dat);
}
