/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/12/01 20:05:47 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	initialize_font_colors(t_font *font)
{
	SDL_Color	temp;

	temp.a = 255;
	temp.r = 0;
	temp.g = 0;
	temp.b = 0;
	font->font_colors.black = temp;
	temp.r = 255;
	font->font_colors.red = temp;
	temp.r = 99;
	font->font_colors.crimson = temp;
	temp.r = 0;
	temp.g = 255;
	font->font_colors.green = temp;
	temp.g = 0;
	temp.b = 255;
	font->font_colors.blue = temp;
	temp.r = 255;
	temp.g = 255;
	font->font_colors.white = temp;
	temp.b = 0;
	font->font_colors.yellow = temp;
	temp.g = 165;
	font->font_colors.orange = temp;
	temp.r = 135;
	temp.g = 206;
	temp.b = 235;
	font->font_colors.skyblue = temp;
}

static void	initialize_background_colors(t_font *font)
{
	font->background_colors.black = 0xFF000000;
	font->background_colors.white = 0xFFFFFFFF;
	font->background_colors.dark_grey = 0xFF222222;
	font->background_colors.light_grey = 0xFF555555;
	font->background_colors.brown = 0xFF4C3228;
}

void	load_fonts(t_font *font)
{
	const char	font_file[] = "assets/fonts/SchoonSquare.ttf";
	char		text[5];

	font->font_sizes[0] = TTF_OpenFont(font_file, 25); // last argument is font size
	if (!font->font_sizes[0])
		error_log(EC_TTF_OPENFONT);
	font->font_sizes[1] = TTF_OpenFont(font_file, 30);
	if (!font->font_sizes[1])
		error_log(EC_TTF_OPENFONT);
	font->font_sizes[2] = TTF_OpenFont(font_file, 35);
	if (!font->font_sizes[2])
		error_log(EC_TTF_OPENFONT);
	font->font_sizes[3] = TTF_OpenFont(font_file, 100);
	if (!font->font_sizes[3])
		error_log(EC_TTF_OPENFONT);
	font->font = font->font_sizes[0]; // initialize the font pointer to the smallest font
	initialize_font_colors(font);
	initialize_background_colors(font);
	font->color = font->font_colors.black; // initialize the font's color
	font->box_color = font->background_colors.dark_grey; // initialize the print_text_boxed background color
	font->text = ft_strcpy(text, "text"); // initialize text if it's used before being set to something specific
}
