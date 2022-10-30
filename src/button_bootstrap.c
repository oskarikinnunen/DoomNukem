/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_bootstrap.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 18:31:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/30 18:15:26 by okinnune         ###   ########.fr       */
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

//int get_dropdown_result()
/*
t_guibutton *hovered(t_list *buttonlist, t_point mousepos)
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

void	buttoncreator(t_editor *ed, t_sdlcontext sdl) //Make this a tool!
{
	static t_guibutton		button =
	{
		0, 0, 0, 0, NULL, "linetool.png", empty_click_func
	};
	t_guibutton				*hover;
	static t_imagedropdown	img_dd;

	hover = hovered(ed->buttonlist, ed->mouse.pos);
	if (img_dd.inprogress)
	{
		//update dropdown; draw it, update it's selection (read mouse position etc)
	}
	drawrectangle(sdl, button.rect, CLR_GREEN);
	draw_buttons(ed->buttonlist, sdl);
	if (hover != NULL)
	{
		//modify_hover(ed, sdl, hover);
		return ;
	}
	if (ed->mouse.held == MOUSE_LEFT && point_cmp(button.rect.position, point_zero()))
	{
		button.img = get_image_by_name(sdl, button.imagename);
		button.rect.position = ed->mouse.pos;
	}
		
	if (ed->mouse.held == MOUSE_LEFT && !point_cmp(button.rect.position, point_zero()))
		button.rect.size = point_abs(point_sub(ed->mouse.pos, button.rect.position));
	if (ed->mouse.held == 0 && !point_cmp(button.rect.position, point_zero()) && !point_cmp(button.rect.size, point_zero()))
	{
		list_push(&ed->buttonlist, &button, sizeof(t_guibutton));
		ft_bzero(&button, sizeof(t_guibutton));
	}
}
*/