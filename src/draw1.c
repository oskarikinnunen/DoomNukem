/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:15:00 by raho              #+#    #+#             */
/*   Updated: 2022/11/01 15:31:47 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	draw_saved_text(t_sdlcontext *sdl, t_img *text, t_point pos)
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

void	draw_text(t_sdlcontext *sdl, t_font *font, const char *str, t_point pos)
{
	int			i;
	int			j;
	t_point		cursor;
	t_point		counter;
	t_point		screen;

	cursor.x = 0;
	cursor.y = 0;
	i = 0;
	while (str[i] != '\0')
	{
		j = str[i] - 32;
		cursor.x += sdl->font->chars[j].offset.x;
		cursor.y += sdl->font->chars[j].offset.y;
		counter.y = 0;
		while (counter.y < sdl->font->chars[j].size.y)
		{
			screen.y = cursor.y + counter.y;
			counter.x = 0;
			while (counter.x < font->chars[j].size.x)
			{
				screen.x = cursor.x + counter.x;
				draw(*sdl, point_add(screen, pos), sdl->font->bitmap->data[(sdl->font->chars[j].pos.x + counter.x) + ((sdl->font->chars[j].pos.y + counter.y) * sdl->font->bitmap->size.x)]);
				counter.x++;
			}
			counter.y++;
		}
		cursor.x -= sdl->font->chars[j].offset.x;
		cursor.y -= sdl->font->chars[j].offset.y;
		cursor.x += sdl->font->chars[j].xadvance;
		i++;
	}
}
