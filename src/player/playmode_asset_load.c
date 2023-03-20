/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_asset_load.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 12:30:03 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "tga.h"
#include "file_io.h"

void	playmode_load_assets(t_sdlcontext *sdl)
{
	t_object	*human;
	int			i;

	// PROTECT IN CASE WORLD LEVEL DOESNT EXIST
	// THESE NEED TO BE IN THE SAME ORDER AS SAVING ATM
	playmode_load_fonts(sdl);
	playmode_load_images(sdl);
	playmode_load_env_textures(sdl);
	create_audio(&sdl->audio);
	playmode_load_sounds(&sdl->audio);
	playmode_load_music(&sdl->audio);
	playmode_load_objects(sdl);
	objects_init(sdl);
	human = get_object_by_name(*sdl, "Human.obj");
	playmode_load_anims("anim", human);
	playmode_load_anim_legend(sdl);
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
