/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_instant_button.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 18:30:57 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/06 17:25:31 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shapes.h"
#include "doomnukem.h"
 
#include "editor_tools.h"

bool	instantbutton(t_rectangle rect, t_mouse *m, t_sdlcontext sdl, char *imgname)
{
	draw_image(sdl, rect.position, *get_image_by_name(sdl, imgname), rect.size);
	if (pointrectanglecollision(m->pos, rect))
	{
		drawrectangle(sdl, rect, CLR_GREEN);
		if (mouse_clicked(*m, MOUSE_LEFT))
		{
			m->click_unhandled = false;
			return (true);
		}
	}
	return (false);
}

bool	instant_text_button(t_sdlcontext sdl, t_mouse *m, const char *str, t_point pos)
{
	t_rectangle	rect;

	rect = print_text_boxed(&sdl, str, pos);
	drawrectangle(sdl, rect, AMBER_1);
	if (pointrectanglecollision(m->pos, rect))
	{
		drawrectangle(sdl, rect, AMBER_3);
		if (mouse_clicked(*m, MOUSE_LEFT))
		{
			m->click_unhandled = false;
			return (true);
		}
	}
	return (false);
}