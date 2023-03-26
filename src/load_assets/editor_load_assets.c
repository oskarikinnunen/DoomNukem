/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_assets.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:37:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "assets.h"
#include "objects.h"

void	editor_load_assets(t_sdlcontext *sdl)
{
	t_object	*human;

	editor_load_fonts(sdl);
	editor_loading_screen("LOADING IMAGES", sdl);
	editor_load_images(sdl);
	editor_loading_screen("LOADING ENV TEXTURES", sdl);
	editor_load_env_textures(sdl);
	editor_loading_screen("LOADING SOUNDS", sdl);
	create_audio(&sdl->audio);
	editor_load_sounds(&sdl->audio);
	editor_loading_screen("LOADING MUSIC", sdl);
	editor_load_music(&sdl->audio);
	editor_loading_screen("LOADING OBJECTS", sdl);
	editor_load_objects(sdl);
	objects_init(sdl);
	editor_loading_screen("LOADING ANIMATIONS", sdl);
	human = get_object_by_name(*sdl, "Human.obj");
	editor_load_anims("anim", human);
	editor_load_anim_legend(sdl);
	log_loaded_animations(sdl);
}
