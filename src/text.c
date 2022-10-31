/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_text.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/10/31 22:00:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

// With this you can test painting all the pixels in the font png to be some exact color apart from say the background

static void	paint_font(t_img *bitmap)
{
	t_point	i;

	i.y = 0;
	while (i.y < bitmap->size.y)
	{
		i.x = 0;
		while (i.x < bitmap->size.x)
		{
			if (bitmap->data[i.x + (i.y * bitmap->size.x)] != 0x000000) // testing black background
				bitmap->data[i.x + (i.y * bitmap->size.x)] = 0x00FF00;
			i.x++;
		}
		i.y++;
	}
}

// ~ Character is so thin and at the bottom of the picture that I think it takes some colors outside of the image

void	save_text(t_font *font, const char *str)
{
	t_img		*text;
	int			i;
	int			j;
	t_point		cursor;
	t_point		counter;
	int			text_width;
	static int	text_nbr = 0;

	//paint_font(font->bitmap); // for testing

	text = ft_memalloc(sizeof(t_img));
	if (!text)
		error_log(EC_MALLOC);

	// first calculating the amount of space needed by the text and then copying the characters to the image
	
	text_width = 0;
	i = 0;
	while (str[i] != '\0')
	{
		j = str[i] - 32;
		text_width += font->chars[j].xadvance;
		i++;
	}
	alloc_image(text, text_width + 10, font->line_height + 10); // 10 for some padding on every side of the text
	ft_strcpy(text->name, str);
	cursor.x = 5;
	cursor.y = 5;
	i = 0;
	while (str[i] != '\0')
	{
		j = str[i] - 32;
		cursor.x += font->chars[j].offset.x;
		cursor.y += font->chars[j].offset.y;
		counter.y = 0;
		while (counter.y < font->chars[j].size.y)
		{
			counter.x = 0;
			while (counter.x < font->chars[j].size.x)
			{
				//if (font->bitmap->data[(font->chars[j].pos.x + counter.x) + ((font->chars[j].pos.y + counter.y) * font->bitmap->size.x)] != 0x000000) // testing black background
				text->data[(cursor.x + counter.x) + ((cursor.y + counter.y) * text->size.x)] = font->bitmap->data[(font->chars[j].pos.x + counter.x) + ((font->chars[j].pos.y + counter.y) * font->bitmap->size.x)];
				counter.x++;
			}
			counter.y++;
		}
		cursor.x -= font->chars[j].offset.x;
		cursor.y -= font->chars[j].offset.y;
		cursor.x += font->chars[j].xadvance;
		i++;
	}
	font->texts[text_nbr] = text;
	text_nbr++;
}
