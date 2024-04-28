/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_tools2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:56:01 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:15:41 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

bool	gui_should_draw(t_autogui *gui)
{
	t_point	cp;

	if (gui->hidden)
		return (false);
	cp = gui_current_pos(gui);
	if (cp.y > gui->rect.position.y + TITLE_BAR_HEIGHT && \
			cp.y + 20 < gui->rect.position.y + gui->rect.size.y)
		return (true);
	gui->scrollable = true;
	return (false);
}

void	gui_start_horizontal(t_autogui *gui)
{
	gui->agl = agl_horizontal;
	gui->offset.x = 0;
}

void	gui_end_horizontal(t_autogui *gui)
{
	gui->agl = agl_vertical;
	gui->offset.x = 0;
	gui->offset.y += 20;
}

void	gui_empty_vertical(int y, t_autogui *gui)
{
	t_rectangle	rect;

	rect.size.x = 0;
	rect.size.y = y;
	gui_layout(gui, rect);
}

void	gui_autosize(t_autogui *gui)
{
	t_rectangle	titlerect;
	t_point		pos;

	pos = point_add(gui->rect.position, (t_point){5, 5});
	titlerect = print_text_boxed(gui->sdl, gui->title, pos);
	gui->minimum_size.x = titlerect.size.x + 32;
	gui->rect.size.x = titlerect.size.x + 32;
}
