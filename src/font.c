/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2023/03/07 16:22:01 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	initialize_background_colors(t_font *font)
{
	font->background_colors.black = CLR_BLACK;
	font->background_colors.white = CLR_WHITE;
	font->background_colors.dark_grey = CLR_DARKGRAY;
	font->background_colors.light_grey = CLR_LIGHTGRAY;
	font->background_colors.brown = CLR_BROWN;
}

// last argument in TTF_OpenFont is the wanted font size
void	editor_load_fonts(t_font *font)
{
	const char	font_file[] = "assets/fonts/Apple2.ttf";
	char		text[5];

	font->font_sizes[0] = TTF_OpenFont(font_file, 8);
	if (!font->font_sizes[0])
		doomlog(LOG_EC_TTF_OPENFONT, NULL);
	font->font_sizes[1] = TTF_OpenFont(font_file, 30);
	if (!font->font_sizes[1])
		doomlog(LOG_EC_TTF_OPENFONT, NULL);
	font->font_sizes[2] = TTF_OpenFont(font_file, 35);
	if (!font->font_sizes[2])
		doomlog(LOG_EC_TTF_OPENFONT, NULL);
	font->font_sizes[3] = TTF_OpenFont(font_file, 100);
	if (!font->font_sizes[3])
		doomlog(LOG_EC_TTF_OPENFONT, NULL);
	font->font = font->font_sizes[0];
	initialize_background_colors(font);
	font->color = color32_to_sdlcolor(AMBER_2);
	font->box_color = font->background_colors.black;
	font->text = ft_strcpy(text, "text");
}

// last argument in TTF_OpenFont is the wanted font size
void	playmode_load_fonts(t_font *font)
{
	const char	font_file[] = "assets/fonts/Apple2.ttf";
	char		text[5];

	font->font_sizes[0] = TTF_OpenFont(font_file, 8);
	if (!font->font_sizes[0])
		doomlog(LOG_EC_TTF_OPENFONT, NULL);
	font->font_sizes[1] = TTF_OpenFont(font_file, 30);
	if (!font->font_sizes[1])
		doomlog(LOG_EC_TTF_OPENFONT, NULL);
	font->font_sizes[2] = TTF_OpenFont(font_file, 35);
	if (!font->font_sizes[2])
		doomlog(LOG_EC_TTF_OPENFONT, NULL);
	font->font_sizes[3] = TTF_OpenFont(font_file, 100);
	if (!font->font_sizes[3])
		doomlog(LOG_EC_TTF_OPENFONT, NULL);
	font->font = font->font_sizes[0];
	initialize_background_colors(font);
	font->color = color32_to_sdlcolor(AMBER_2);
	font->box_color = font->background_colors.black;
	font->text = ft_strcpy(text, "text");
}
