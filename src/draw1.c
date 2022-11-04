/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
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

void	draw_text(t_sdlcontext *sdl, const char *str, t_point pos, t_point boundaries)
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
					word_length += sdl->font->chars[j].xadvance;
				else
				{
					word_length += sdl->font->chars[j].offset.x;
					word_length += sdl->font->chars[j].size.x;
				}
				k++;
			}

			// Does the word fit the line? If not, move the cursor to the next line
			if (cursor.x + word_length > boundaries.x - 5) // Right now text shouldn't go closer to the edges than 5 pixels
			{
				cursor.x = 0;
				cursor.y += sdl->font->line_height;

				// Does the word fit now? If not, stop the function and print an error
				if (cursor.x + word_length > boundaries.x - 5) // Right now text shouldn't go closer to the edges than 5 pixels
				{
					printf("Not enough space widthwise to draw the text: %s\n", str);
					return ;
				}
				if (cursor.y > boundaries.y)
				{
					printf("Not enough space heightwise to draw the text: %s\n", str);
					return ;
				}
			}
		}

		j = str[i] - 32;
		counter.y = 0;
		while (counter.y < sdl->font->chars[j].size.y)
		{
			screen.y = cursor.y + sdl->font->chars[j].offset.y + counter.y;
			counter.x = 0;
			while (counter.x < sdl->font->chars[j].size.x)
			{
				screen.x = cursor.x + sdl->font->chars[j].offset.x + counter.x;
				draw(*sdl, point_add(screen, pos), sdl->font->bitmap->data[(sdl->font->chars[j].pos.x + counter.x) + ((sdl->font->chars[j].pos.y + counter.y) * sdl->font->bitmap->size.x)]);
				counter.x++;
			}
			counter.y++;
		}

		cursor.x += sdl->font->chars[j].xadvance;
		if (k > 0)
			k--;
		i++;
	}
}

static void	black_text_background(t_sdlcontext *sdl, t_point pos, t_point size)
{
	t_point	i;

	pos.x -= 3; // for borders because some letters start with as much as -3 offset
	pos.y -= 3; // for borders because some letters start with as much as -3 offset
	i.y = 0;
	while (i.y < size.y + 6) // + 6 for borders
	{
		i.x = 0;
		while (i.x < size.x + 6) // + 6 for borders
		{
			draw(*sdl, point_add(pos, i), 0);
			i.x++;
		}
		i.y++;
	}
}

void	draw_text_boxed(t_sdlcontext *sdl, const char *str, t_point pos, t_point boundaries)
{
	int			i;
	int			j;
	int			k;
	t_point		cursor;
	t_point		counter;
	t_point		screen;
	int			word_length;
	t_point		text_size;
	t_img		*background;

	if (pos.x < 5 || pos.x > sdl->window_w - 5 || pos.y < 5 || pos.y > sdl->window_h - 5)
	{
		printf("Trying to draw text outside of bounds. Text's starting position can't be closer than 5 pixels to the edges.\n");
		return ;
	}

	text_size.x = 0;
	text_size.y = 0;
	word_length = 0;
	cursor.x = 0;
	cursor.y = 0;
	i = 0;
	k = 0;

	// Calculate the size for the text's background
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
					word_length += sdl->font->chars[j].xadvance;
				else
				{
					word_length += sdl->font->chars[j].offset.x;
					word_length += sdl->font->chars[j].size.x;
				}
				k++;
			}

			// Does the word fit the line? If not, move the cursor to the next line
			if (cursor.x + word_length > boundaries.x - 5) // Right now text shouldn't go closer to the edges than 5 pixels
			{
				cursor.x = 0;
				cursor.y += sdl->font->line_height;

				// Does the word fit now? If not, stop the function and print an error
				if (cursor.x + word_length > boundaries.x - 5) // Right now text shouldn't go closer to the edges than 5 pixels
				{
					printf("Not enough space widthwise to draw the text: %s\n", str);
					return ;
				}
				if (cursor.y > boundaries.y)
				{
					printf("Not enough space heightwise to draw the text: %s\n", str);
					return ;
				}
			}
		}

		j = str[i] - 32;

		screen.x = cursor.x + sdl->font->chars[j].offset.x;
		screen.y = cursor.y + sdl->font->chars[j].offset.y;
		counter.y = 0;
		while (counter.y < sdl->font->chars[j].size.y)
		{
			screen.y = cursor.y + sdl->font->chars[j].offset.y + counter.y;
			if (screen.y > text_size.y)
				text_size.y = screen.y;
			counter.x = 0;
			while (counter.x < sdl->font->chars[j].size.x)
			{
				screen.x = cursor.x + sdl->font->chars[j].offset.x + counter.x;
				if (screen.x > text_size.x)
					text_size.x = screen.x;
				counter.x++;
			}
			counter.y++;
		}
		cursor.x += sdl->font->chars[j].xadvance;
		if (k > 0)
			k--;
		i++;
	}

	// Draw the background
	black_text_background(sdl, pos, text_size);
	// Draw the text
	draw_text(sdl, str, pos, boundaries);
}
