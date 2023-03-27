/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_anims.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:49:29 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "assets.h"
#include "objects.h"

static void	unpack_and_load_anim(int level_fd, char *anim_path,
									char *anim_name, t_object *object)
{
	load_and_write_filecontent(level_fd, anim_path, TEMPANIM);
	parse_anim(TEMPANIM, anim_name, object);
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded anim:", extract_file_name(anim_path), NULL});
}

static int	parse_anim_list(int level_fd, char *anim_name, t_object *object)
{
	int		temp_fd;
	int		ret;
	char	*anim_path;

	temp_fd = ft_fileopen(TEMPANIMLIST, O_RDONLY);
	anim_path = NULL;
	ret = get_next_line(temp_fd, &anim_path);
	while (ret)
	{
		if (anim_path)
		{
			unpack_and_load_anim(level_fd, anim_path, anim_name, object);
			free(anim_path);
			anim_path = NULL;
		}
		ret = get_next_line(temp_fd, &anim_path);
	}
	ft_fileclose(temp_fd, TEMPANIMLIST);
	return (ret);
}

void	playmode_load_anims(int level_fd, char *anim_name, t_object *object)
{
	int	ret;
	int	frame_malloc_count;

	doomlog(LOG_NORMAL, "UNPACKING ANIMS");
	load_and_write_filecontent(level_fd, ANIMLISTPATH, TEMPANIMLIST);
	frame_malloc_count = count_asset_list(TEMPANIMLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPANIMLIST, "framecount =", s_itoa(frame_malloc_count), NULL});
	object->o_anim.frames = \
			prot_memalloc(sizeof(t_object_anim_frame) * frame_malloc_count);
	ft_strncpy_term(object->o_anim.name, anim_name, 120);
	ret = parse_anim_list(level_fd, anim_name, object);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_anims");
}
