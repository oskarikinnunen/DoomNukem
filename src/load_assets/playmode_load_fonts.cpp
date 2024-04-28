/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_fonts.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:48:10 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "assets.h"
#include "file_io.h"

static void	unpack_and_load_font(int font_i, int level_fd,
									char *font_name, t_sdlcontext *sdl)
{
	load_and_write_filecontent(level_fd, font_name, TEMPFONT);
	sdl->fonts[font_i] = font_parse(TEMPFONT);
	if (sdl->fonts[font_i].sizes[0] != NULL)
		ft_strncpy_term(sdl->fonts[font_i].name, \
				extract_file_name(font_name), 120);
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded .ttf file:", sdl->fonts[font_i].name, NULL});
}

static int	parse_font_list(int level_fd, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*font_name;
	int		temp_fd;

	i = 0;
	font_name = NULL;
	temp_fd = ft_fileopen(TEMPFONTLIST, O_RDONLY);
	ret = get_next_line(temp_fd, &font_name);
	while (ret)
	{
		if (font_name)
		{
			unpack_and_load_font(i, level_fd, font_name, sdl);
			free(font_name);
			font_name = NULL;
			i++;
		}
		ret = get_next_line(temp_fd, &font_name);
	}
	ft_fileclose(temp_fd, TEMPFONTLIST);
	return (ret);
}

void	playmode_load_fonts(int level_fd, t_sdlcontext *sdl)
{
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING FONTS");
	load_and_write_filecontent(level_fd, FONTLISTPATH, TEMPFONTLIST);
	sdl->fontcount = count_asset_list(TEMPFONTLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPFONTLIST, "size =", s_itoa(sdl->fontcount), NULL});
	sdl->fonts = prot_memalloc(sizeof(t_font) * sdl->fontcount);
	ret = parse_font_list(level_fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_fonts");
	sdl->font_default = &sdl->fonts[0];
}
