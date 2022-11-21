/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/11/21 21:15:12 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	initialize_colors(t_ttfont *ttfont)
{
	SDL_Color	temp;

	temp.a = 255;
	temp.r = 0;
	temp.g = 0;
	temp.b = 0;
	ttfont->font_colors.black = temp;
	temp.r = 255;
	ttfont->font_colors.red = temp;
	temp.r = 99;
	ttfont->font_colors.crimson = temp;
	temp.r = 0;
	temp.g = 255;
	ttfont->font_colors.green = temp;
	temp.g = 0;
	temp.b = 255;
	ttfont->font_colors.blue = temp;
	temp.r = 255;
	temp.g = 255;
	ttfont->font_colors.white = temp;
}

void	load_ttfonts(t_ttfont *ttfont)
{
	const char	ttf_file[] = "assets/fonts/SchoonSquare.ttf";
	char		text[5];

	ttfont->font_sizes[0] = TTF_OpenFont(ttf_file, 25); // last argument is font size
	if (!ttfont->font_sizes[0])
		error_log(EC_TTF_OPENFONT);
	ttfont->font_sizes[1] = TTF_OpenFont(ttf_file, 30);
	if (!ttfont->font_sizes[1])
		error_log(EC_TTF_OPENFONT);
	ttfont->font_sizes[2] = TTF_OpenFont(ttf_file, 35);
	if (!ttfont->font_sizes[2])
		error_log(EC_TTF_OPENFONT);
	ttfont->font_sizes[3] = TTF_OpenFont(ttf_file, 40);
	if (!ttfont->font_sizes[3])
		error_log(EC_TTF_OPENFONT);
	ttfont->font = ttfont->font_sizes[0]; // initialize the font pointer to the smallest font
	initialize_colors(ttfont);
	ttfont->color = ttfont->font_colors.black; // initialize the font's color to crimson
	ttfont->text = ft_strcpy(text, "text"); // initialize text if it's used before being set to something specific
}
