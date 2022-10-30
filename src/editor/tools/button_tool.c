/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 15:40:24 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/31 01:17:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "inputhelp.h"

t_guibutton	dragcorner(t_guibutton button)
{
	t_guibutton	dc;

	dc.rect.size = (t_point){12, 12};
	dc.rect.position = point_add(button.rect.position, button.rect.size);
	dc.rect.position = point_sub(dc.rect.position, dc.rect.size);
	return (dc);
}

static void draw_buttongrid(t_sdlcontext sdl)
{
	t_point	pen;

	pen = point_zero();
	while (pen.x < sdl.window_w)
	{
		drawline(sdl, (t_point){pen.x, 0}, (t_point){pen.x, sdl.window_h}, CLR_DARKGRAY);
		pen.x += 20;
	}
	while (pen.y < sdl.window_h)
	{
		drawline(sdl, (t_point){0, pen.y}, (t_point){sdl.window_w, pen.y}, CLR_DARKGRAY);
		pen.y += 20;
	}
}

static void draw_moveindicators(t_editor *ed, t_sdlcontext sdl)
{
	t_list		*l;
	t_guibutton	button;

	l = ed->buttonlist;
	while (l != NULL)
	{
		button = *(t_guibutton *)l->content;
		drawcircle(sdl, point_add(button.rect.position, point_div(button.rect.size, 2)), button.rect.size.x / 2, CLR_GREEN);
		l = l->next;
	}
	draw_buttongrid(sdl);
}

static void draw_resizeindicators(t_editor *ed, t_sdlcontext sdl)
{
	t_list		*l;
	t_guibutton	button;

	l = ed->buttonlist;
	while (l != NULL)
	{
		button = *(t_guibutton *)l->content;
		drawrectangle(sdl, dragcorner(button).rect, CLR_TURQ);
		l = l->next;
	}
	draw_buttongrid(sdl);
}

void	button_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_buttontooldata	*tdat;
	t_guibutton			button;
	t_imagedropdown		dd;

	tdat = (t_buttontooldata *)ed->tool->tooldata;
	dd = tdat->dropdown;
	draw_imagedropdown(sdl, dd);
	button = tdat->button;
	drawrectangle(sdl, button.rect, CLR_PRPL);
	if (tdat->selected != NULL)
	{
		drawrectangle(sdl, tdat->selected->rect, CLR_PRPL);
		if (tdat->selected->img != get_image_by_index(sdl, tdat->selected->imageindex))
			tdat->selected->img = get_image_by_index(sdl, tdat->selected->imageindex);
	}
	if ((ed->keystate >> KEYS_CTRLMASK) & 1)
		draw_moveindicators(ed, sdl);
	if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		draw_resizeindicators(ed, sdl);
}

static t_point snap_point(t_point point) //TODO: make a reusable version of this, and use for new walltool?
{
	t_point	result;

	result.x = point.x - (point.x % 20);
	result.y = point.y - (point.y % 20);
	return (result);
}

void	modify_selected(t_editor *ed, t_buttontooldata *btd)
{
	t_imagedropdown	*dd;
	t_point			mouse_offset;
	t_guibutton		*select;

	dd = &btd->dropdown;
	select = btd->selected;
	if (dd->changed)
	{
		select->imageindex = dd->selected;
		dd->changed = false;
	}
	if (ed->mouse.scroll_delta != 0 && !dd->inprogress)
	{
		//system("code -g src/editor/editor_new_buttons.c:50"); TODO: do this after we have text rendering, redo the function selector
		select->func_index += ed->mouse.scroll_delta;
		select->func_index = ft_clamp(select->func_index, 0, 5);
		select->onclick = get_button_func(select->func_index).onclick;
		printf("on click changed to %s, index %i\n",
			get_button_func(select->func_index).func_name, select->func_index);
	}
	if (ed->mouse.held == MOUSE_LEFT && !dd->inprogress)
	{
		if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		{
			select->rect.size = snap_point(point_sub(ed->mouse.pos, select->rect.position));
			select->rect.size.x = ft_clamp(select->rect.size.x, 20, 260);
			select->rect.size.y = ft_clamp(select->rect.size.y, 20, 260);
			select->rect.size.x -= 3;
			select->rect.size.y -= 3;
		}
		else if ((ed->keystate >> KEYS_CTRLMASK) & 1)
		{
			select->rect.position = snap_point(ed->mouse.pos);
		}
	}
	if (mouse_clicked(ed->mouse, MOUSE_MDL))
		start_imagedropdown(ed->mouse.pos, dd);
	if (mouse_clicked(ed->mouse, MOUSE_RIGHT))
	{
		list_remove(&ed->buttonlist, select, sizeof(t_guibutton));
		btd->selected = NULL;
	}
}

void	button_tool_update(t_editor *ed)
{
	t_buttontooldata		*tdat;
	t_guibutton				*button;
	t_guibutton				*hover;
	
	tdat = (t_buttontooldata *)ed->tool->tooldata;
	update_imagedropdown(ed, &tdat->dropdown);
	button = &tdat->button;
	hover = hovered(ed->buttonlist, ed->mouse.pos);
	if (tdat->selected != hover && hover != NULL)
		tdat->selected = hover;
	if ((mouse_clicked(ed->mouse, MOUSE_RIGHT) && !hover))
		tdat->selected = NULL;
	if (tdat->selected != NULL)
	{
		modify_selected(ed, tdat);
		return ;
	}
		
	if (tdat->dropdown.inprogress)
		return ;
	if (ed->mouse.held == MOUSE_LEFT && point_cmp(button->rect.position, point_zero()))
		button->rect.position = ed->mouse.pos;
	if (ed->mouse.held == MOUSE_LEFT && !point_cmp(button->rect.position, point_zero()))
	{
		button->rect.size = point_abs(point_sub(ed->mouse.pos, button->rect.position));
		button->rect.size.x = button->rect.size.x - (button->rect.size.x % 16);
		button->rect.size.y = button->rect.size.y - (button->rect.size.y % 16);
		//button->rect.size = point_div(button->rect.size, 4);
		//button->rect.size = point_mul(button->rect.size, 4);
	}
		
	if (ed->mouse.held == 0 && !point_cmp(button->rect.position, point_zero()) && !point_cmp(button->rect.size, point_zero()))
	{
		list_push(&ed->buttonlist, button, sizeof(t_guibutton));
		ft_bzero(button, sizeof(t_guibutton));
	}
}

t_tool	*get_button_editor_tool()
{
	static t_tool	tool =
	{
		button_tool_update, button_tool_draw
	};
	if (tool.tooldata == NULL)
	{
		ft_strcpy(tool.icon_name, "button.png");
		tool.tooldata = ft_memalloc(sizeof(t_buttontooldata));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
	}
	return (&tool);
}