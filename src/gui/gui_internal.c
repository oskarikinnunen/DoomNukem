/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_internal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:45:02 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 15:27:35 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

//Internal function, rename with a better name
void	gui_layout(t_autogui *gui, t_rectangle rect)
{
	int	x;

	x = ft_max(rect.size.x + 10, gui->min_x);
	if (gui->agl == agl_vertical)
	{
		if (!gui_shoulddraw(gui))
		{
			gui->overdraw += 20;
		}
		gui->offset.y += 20;
	}
	else
		gui->offset.x += x;
	if (gui->offset.x > gui->x_maxdrawn)
		gui->x_maxdrawn = gui->offset.x;
}

bool	gui_int_slider_internal(int *i, float mul,
								t_rectangle rect, t_autogui *gui)
{
	int		add;
	bool	modified;

	add = 0;
	modified = 0;
	print_text_boxed(gui->sdl, "Drag to affect value",
		point_add(gui->hid->mouse.pos, (t_point){40, -10}));
	draw_rectangle(*gui->sdl, rect, AMBER_1);
	if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
		gui_lock_mouse(gui);
	if (gui->hid->mouse.held == MOUSE_LEFT)
	{
		add = (float)gui->hid->mouse.delta.x;
		modified = (add != 0);
	}
	*i = *i + add;
	return (modified);
}

t_buttonreturn	autogui_internal_button(char *str, t_autogui *gui)
{
	t_buttonreturn	br;

	br.rect = print_text_boxed(gui->sdl, str, gui_currentpos(gui));
	draw_rectangle(*gui->sdl, br.rect, AMBER_0);
	br.clicked = false;
	if (collision_point_rectangle(gui->hid->mouse.pos, br.rect))
	{
		draw_rectangle(*gui->sdl, br.rect, AMBER_2);
		if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
		{
			br.clicked = true;
			gui->hid->mouse.click_unhandled = false;
		}
	}
	return (br);
}

t_buttonreturn	autogui_internal_colored_button(
							char *str, t_autogui *gui, uint32_t color)
{
	t_buttonreturn	br;

	br.rect = print_text_boxed(gui->sdl, str, gui_currentpos(gui));
	draw_rectangle(*gui->sdl, br.rect, AMBER_2);
	br.clicked = false;
	if (collision_point_rectangle(gui->hid->mouse.pos, br.rect))
	{
		draw_rectangle(*gui->sdl, br.rect, color);
		if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
		{
			br.clicked = true;
			gui->hid->mouse.click_unhandled = false;
		}
	}
	return (br);
}
