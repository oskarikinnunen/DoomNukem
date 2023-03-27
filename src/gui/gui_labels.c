/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_labels.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:37:47 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:14:54 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

void	gui_label(char *str, t_autogui *gui)
{
	t_rectangle	rect;

	rect = empty_rect();
	if (gui_should_draw(gui))
	{
		rect = print_text_colored(gui->sdl, str, gui_current_pos(gui), AMBER_3);
	}
	gui_layout(gui, rect);
}

bool	gui_hoverlabel(char *str, t_autogui *gui)
{
	t_rectangle	rect;
	bool		hovered;

	rect = empty_rect();
	hovered = false;
	if (gui_should_draw(gui))
	{
		rect = print_text_boxed(gui->sdl, str, gui_current_pos(gui));
		if (collision_point_rectangle(gui->hid->mouse.pos, rect))
			hovered = true;
	}
	gui_layout(gui, rect);
	return (hovered);
}
