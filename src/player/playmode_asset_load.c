/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_asset_load.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/16 15:51:31 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "tga.h"
#include <dirent.h>
#include "file_io.h"

void	playmode_load_assets(t_sdlcontext *sdl)
{
	t_object	*human;
	int			i;

	// These 3 playmode load functions are working but are terribly optimized. TODO: Optimize them
	playmode_load_images(sdl);
	playmode_load_env_textures(sdl);
	playmode_load_objects(sdl);
	//editor_load_images(sdl);
	//editor_load_env_textures(sdl);
	//editor_load_objects(sdl);
	editor_load_fonts(sdl);
	editor_load_audio(&sdl->audio);
	objects_init(sdl);
	human = get_object_by_name(*sdl, "Human.obj");
	editor_load_anims(human, "anim", 0);
	editor_load_anim_legend(sdl);
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
