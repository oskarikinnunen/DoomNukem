/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_asset_load.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/17 16:20:21 by raho             ###   ########.fr       */
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

	// PROTECT IN CASE WORLD LEVEL DOESNT EXIST
	// TODO: Optimize playmode loading
	// THESE NEED TO BE IN THE SAME ORDER AS SAVING ATM
	playmode_load_images(sdl);
	playmode_load_env_textures(sdl);
	playmode_load_objects(sdl);
	playmode_load_fonts(sdl);
	create_audio(&sdl->audio);
	playmode_load_sounds(&sdl->audio);
	playmode_load_music(&sdl->audio);
	//editor_load_images(sdl);
	//editor_load_env_textures(sdl);
	//editor_load_objects(sdl);
	//editor_load_fonts(sdl);
	//create_audio(&sdl->audio);
	//editor_load_sounds(&sdl->audio);
	//editor_load_music(&sdl->audio);
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
