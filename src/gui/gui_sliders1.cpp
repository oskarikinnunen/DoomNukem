/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_sliders1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:38:47 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:15:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

bool	gui_labeled_float_slider(char *str, float *f, float mul, t_autogui *gui)
{
	bool	modified;

	modified = false;
	gui_start_horizontal(gui);
	gui_label(str, gui);
	modified = gui_float_slider(f, mul, gui);
	gui_end_horizontal(gui);
	return (modified);
}

void	gui_labeled_vector3_slider(
			char *str, t_vector3 *vec, float mul, t_autogui *gui)
{
	int	temp_min_x;

	temp_min_x = gui->min_x;
	gui_start_horizontal(gui);
	gui_label(str, gui);
	gui->min_x = 50;
	gui_float_slider(&vec->x, mul, gui);
	gui_float_slider(&vec->y, mul, gui);
	gui_float_slider(&vec->z, mul, gui);
	gui_end_horizontal(gui);
	gui->min_x = temp_min_x;
}

bool	gui_labeled_int_slider(char *str, int *i, float mul, t_autogui *gui)
{
	bool	modified;

	modified = false;
	gui_start_horizontal(gui);
	gui_label(str, gui);
	modified = gui_int_slider(i, mul, gui);
	gui_end_horizontal(gui);
	return (modified);
}

bool	gui_int_slider(int *i, float mul, t_autogui *gui)
{
	t_rectangle	rect;
	char		*str;
	t_point		mousepos;
	int			add;
	bool		modified;

	rect = empty_rect();
	add = 0;
	modified = false;
	if (gui_should_draw(gui))
	{
		rect = print_text_boxed(gui->sdl, s_itoa(*i), gui_current_pos(gui));
		draw_rectangle(*gui->sdl, rect, AMBER_0);
		if (collision_point_rectangle(gui->hid->mouse.pos, rect))
			modified = gui_int_slider_internal(i, mul, rect, gui);
	}
	gui_layout(gui, rect);
	return (modified);
}
