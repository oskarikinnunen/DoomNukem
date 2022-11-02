/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:15:00 by raho              #+#    #+#             */
/*   Updated: 2022/11/02 20:55:05 by raho             ###   ########.fr       */
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

float	scale_font_pixel(float value, float oldmin, float oldmax, float newmin, float newmax)
{
	float	temp;
	float	result;

	temp = (newmax - newmin) * (value - oldmin);
	result = (temp / (oldmax - oldmin)) + newmin;
	return (result);
}

void	draw_text(t_sdlcontext *sdl, const char *str, t_point pos, int font_size)
{
	int			i;
	int			j;
	t_point		cursor;
	t_point		counter;
	t_point		screen;
	int			text_width;

	// Avoids infinite loop when splitting the next character to the next line
	/*
	if (max_width < 23)
		max_width = 23;
	*/

	// Calculates the width of the text to be drawed
	/*
	text_width = 0;
	i = 0;
	while (str[i] != '\0')
	{
		j = str[i] - 32;
		if (str[i + 1] == '\0')
		{
			text_width += sdl->font->chars[j].offset.x;
			text_width += sdl->font->chars[j].size.x;
		}
		else
			text_width += sdl->font->chars[j].xadvance;
		i++;
	}
	if (pos.x + text_width > sdl->window_w)
		pos.y += 100;
	*/

	cursor.x = 0;
	cursor.y = 0;
	i = 0;
	while (str[i] != '\0')
	{
		j = str[i] - 32;

		// Splits the next character to the next line
		/*
		if (cursor.x + sdl->font->chars[j].offset.x + sdl->font->chars[j].size.x > max_width)
		{
			cursor.x = 0;
			cursor.y += sdl->font->line_height;
		}
		*/
		
		cursor.x += sdl->font[font_size].chars[j].offset.x;
		cursor.y += sdl->font[font_size].chars[j].offset.y;
		counter.y = 0;
		while (counter.y < sdl->font[font_size].chars[j].size.y)
		{
			screen.y = cursor.y + counter.y;
			counter.x = 0;
			while (counter.x < sdl->font[font_size].chars[j].size.x)
			{
				screen.x = cursor.x + counter.x;
				draw(*sdl, point_add(screen, pos), sdl->font[font_size].bitmap->data[(sdl->font[font_size].chars[j].pos.x + counter.x) + ((sdl->font[font_size].chars[j].pos.y + counter.y) * sdl->font[font_size].bitmap->size.x)]);
				counter.x++;
			}
			counter.y++;
		}
		cursor.x -= sdl->font[font_size].chars[j].offset.x;
		cursor.y -= sdl->font[font_size].chars[j].offset.y;
		cursor.x += sdl->font[font_size].chars[j].xadvance;
		i++;
	}
}
