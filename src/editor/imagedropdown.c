/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imagedropdown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 16:21:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/30 17:33:00 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "doomnukem.h"

void	draw_imagedropdown(t_sdlcontext sdl, t_imagedropdown dd)
{
	int			i;
	int			img_index;
	t_guibutton	butn;

	if (!dd.inprogress)
		return ;
	i = 0;
	img_index = dd.scroll;
	while (i < 3)
	{
		butn = dd.buttons[i];
		draw_image(sdl, butn.rect.position, *get_image_by_index(sdl, img_index), butn.rect.size);
		i++;
		img_index++;
	}
}

void	start_imagedropdown(t_point origin, t_imagedropdown *dd)
{
	int			i;
	t_rectangle	i_rect;
	
	i = 0;
	dd->inprogress = true;
	dd->origin = origin;
	dd->scroll = 0;
	i_rect.position = dd->origin;
	i_rect.size = (t_point){42, 42};
	while (i < 3)
	{
		dd->buttons[i].rect.position = i_rect.position;
		dd->buttons[i].rect.size = i_rect.size;
		i_rect.position = point_add(i_rect.position, (t_point){0, i_rect.size.y});
		i++;
	}
}

void	update_imagedropdown(t_editor *ed, t_imagedropdown *dd)
{
	int	i;

	if (!dd->inprogress)
		return ;
	i = 0;
	while (i < 3)
	{
		if (ed->mouse.click_unhandled && ed->mouse.click_button == MOUSE_LEFT)
		{
			if (pointrectanglecollision(ed->mouse.pos, dd->buttons[i].rect))
			{
				dd->selected = i + dd->scroll;
				printf("SELECTED IMAGE %i \n", i + dd->scroll);
				dd->changed = true;
				dd->inprogress = false;
				break ;
			}
		}
		i++;
	}
	dd->scroll += ed->mouse.scroll_delta;
	dd->scroll = ft_clamp(dd->scroll, 0, 256);
}