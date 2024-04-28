/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_tools3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:58:44 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:14:54 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

void	gui_limit_rect(t_autogui *gui)
{
	gui->rect.size.x = ft_clamp(gui->rect.size.x, gui->minimum_size.x, 600);
	gui->rect.size.y = ft_clamp(gui->rect.size.y, gui->minimum_size.y, \
								gui->sdl->screensize.y);
	gui->rect.size.x = ft_max(gui->rect.size.x, gui->x_max_drawn);
	gui->rect.position.x = ft_clamp(gui->rect.position.x, 6, \
									gui->sdl->window_w - 6 - gui->rect.size.x);
	gui->rect.position.y = ft_clamp(gui->rect.position.y, 6, \
									gui->sdl->window_h - 6 - gui->rect.size.y);
}

bool	clicked_rectangle(t_mouse m, t_rectangle rect)
{
	return (mouse_clicked(m, MOUSE_LEFT) && \
			collision_point_rectangle(m.pos, rect));
}

void	update_hidecross(t_autogui *gui)
{
	t_rectangle	hidecross;
	t_point		p1;
	t_point		p2;

	hidecross.size = (t_point){32, 32};
	hidecross.position = point_add(gui->rect.position, \
									(t_point){.x = gui->rect.size.x});
	hidecross.position.x -= hidecross.size.x;
	draw_rectangle_filled(*gui->sdl, hidecross, AMBER_1);
	draw_rectangle(*gui->sdl, hidecross, AMBER_0);
	p1 = hidecross.position;
	p2 = point_add(hidecross.position, hidecross.size);
	drawline(*gui->sdl, p1, p2, AMBER_3);
	p1 = point_add(hidecross.position, (t_point){.x = hidecross.size.x});
	p2 = point_add(hidecross.position, (t_point){.y = hidecross.size.y});
	drawline(*gui->sdl, p1, p2, AMBER_3);
	drawline(*gui->sdl, hidecross.position, \
			point_add(hidecross.position, hidecross.size), AMBER_3);
	if (clicked_rectangle(gui->hid->mouse, hidecross))
		gui->hidden = true;
}

void	update_scrollbar(t_autogui *gui)
{
	t_rectangle	scrollrect;
	int			max_y;
	float		lerp;
	int			y_off;

	scrollrect.size = (t_point){16, 16};
	scrollrect.position = point_add(gui->rect.position, \
									(t_point){.x = gui->rect.size.x});
	scrollrect.position.y += 32;
	scrollrect.position.x -= scrollrect.size.x;
	max_y = gui->rect.size.y - 64 - scrollrect.size.x;
	lerp = 0.0f;
	if (-gui->scroll.y != 0)
		lerp = (float)-gui->scroll.y / (float)gui->overdraw;
	scrollrect.position.y += (float)max_y * lerp;
	draw_rectangle_filled(*gui->sdl, scrollrect, INT_MAX);
	draw_rectangle(*gui->sdl, scrollrect, AMBER_0);
	if (gui->hid->mouse.held == MOUSE_LEFT && \
			collision_point_rectangle(gui->hid->mouse.pos, scrollrect))
		gui->scroll_held = true;
	if (collision_point_rectangle(gui->hid->mouse.pos, scrollrect))
		draw_rectangle(*gui->sdl, scrollrect, AMBER_1);
	if (gui->scroll_held)
		gui->scroll.y += (scrollrect.position.y + (scrollrect.size.y / 2)) - \
							gui->hid->mouse.pos.y;
}

void	gui_layout_big(t_autogui *gui, t_rectangle rect)
{
	int	x;

	x = ft_max(rect.size.x + 10, gui->min_x);
	if (gui->agl == agl_vertical)
	{
		if (!gui_should_draw(gui))
		{
			gui->overdraw += 34;
		}
		gui->offset.y += 34;
	}
	else
		gui->offset.x += x;
	if (gui->offset.x > gui->x_max_drawn)
		gui->x_max_drawn = gui->offset.x;
}
