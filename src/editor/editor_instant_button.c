/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_instant_button.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 18:30:57 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/03 18:42:32 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shapes.h"
#include "doomnukem.h"
#include "inputhelp.h"

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