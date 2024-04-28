/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_tools1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:54:09 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:15:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void	gui_int(int i, t_autogui *gui)
{
	char		*str;
	t_rectangle	rect;

	rect = empty_rect();
	if (gui_should_draw(gui))
	{
		str = ft_itoa(i);
		rect = print_text_colored(gui->sdl, str, gui_current_pos(gui), AMBER_3);
		free(str);
	}
	gui_layout(gui, rect);
}

void	gui_point(t_point point, t_autogui *gui)
{
	gui_start_horizontal(gui);
	gui_int(point.x, gui);
	gui_int(point.y, gui);
	gui_end_horizontal(gui);
}

void	gui_lock_mouse(t_autogui *gui)
{
	t_point	mousepos;

	mousepos = gui->hid->mouse.pos;
	force_mouselock(gui->hid);
	gui->hid->mouse.pos = mousepos;
	gui->hid->mouse.dragging_ui = true;
}

t_point	gui_current_pos(t_autogui *gui)
{
	t_point	pos;

	pos = point_add(gui->rect.position, point_add(gui->scroll, gui->offset));
	pos = point_add(pos, (t_point){5, 0});
	return (pos);
}
