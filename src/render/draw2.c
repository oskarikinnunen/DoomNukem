/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 22:03:23 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 11:58:08 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "vectors.h"

void	draw_crosshair(t_sdlcontext *sdl)
{
	drawcircle(*sdl, point_div(sdl->screensize, 2), 4, CLR_BLUE);
}

t_rectangle	empty_rect(void)
{
	return ((t_rectangle){0});
}
