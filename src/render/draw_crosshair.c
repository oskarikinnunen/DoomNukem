/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_crosshair.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 22:03:23 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 22:05:25 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "vectors.h"

void	draw_crosshair(t_sdlcontext *sdl)
{
	drawcircle(*sdl, point_div(sdl->screensize, 2), 4, CLR_BLUE);
}
