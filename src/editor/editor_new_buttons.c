/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/03 19:53:56 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

typedef void	(*t_button_click_fptr)(t_editor *ed);

void	empty_click_func(t_editor *ed)
{
	ed->mouse.click_unhandled = false;
}

void	set_tool_button(t_editor *ed)
{
	ed->tool = get_button_editor_tool();
	ed->mouse.click_unhandled = false;
}

void	set_tool_point(t_editor *ed)
{
	ed->tool = get_point_tool();
	ed->mouse.click_unhandled = false;
}

void	set_tool_wall(t_editor *ed)
{
	ed->tool = get_wall_tool();
	ed->mouse.click_unhandled = false;
}

void	set_tool_entity(t_editor *ed)
{
	ed->tool = get_entity_tool();
	ed->mouse.click_unhandled = false;
}

void	editor_angle_x_sub(t_editor *ed)
{
	//ed->lookangle.x -= ft_degtorad(45.0f);
}

void	editor_angle_x_add(t_editor *ed)
{
	//ed->lookangle.x += ft_degtorad(45.0f);
}

void	editor_angle_y_sub(t_editor *ed)
{
	if (ed->angle.y == -RAD90 * 0.99f)
	{
		ed->angle.y += ft_degtorad(45.0f);
		ed->forward_offset.y = -2.0f;
	}
	else
	{
		ed->angle.y = -RAD90 * 0.99f;
		ed->forward_offset.y = 0.0f;
	}
}

void	editor_angle_y_add(t_editor *ed)
{
	ed->forward_offset.y += 1.0f;
}

#include "assert.h"

t_click_func_def	get_button_func(int	index)
{
	static	t_click_func_def functions[256] = //add new button click functions here
	{
		{"empty_click", empty_click_func},
		{"set_tool_button", set_tool_button},
		{"set_tool_point", set_tool_point},
		{"set_tool_entity", set_tool_entity},
		{"set_tool_wall", set_tool_wall},
		{"editor_angle_x_sub", editor_angle_x_sub},
		{"editor_angle_x_add", editor_angle_x_add},
		{"editor_angle_y_sub", editor_angle_y_sub},
		{"editor_angle_y_add", editor_angle_y_add}
	};
	int	i;

	i = 0;
	while (i < 256)
	{
		if (functions[i].onclick == NULL)
			break ;
		if (i == index)
			return (functions[i]);
		i++;
	}
	return (functions[0]);
}

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

void draw_buttons(t_editor ed, t_sdlcontext sdl)
{
	t_list		*l;
	t_guibutton	button;
	
	l = ed.buttonlist;
	while (l != NULL)
	{
		button = *(t_guibutton *)l->content;
		if (button.img != NULL)
			draw_image(sdl, button.rect.position, *button.img, button.rect.size);
		if (pointrectanglecollision(ed.mouse.pos, button.rect))
		{
			drawrectangle(sdl, button.rect, CLR_GREEN);
		}
			
		l = l->next;
	}
}

void	initialize_buttons(t_list *buttonlist, t_sdlcontext sdl)
{
	t_list		*l;
	t_guibutton	*button;

	l = buttonlist;
	while (l != NULL)
	{
		button = (t_guibutton *)l->content;
		button->img = get_image_by_index(sdl, button->imageindex);
		button->onclick = get_button_func(button->func_index).onclick;
		button->rect.size.x = ft_clamp(button->rect.size.x, 8, 1024);
		button->rect.size.y = ft_clamp(button->rect.size.y, 8, 1024);
		l = l->next;
	}
}
