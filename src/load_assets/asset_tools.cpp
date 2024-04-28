/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:55:40 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "assets.h"

int	count_asset_list(char *filename)
{
	char	*line;
	int		ret;
	int		fd;
	int		i;

	if (filename == NULL)
		return (0);
	fd = ft_fileopen(filename, O_RDONLY);
	line = NULL;
	ret = get_next_line(fd, &line);
	i = 0;
	while (ret)
	{
		i++;
		if (line)
		{
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, filename);
	ft_fileclose(fd, filename);
	return (i);
}

void	log_loaded_animations(t_sdlcontext *sdl)
{
	int	i;

	doomlog_mul(LOG_NORMAL, (char *[4]){\
		"loaded", s_itoa(sdl->human_anim_count), "animations:", NULL});
	i = 0;
	while (i < sdl->human_anim_count)
	{
		doomlog_mul(LOG_NORMAL, (char *[2]){\
			sdl->human_anims[i].name, NULL});
		i++;
	}
}
