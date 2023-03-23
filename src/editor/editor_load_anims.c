/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_anims.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:51:14 by raho              #+#    #+#             */
/*   Updated: 2023/03/23 15:20:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"

static int	parse_anim_list(int fd, char *anim_name, t_object *object)
{
	int		ret;
	char	*anim_path;

	anim_path = NULL;
	ret = get_next_line(fd, &anim_path);
	while (ret)
	{
		if (anim_path)
		{
			parse_anim(anim_path, anim_name, object);
			doomlog_mul(LOG_NORMAL, (char *[3]){\
					"parsed anim file:", extract_filename(anim_path), NULL});
			free(anim_path);
			anim_path = NULL;
		}
		ret = get_next_line(fd, &anim_path);
	}
	return (ret);
}

void	editor_load_anims(char *anim_name, t_object *object)
{
	int	fd;
	int	ret;
	int	frame_malloc_count;

	doomlog(LOG_NORMAL, "LOADING ANIMATIONS");
	frame_malloc_count = count_asset_list(ANIMLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			ANIMLISTPATH, "framecount =", s_itoa(frame_malloc_count), NULL});
	object->o_anim.frames = \
			ft_memalloc(sizeof(t_objectanimframe) * frame_malloc_count);
	ft_strncpy_term(object->o_anim.name, anim_name, 120);
	if (object->o_anim.frames == NULL)
		doomlog(LOG_EC_MALLOC, "editor_load_anims");
	fd = fileopen(ANIMLISTPATH, O_RDONLY);
	ret = parse_anim_list(fd, anim_name, object);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, ANIMLISTPATH);
	fileclose(fd, ANIMLISTPATH);
}
