/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_sliders2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:44:04 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 22:26:19 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

static void	mouselock_gui(t_autogui *gui)
{
	force_mouselock(gui->hid);
	gui->hid->mouse.dragging_ui = true;
}

static void	if_clicked_lock_mouse(t_autogui *gui)
{
	t_point	mousepos;

	if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
	{
		mousepos = gui->hid->mouse.pos;
		force_mouselock(gui->hid);
		gui->hid->mouse.pos = mousepos;
		gui->hid->mouse.dragging_ui = true;
	}
}

static bool	drag_slider(t_autogui *gui, float *f, float mul, t_rectangle rect)
{
	float	add;
	bool	modified;

	add = 0.0f;
	draw_rectangle(*gui->sdl, rect, AMBER_1);
	if_clicked_lock_mouse(gui);
	if (gui->hid->mouse.held == MOUSE_LEFT)
	{
		add += (float)gui->hid->mouse.delta.x * mul;
		modified = (add != 0.0f);
	}
	*f += add;
	return (modified);
}

bool	gui_float_slider(float	*f, float mul, t_autogui *gui)
{
	t_rectangle	rect;
	char		*str;
	float		add;
	bool		modified;

	rect = empty_rect();
	add = 0.0f;
	modified = false;
	if (gui_should_draw(gui))
	{
		str = ft_ftoa(*f, 4);
		rect = print_text_boxed(gui->sdl, str, gui_current_pos(gui));
		draw_rectangle(*gui->sdl, rect, AMBER_0);
		if (collision_point_rectangle(gui->hid->mouse.pos, rect))
			modified = drag_slider(gui, f, mul, rect);
		free(str);
	}
	gui_layout(gui, rect);
	return (modified);
}
