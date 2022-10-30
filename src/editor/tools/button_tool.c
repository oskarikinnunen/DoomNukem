/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 15:40:24 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/30 18:28:42 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "inputhelp.h"

static t_guibutton *hovered(t_list *buttonlist, t_point mousepos)
{
	t_list		*l;
	t_guibutton	*button;

	l = buttonlist;
	while (l != NULL)
	{
		button = (t_guibutton *)l->content;
		if (pointrectanglecollision(mousepos, button->rect))
			return (button);
		l = l->next;
	}
	return (NULL);
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
		drawrectangle(sdl, tdat->selected->rect, CLR_GREEN);
}


void	modify_selected(t_editor *ed, t_buttontooldata *btd)
{
	t_imagedropdown	*dd;
	t_point			mouse_offset;
	t_guibutton		*select;
	static int		func_indexer;

	dd = &((t_buttontooldata *)ed->tool->tooldata)->dropdown;
	select = btd->selected;
	func_indexer += ed->mouse.scroll_delta;
	func_indexer = ft_clamp(func_indexer, 0, 5);
	if (dd->changed)
	{
		select->imageindex = dd->selected;
		dd->changed = false;
	}
	if (ed->mouse.scroll_delta != 0)
	{
		select->func_index = func_indexer;
		select->onclick = get_button_func(func_indexer).onclick;
		printf("on click changed to %s, index %i\n", get_button_func(func_indexer).func_name, select->func_index);
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
	if (tdat->selected != NULL)
		modify_selected(ed, tdat);
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
		tool.tooldata = ft_memalloc(sizeof(t_buttontooldata));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
	}
	return (&tool);
}