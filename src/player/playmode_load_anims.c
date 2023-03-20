/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_anims.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:49:29 by raho              #+#    #+#             */
/*   Updated: 2023/03/20 15:05:31 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"

static void	unpack_and_load_anim(char *anim_path, char *anim_name,
									t_object *object)
{
	load_and_write_filecontent(LEVEL0FILE, anim_path, TEMPANIM);
	parse_anim(TEMPANIM, anim_name, object);
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded anim:", extract_filename(anim_path), NULL});
	remove(TEMPANIM);
}

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
			unpack_and_load_anim(anim_path, anim_name, object);
			free(anim_path);
			anim_path = NULL;
		}
		ret = get_next_line(fd, &anim_path);
	}
	return (ret);
}

void	playmode_load_anims(char *anim_name, t_object *object)
{
	int	fd;
	int	ret;
	int	frame_malloc_count;

	doomlog(LOG_NORMAL, "UNPACKING ANIMS");
	load_and_write_filecontent(LEVEL0FILE, ANIMLISTPATH, TEMPANIMLIST);
	frame_malloc_count = count_asset_list(TEMPANIMLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPANIMLIST, "framecount =", s_itoa(frame_malloc_count), NULL});
	object->o_anim.frames = \
			ft_memalloc(sizeof(t_objectanimframe) * frame_malloc_count);
	if (object->o_anim.frames == NULL)
		doomlog(LOG_EC_MALLOC, "playmode_load_anims");
	fd = fileopen(TEMPANIMLIST, O_RDONLY);
	ret = parse_anim_list(fd, anim_name, object);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_anims");
	fileclose(fd, TEMPANIMLIST);
	remove(TEMPANIMLIST);
}
