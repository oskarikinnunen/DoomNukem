/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2023/03/09 20:19:04 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"

static void	initialize_background_colors(t_font *font)
{
	font->background_colors.black = CLR_BLACK;
	font->background_colors.white = CLR_WHITE;
	font->background_colors.dark_grey = CLR_DARKGRAY;
	font->background_colors.light_grey = CLR_LIGHTGRAY;
	font->background_colors.brown = CLR_BROWN;
}

void	editor_load_fonts(t_sdlcontext *sdl)
{
	char	*font_path;
	int		ret;
	int		fd;
	int		i;
	int		j;
	int		font_size;
	int		size_count;

	doomlog(LOG_NORMAL, "LOADING FONTS");
	sdl->fontcount = count_asset_list(FONTLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){FONTLISTPATH, "size =", s_itoa(sdl->fontcount), NULL});
	sdl->fonts = ft_memalloc(sizeof(t_font) * sdl->fontcount);
	if (sdl->fonts == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->fonts");
	fd = fileopen(FONTLISTPATH, O_RDONLY);
	font_path = NULL;
	ret = get_next_line(fd, &font_path);
	i = 0;
	font_size = 8;
	sdl->fonts->size_count = 4;
	while (ret)
	{
		if (font_path)
		{
			j = 0;
			while (j < sdl->fonts->size_count)
			{
				sdl->fonts[i].sizes[j] = TTF_OpenFont(font_path, ((font_size * j) + font_size));
				if (!sdl->fonts[i].sizes[j])
					doomlog(LOG_EC_TTF_OPENFONT, NULL);
				j++;
			}
			ft_strcpy(sdl->fonts[i].name, extract_filename(font_path));
			initialize_background_colors(&sdl->fonts[i]);
			sdl->fonts[i].color = color32_to_sdlcolor(AMBER_2);
			sdl->fonts[i].box_color = sdl->fonts[i].background_colors.black;
			sdl->fonts[i].size_default = sdl->fonts[i].sizes[0];
			free(font_path);
			font_path = NULL;
		}
		i++;
		ret = get_next_line(fd, &font_path);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, FONTLISTPATH);
	fileclose(fd, FONTLISTPATH);
	sdl->font_default = &sdl->fonts[0];
	doomlog_mul(LOG_NORMAL, (char *[4]){"parsed", s_itoa(i), "fontfiles", NULL});
	doomlog_mul(LOG_NORMAL, (char *[3]){"default font =", sdl->font_default->name, NULL});
}

/* void	playmode_load_fonts(t_sdlcontext *sdl)
{
	char	*font_path;
	int		ret;
	int		fd;
	int		i;
	int		j;
	int		font_size;
	int		size_count;

	doomlog(LOG_NORMAL, "LOADING FONTS");
	sdl->fontcount = count_asset_list(font_list);
	doomlog_mul(LOG_NORMAL, (char *[4]){font_list, "size =", s_itoa(sdl->fontcount), NULL});
	sdl->fonts = ft_memalloc(sizeof(t_font) * sdl->fontcount);
	if (sdl->fonts == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->fonts");
	fd = fileopen(font_list, O_RDONLY);
	font_path = NULL;
	ret = get_next_line(fd, &font_path);
	i = 0;
	font_size = 8;
	sdl->fonts->size_count = 4;
	while (ret)
	{
		if (font_path)
		{
			j = 0;
			while (j < sdl->fonts->size_count)
			{
				sdl->fonts[i].sizes[j] = TTF_OpenFont(font_path, ((font_size * j) + font_size));
				if (!sdl->fonts[i].sizes[j])
					doomlog(LOG_EC_TTF_OPENFONT, NULL);
				j++;
			}
			ft_strcpy(sdl->fonts[i].name, extract_filename(font_path));
			initialize_background_colors(&sdl->fonts[i]);
			sdl->fonts[i].color = color32_to_sdlcolor(AMBER_2);
			sdl->fonts[i].box_color = sdl->fonts[i].background_colors.black;
			sdl->fonts[i].size_default = sdl->fonts[i].sizes[0];
			free(font_path);
			font_path = NULL;
		}
		i++;
		ret = get_next_line(fd, &font_path);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, font_list);
	fileclose(fd, font_list);
	sdl->font_default = &sdl->fonts[0];
	doomlog_mul(LOG_NORMAL, (char *[4]){"parsed", s_itoa(i), "fontfiles", NULL});
	doomlog_mul(LOG_NORMAL, (char *[3]){"default font =", sdl->font_default->name, NULL});
}
 */