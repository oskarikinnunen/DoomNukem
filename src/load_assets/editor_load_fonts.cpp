/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_fonts.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 20:56:42 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "assets.h"

t_font	font_parse(char *file_name)
{
	t_font	font;
	int		i;

	ft_bzero(&font, sizeof(t_font));
	i = 0;
	while (i < FONT_SIZE_COUNT)
	{
		font.sizes[i] = TTF_OpenFont(file_name, \
							((FONT_SIZE_DEFAULT * i) + FONT_SIZE_DEFAULT));
		if (!font.sizes[i])
			doomlog(LOG_EC_TTF_OPENFONT, NULL);
		i++;
	}
	font.background_colors.black = CLR_BLACK;
	font.background_colors.white = CLR_WHITE;
	font.background_colors.dark_grey = CLR_DARKGRAY;
	font.background_colors.light_grey = CLR_LIGHTGRAY;
	font.background_colors.brown = CLR_BROWN;
	font.color = color32_to_sdlcolor(AMBER_2);
	font.box_color = font.background_colors.black;
	font.size_default = font.sizes[0];
	return (font);
}

static int	parse_font_list(int fd, t_sdlcontext *sdl)
{
	char	*font_path;
	int		ret;
	int		i;

	i = 0;
	font_path = NULL;
	ret = get_next_line(fd, &font_path);
	while (ret)
	{
		if (font_path)
		{
			sdl->fonts[i] = font_parse(font_path);
			if (sdl->fonts[i].sizes[0] != NULL)
				ft_strncpy_term(sdl->fonts[i].name, \
						extract_file_name(font_path), 120);
			doomlog_mul(LOG_NORMAL, (char *[3]){\
					"parsed .ttf file:", sdl->fonts[i].name, NULL});
			free(font_path);
			font_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &font_path);
	}
	return (ret);
}

void	editor_load_fonts(t_sdlcontext *sdl)
{
	int	ret;
	int	fd;

	doomlog(LOG_NORMAL, "LOADING FONTS");
	sdl->fontcount = count_asset_list(FONTLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			FONTLISTPATH, "size =", s_itoa(sdl->fontcount), NULL});
	sdl->fonts = prot_memalloc(sizeof(t_font) * sdl->fontcount);
	fd = ft_fileopen(FONTLISTPATH, O_RDONLY);
	ret = parse_font_list(fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, FONTLISTPATH);
	ft_fileclose(fd, FONTLISTPATH);
	sdl->font_default = &sdl->fonts[0];
}
