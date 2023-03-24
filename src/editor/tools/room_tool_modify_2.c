/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_modify_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 17:52:12 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 17:56:39 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

t_areastatus	room_get_areastatus(t_area area)
{
	t_areastatus	status;

	status.is_floor \
		= area.floor_enabled && !area.ceiling_enabled
		&& !area.walls_enabled && area.loop;
	status.is_exterior \
		= !area.floor_enabled && !area.ceiling_enabled
		&& area.walls_enabled;
	status.is_room \
		= area.floor_enabled && area.ceiling_enabled
		&& area.walls_enabled && area.loop;
	return (status);
}

bool	gui_roompresets(t_area *room, t_autogui *gui, t_world *world)
{
	t_areastatus	status;

	status = room_get_areastatus(*room);
	if (gui_highlighted_button_if("Floor", gui, status.is_floor))
	{
		room_setpreset_floor(room);
		return (true);
	}
	if (gui_highlighted_button_if("Exterior", gui, status.is_exterior))
	{
		room_setpreset_exterior(room);
		return (true);
	}
	if (status.is_exterior
		&& gui_highlighted_button_if("Loop", gui, room->loop))
	{
		room->loop = !room->loop;
		return (true);
	}
	if (gui_highlighted_button_if("Room", gui, status.is_room))
	{
		room_setpreset_room(room);
		return (true);
	}
	return (false);
}
