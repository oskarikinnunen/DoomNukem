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

void	draw_text(t_sdlcontext *sdl, const char *str, t_point pos, int font_size)
{
	int			i;
	int			j;
	int			k;
	t_point		cursor;
	t_point		counter;
	t_point		screen;
	int			word_length;

	if (pos.x < 5 || pos.x > sdl->window_w - 5 || pos.y < 5 || pos.y > sdl->window_h - 5)
	{
		printf("Trying to draw text outside of bounds. Text's starting position can't be closer than 5 pixels to the edges.\n");
		return ;
	}

	word_length = 0;
	cursor.x = 0;
	cursor.y = 0;
	i = 0;
	k = 0;

	while (str[i] != '\0')
	{
		// Calculate the next word's length
		if (str[i] != ' ' && k == 0)
		{
			word_length = 0;
			while (str[i + k] != '\0' && str[i + k] != ' ')
			{
				j = str[i + k] - 32;
				if (str[i + k + 1] == '\0')
					word_length += sdl->font[font_size].chars[j].xadvance;
				else
				{
					word_length += sdl->font[font_size].chars[j].offset.x;
					word_length += sdl->font[font_size].chars[j].size.x;
				}
				k++;
			}

			// Does the word fit the line? If not, move the cursor to the next line
			if (cursor.x + pos.x + word_length > sdl->window_w - 5) // Right now text shouldn't go closer to the edges than 5 pixels
			{
				cursor.x = 0;
				cursor.y += sdl->font[font_size].line_height;

				// Does the word fit now? If not, stop the function and print and error
				if (cursor.x + pos.x + word_length > sdl->window_w - 5) // Right now text shouldn't go closer to the edges than 5 pixels
				{
					printf("Not enough space widthwise to draw the text: %s\n", str);
					return ;
				}
			}
		}

		j = str[i] - 32;

		counter.y = 0;
		while (counter.y < sdl->font[font_size].chars[j].size.y)
		{
			screen.y = cursor.y + sdl->font[font_size].chars[j].offset.y + counter.y;
			counter.x = 0;
			while (counter.x < sdl->font[font_size].chars[j].size.x)
			{
				screen.x = cursor.x + sdl->font[font_size].chars[j].offset.x + counter.x;
				draw(*sdl, point_add(screen, pos), sdl->font[font_size].bitmap->data[(sdl->font[font_size].chars[j].pos.x + counter.x) + ((sdl->font[font_size].chars[j].pos.y + counter.y) * sdl->font[font_size].bitmap->size.x)]);
				counter.x++;
			}
			counter.y++;
		}

		cursor.x += sdl->font[font_size].chars[j].xadvance;
		if (k > 0)
			k--;
		i++;
	}
}
