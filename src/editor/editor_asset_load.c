/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_asset_load.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/21 13:17:03 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include <dirent.h>
#include "file_io.h"

// returns a pointer to the start of the file's name without its path
char	*extract_filename(const char *filepath)
{
	char	*filename;

	filename = ft_strrchr(filepath, '/');
	filename++;
	return (filename);
}

int	count_asset_list(char *filename)
{
	char	*line;
	int		ret;
	int		fd;
	int		i;

	if (filename == NULL)
		return (0);
	fd = fileopen(filename, O_RDONLY);
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
	fileclose(fd, filename);
	return (i);
}

void	editor_load_assets(t_sdlcontext *sdl)
{
	t_object	*human;
	int			i;

	editor_load_images(sdl);
	editor_load_env_textures(sdl);
	editor_load_objects(sdl);
	editor_load_fonts(sdl);
	editor_load_audio(&sdl->audio);
	objects_init(sdl);
	human = get_object_by_name(*sdl, "Human.obj");
	//editor_load_anims(human, "anim", 0);
	//editor_load_anim_legend(sdl);
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
