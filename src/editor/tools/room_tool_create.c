/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:47:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 11:45:29 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

static t_vector3	createmode_raycast(t_editor *ed, t_roomtooldata	*dat)
{
	t_vector3		result;
	t_ray			ray;
	t_raycastinfo	info;

	ray.origin = ed->player.headposition;
	ray.dir = ed->player.lookdir;
	if (dat->room->edgecount == 0)
	{	
		if (dat->raycastinfo.hit_entity != NULL)
			result = vector3_snap(dat->raycastinfo.hit_pos, 10);
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
		t_roomtooldata *dat, t_sdlcontext *sdl)
{
	t_autogui	*gui;

	gui = &dat->newroom_gui;
	gui_start(gui);
	if (dat->room->edgecount == 0)
		gui_label("Place first wall", gui);
	else if (dat->room->edgecount > 1)
		gui_label("Shift + click to finish room", gui);
	else
		gui_emptyvertical(20, gui);
	if (gui_room_presets(dat->room, gui, &ed->world))
		printf("after roompresets, edgecount is %i \n", dat->room->edgecount);
	gui_end(gui);
}

static void	createmode_step_back(t_editor *ed, t_sdlcontext *sdl,
		t_roomtooldata *dat)
{
	if (dat->room->edgecount >= 1)
		dat->room->edgecount--;
	else
	{
		free(dat->room);
		dat->room = NULL;
		dat->rtm = rtm_none;
		return ;
	}
}

static void	createmode_highlight(t_editor *ed, t_sdlcontext *sdl,
		t_roomtooldata *dat, t_vector3 cursor)
{
	highlight_room(ed, sdl, dat->room, CLR_BLUE);
	if (is_joined(v3tov2(cursor), dat->room, &ed->world))
		render_circle(ed->world.sdl, cursor, 5, CLR_GREEN);
	else
		render_circle(ed->world.sdl, cursor, 5, CLR_BLUE);
}

void	createmode(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_vector3		cursor;

	cursor = createmode_raycast(ed, dat);
	if (dat->room->edgecount == 0)
		dat->room->height = cursor.z;
	createmode_drawgui(ed, dat, sdl);
	dat->room->ceiling_height = 50;
	if ((ed->hid.keystate >> KEYS_ENTERMASK) & 1)
	{
		dat->rtm = rtm_modify;
		dat->room = world_add_room(&ed->world, dat->room);
		return ;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		dat->room->edges[dat->room->edgecount] = v3tov2(cursor);
		dat->room->edgecount++;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		createmode_step_back(ed, sdl, dat);
	createmode_highlight(ed, sdl, dat, cursor);
}
