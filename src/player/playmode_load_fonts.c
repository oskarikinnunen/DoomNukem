/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_fonts.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:48:10 by raho              #+#    #+#             */
/*   Updated: 2023/03/17 20:07:24 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"

static void	unpack_and_load_font(int font_i, char *font_name,
								t_sdlcontext *sdl)
{
	load_and_write_filecontent(LEVEL0FILE, font_name, TEMPFONT);
	sdl->fonts[font_i] = font_parse(TEMPFONT);
	if (sdl->fonts[font_i].sizes[0] != NULL)
		ft_strcpy(sdl->fonts[font_i].name, extract_filename(font_name));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded .ttf file:", sdl->fonts[font_i].name, NULL});
	remove(TEMPFONT);
}

static int	parse_font_list(int fd, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*font_name;

	i = 0;
	font_name = NULL;
	ret = get_next_line(fd, &font_name);
	while (ret)
	{
		if (font_name)
		{
			unpack_and_load_font(i, font_name, sdl);
			free(font_name);
			font_name = NULL;
			i++;
		}
		ret = get_next_line(fd, &font_name);
	}
	return (ret);
}

void	playmode_load_fonts(t_sdlcontext *sdl)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING FONTS");
	load_and_write_filecontent(LEVEL0FILE, FONTLISTPATH, TEMPFONTLIST);
	sdl->fontcount = count_asset_list(TEMPFONTLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPFONTLIST, "size =", s_itoa(sdl->fontcount), NULL});
	sdl->fonts = ft_memalloc(sizeof(t_font) * sdl->fontcount);
	if (sdl->fonts == NULL)
		doomlog(LOG_EC_MALLOC, "playmode_load_fonts");
	fd = fileopen(TEMPFONTLIST, O_RDONLY);
	ret = parse_font_list(fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_fonts");
	fileclose(fd, TEMPFONTLIST);
	sdl->font_default = &sdl->fonts[0];
	remove(TEMPFONTLIST);
}
