/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_assets.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/23 19:40:40 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "tga.h"
#include "file_io.h"

// THESE NEED TO BE IN THE SAME EXACT ORDER AS THE ONES IN WORLD_SAVE
void	playmode_load_assets(char *level_name, t_sdlcontext *sdl)
{
	t_object	*human;
	int			i;
	char		level_path[256];

	ft_strcpy(level_path, "worlds/");
	ft_strncat(level_path, level_name, 200);
	playmode_load_fonts(level_path, sdl);
	playmode_load_images(level_path, sdl);
	playmode_loading_screen("LOADING ENV TEXTURES", sdl);
	playmode_load_env_textures(level_path, sdl);
	create_audio(&sdl->audio);
	playmode_loading_screen("LOADING SOUNDS", sdl);
	playmode_load_sounds(level_path, &sdl->audio);
	playmode_loading_screen("LOADING MUSIC", sdl);
	playmode_load_music(level_path, &sdl->audio);
	playmode_loading_screen("LOADING OBJECTS", sdl);
	playmode_load_objects(level_path, sdl);
	objects_init(sdl);
	human = get_object_by_name(*sdl, "Human.obj");
	playmode_loading_screen("LOADING ANIMATIONS", sdl);
	//playmode_load_anims(level_path, "anim", human);
	//playmode_load_anim_legend(level_path, sdl);
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
