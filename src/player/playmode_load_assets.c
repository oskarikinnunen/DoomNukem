/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_assets.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/23 21:39:35 by raho             ###   ########.fr       */
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
	int			fd;

	ft_strcpy(level_path, "worlds/");
	ft_strncat(level_path, level_name, 200);
	fd = fileopen(level_path, O_RDONLY);
	playmode_load_fonts(fd, sdl);
	playmode_load_images(fd, sdl);
	playmode_loading_screen("LOADING ENV TEXTURES", sdl);
	playmode_load_env_textures(fd, sdl);
	create_audio(&sdl->audio);
	playmode_loading_screen("LOADING SOUNDS", sdl);
	playmode_load_sounds(fd, &sdl->audio);
	playmode_loading_screen("LOADING MUSIC", sdl);
	playmode_load_music(fd, &sdl->audio);
	playmode_loading_screen("LOADING OBJECTS", sdl);
	playmode_load_objects(fd, sdl);
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
	fileclose(fd, level_name);
}
