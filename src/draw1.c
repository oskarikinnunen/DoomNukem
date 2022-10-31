/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:15:00 by raho              #+#    #+#             */
/*   Updated: 2022/10/31 22:15:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	draw_text(t_sdlcontext *sdl, t_img *text, t_point pos)
{
	t_point		pixel;
	uint32_t	color;

	pixel.y = 0;
	while (pixel.y < text->size.y)
	{
		pixel.x = 0;
		while (pixel.x < text->size.x)
		{
			color = text->data[pixel.x + (pixel.y * text->size.x)];
			draw(*sdl, point_add(pixel, pos), color);
			pixel.x++;
		}
		pixel.y++;
	}
}
