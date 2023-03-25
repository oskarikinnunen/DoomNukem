/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_sliders2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:44:04 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 15:42:04 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

bool	gui_float_slider(float	*f, float mul, t_autogui *gui)
{
	t_rectangle	rect;
	char		*str;
	t_point		mousepos;
	float		add;
	bool		modified;

	rect = empty_rect();
	add = 0.0f;
	modified = false;
	if (gui_shoulddraw(gui))
	{
		str = ft_ftoa(*f, 4);
		rect = print_text_boxed(gui->sdl, str, gui_currentpos(gui));
		draw_rectangle(*gui->sdl, rect, AMBER_0);
		if (collision_point_rectangle(gui->hid->mouse.pos, rect))
		{
			print_text_boxed(gui->sdl, "Drag to affect value", \
					point_add(gui->hid->mouse.pos, (t_point){40, -10}));
			draw_rectangle(*gui->sdl, rect, AMBER_1);
			if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
			{
				mousepos = gui->hid->mouse.pos;
				force_mouselock(gui->hid);
				gui->hid->mouse.pos = mousepos;
				gui->hid->mouse.dragging_ui = true;
			}
			if (gui->hid->mouse.held == MOUSE_LEFT)
			{
				add += (float)gui->hid->mouse.delta.x * mul;
				if (add != 0.0f)
					modified = true;
			}
			if ((gui->hid->keystate >> KEYS_SHIFTMASK) & 1)
				add *= 2.0f;
			if ((gui->hid->keystate >> KEYS_LALTMASK) & 1)
				add *= 0.5f;
			*f += add;
		}
		free(str);
	}
	gui_layout(gui, rect);
	return (modified);
}
