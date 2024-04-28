/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_anim_legend.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 20:09:06 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:23:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "assets.h"

static void	parse_frame_line(char *line, t_sdlcontext *sdl)
{
	static uint32_t	prev_frame = 0;
	uint32_t		frame;

	frame = ft_atoi(line);
	sdl->human_anims[sdl->human_anim_count].endframe = frame;
	sdl->human_anims[sdl->human_anim_count].start_frame = prev_frame;
	prev_frame = frame;
	sdl->human_anim_count++;
}

static void	parse_anim_name(char *line, t_sdlcontext *sdl)
{
	ft_bzero(&sdl->human_anims[sdl->human_anim_count], \
							sizeof(t_human_animation));
	ft_memcpy(sdl->human_anims[sdl->human_anim_count].name, \
							line, ft_strlen(line + 1));
}

static int	parse_lines(int temp_fd, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*line;

	i = 0;
	line = NULL;
	ret = get_next_line(temp_fd, &line);
	while (ret)
	{
		if (line)
		{
			if (i % 2 == 0)
				parse_anim_name(line, sdl);
			else
				parse_frame_line(line, sdl);
			i++;
			free(line);
			line = NULL;
		}
		ret = get_next_line(temp_fd, &line);
	}
	return (ret);
}

void	playmode_load_anim_legend(int level_fd, t_sdlcontext *sdl)
{
	int	temp_fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING ANIMATIONS");
	load_and_write_filecontent(level_fd, ANIMLEGENDPATH, TEMPANIMLEGEND);
	temp_fd = ft_fileopen(TEMPANIMLEGEND, O_RDONLY);
	sdl->human_anims = prot_memalloc(sizeof(t_human_animation) * 30);
	sdl->human_anim_count = 0;
	ret = parse_lines(temp_fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_anim_legend");
	ft_fileclose(temp_fd, ANIMLEGENDPATH);
}
