/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_set_up.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:47:48 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:11:50 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void	gui_start(t_autogui *gui)
{
	t_rectangle	temp;

	if (gui->sdl == NULL)
		return ;
	if (!gui->hidden)
	{
		draw_rectangle_raster(*gui->sdl, gui->rect, 1);
		draw_rectangle(*gui->sdl, gui->rect, AMBER_0);
		temp.position = point_sub(gui->rect.position, point_one());
		temp.size = point_add(gui->rect.size, (t_point){2, 2});
		draw_rectangle(*gui->sdl, temp, AMBER_1);
		temp.position = point_sub(temp.position, point_one());
		temp.size = point_add(temp.size, (t_point){2, 2});
		draw_rectangle(*gui->sdl, temp, AMBER_2);
	}
	gui->overdraw = 0;
	gui->offset.y = TITLE_BAR_HEIGHT + 5;
	gui->offset.x = 5;
	if (gui->rect.size.x < gui->minimum_size.x)
		gui->rect.size.x = gui->minimum_size.x;
	if (gui->rect.size.y < gui->minimum_size.y)
		gui->rect.size.y = gui->minimum_size.y;
	gui->x_max_drawn = 0;
}

t_autogui	init_gui(t_gui_init gui_init)
{
	t_autogui	gui;

	ft_bzero(&gui, sizeof(t_autogui));
	gui.sdl = gui_init.sdl;
	gui.hid = gui_init.hid;
	gui.player = gui_init.player;
	gui.rect.position = gui_init.origin;
	gui.rect.size = (t_point){240, 400};
	gui.title = gui_init.title;
	gui.minimum_size.x = 240;
	gui.minimum_size.y = 128;
	return (gui);
}
