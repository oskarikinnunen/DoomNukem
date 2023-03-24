/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:47:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 22:04:40 by okinnune         ###   ########.fr       */
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

static void createmode_drawgui(t_editor *ed, t_roomtooldata *dat, t_sdlcontext *sdl)
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
	if (gui_roompresets(dat->room, gui, &ed->world))
		printf("after roompresets, edgecount is %i \n", dat->room->edgecount);
	gui_end(gui);
}

void	createmode(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_vector2		*edge;
	t_vector3		cursor;
	bool			placelast;

	cursor = createmode_raycast(ed, dat);
	if (dat->room->edgecount == 0)
		dat->room->height = cursor.z;
	createmode_drawgui(ed, dat, sdl);
	dat->room->ceiling_height = 50;
	placelast = ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1 && dat->room->edgecount > 1);
	if ((ed->hid.keystate >> KEYS_ENTERMASK) & 1)
	{
		dat->rtm = rtm_modify;
		dat->room = world_add_room(&ed->world, dat->room);
		return ;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		if (!placelast)
		{
			edge = &dat->room->edges[dat->room->edgecount];
			dat->room->edgecount++;
			*edge = v3tov2(cursor);
		}
		else
		{
			if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
			{
				dat->rtm = rtm_modify;
				world_add_room(&ed->world, dat->room);
				dat->room = list_findlast(ed->world.arealist);
				return ;
			}
		}
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		if (dat->room->edgecount >= 1)
			dat->room->edgecount--;
		else
		{
			free(dat->room);
			dat->room = NULL;
			dat->rtm = rtm_none;
			return;
		}
	}
	highlight_room(ed, sdl, dat->room, CLR_BLUE);
	if (is_joined(v3tov2(cursor), dat->room, &ed->world))
		render_circle(ed->world.sdl, cursor, 5, CLR_GREEN);
	else
		render_circle(ed->world.sdl, cursor, 5, CLR_BLUE);
}
