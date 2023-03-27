/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_assets.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:26:40 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "assets.h"
#include "file_io.h"
#include "objects.h"

void	playmode_load_assets(char *level_name, t_sdlcontext *sdl)
{
	t_object	*human;
	char		level_path[256];
	int			fd;

	ft_strncpy_term(level_path, "worlds/", 250);
	ft_strncat(level_path, level_name, 200);
	fd = ft_fileopen(level_path, O_RDONLY);
	playmode_load_fonts(fd, sdl);
	playmode_load_images(fd, sdl);
	playmode_loading_screen("LOADING ENV TEXTURES", sdl);
	playmode_load_env_textures(fd, sdl);
	playmode_loading_screen("LOADING SOUNDS", sdl);
	create_audio(&sdl->audio);
	playmode_load_sounds(fd, &sdl->audio);
	playmode_loading_screen("LOADING MUSIC", sdl);
	playmode_load_music(fd, &sdl->audio);
	playmode_loading_screen("LOADING OBJECTS", sdl);
	playmode_load_objects(fd, sdl);
	objects_init(sdl);
	playmode_loading_screen("LOADING ANIMATIONS", sdl);
	human = get_object_by_name(*sdl, "Human.obj");
	playmode_load_anims(fd, "anim", human);
	playmode_load_anim_legend(fd, sdl);
	log_loaded_animations(sdl);
	ft_fileclose(fd, level_name);
}
