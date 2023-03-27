/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_end.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 15:05:22 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 22:34:45 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

static void	draw_dragcorner(t_autogui	*gui)
{
	t_rectangle	dragcorner;

	if (!gui->locked)
	{
		dragcorner.size = (t_point){16, 16};
		dragcorner.position = point_sub(point_add(gui->rect.position,
					gui->rect.size), dragcorner.size);
		draw_rect_tri(gui->sdl, dragcorner, AMBER_0);
		if (collision_point_rectangle(gui->hid->mouse.pos, dragcorner)
			|| gui->drag_held)
			draw_rect_tri(gui->sdl, dragcorner, AMBER_1);
		if (gui->hid->mouse.held == MOUSE_LEFT && \
collision_point_rectangle(gui->hid->mouse.pos, dragcorner) \
	&& !gui->hid->mouse.dragging_ui)
			gui->drag_held = true;
		if (gui->drag_held)
		{
			gui->scroll.y = 0;
			gui->rect.size = point_sub(point_add(gui->hid->mouse.pos,
						point_div(dragcorner.size, 2)), gui->rect.position);
		}
	}
}

static void	drag_release(t_autogui *gui)
{
	if (gui->hid->mouse.held == 0)
	{
		gui->move_held = false;
		gui->drag_held = false;
		if (gui->hid->mouse.dragging_ui)
		{
			gui->hid->mouse.dragging_ui = false;
			force_mouseunlock(gui->hid);
		}
		gui->scroll_held = false;
	}
}

static void	scroll(t_autogui *gui)
{
	if (gui->offset.y < gui->rect.size.y)
	{
		gui->scroll.y = 0;
		gui->scrollable = false;
		gui->rect.size.y = gui->offset.y + TITLE_BAR_HEIGHT;
	}
	if (gui->scrollable)
	{
		gui->scroll.y += gui->hid->mouse.scroll_delta * 16;
		update_scrollbar(gui);
		gui->scroll.y = ft_clamp(gui->scroll.y, -gui->overdraw, 0);
	}
}

static void	place(t_autogui *gui)
{
	if (gui->offset.y < gui->rect.size.y)
	{
		gui->scroll.y = 0;
		gui->scrollable = false;
		gui->rect.size.y = gui->offset.y + TITLE_BAR_HEIGHT;
	}
	if (gui->scrollable)
	{
		gui->scroll.y += gui->hid->mouse.scroll_delta * 16;
		update_scrollbar(gui);
		gui->scroll.y = ft_clamp(gui->scroll.y, -gui->overdraw, 0);
	}
}

void	gui_end(t_autogui *gui)
{
	t_rectangle	dragbar;

	if (gui->hidden || gui->sdl == NULL)
		return ;
	dragbar = gui->rect;
	dragbar.size.y = TITLE_BAR_HEIGHT;
	draw_rectangle_filled(*gui->sdl, dragbar, 1);
	print_text_colored(gui->sdl, gui->title,
		point_add(gui->rect.position, (t_point){5, 5}), AMBER_3);
	draw_dragcorner(gui);
	if (gui->move_held && !gui->locked)
			gui->rect.position = point_sub(gui->hid->mouse.pos,
				point_div(dragbar.size, 2));
	if (collision_point_rectangle(gui->hid->mouse.pos, dragbar)
		|| gui->move_held)
		draw_rectangle(*gui->sdl, dragbar, AMBER_1);
	if (gui->hid->mouse.held == MOUSE_LEFT
		&& collision_point_rectangle(gui->hid->mouse.pos, dragbar)
		&& !gui->hid->mouse.dragging_ui)
		gui->move_held = true;
	drag_release(gui);
	scroll(gui);
	place(gui);
	gui_limit_rect(gui);
}
