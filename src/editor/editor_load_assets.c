/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_assets.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/23 15:23:52 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "file_io.h"
#include "tga.h"

int	count_asset_list(char *filename)
{
	char	*line;
	int		ret;
	int		fd;
	int		i;

	if (filename == NULL)
		return (0);
	fd = fileopen(filename, O_RDONLY);
	line = NULL;
	ret = get_next_line(fd, &line);
	i = 0;
	while (ret)
	{
		i++;
		if (line)
		{
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, filename);
	fileclose(fd, filename);
	return (i);
}

void	editor_loading_screen(char *loading_message, t_sdlcontext *sdl)
{
	static int		first_time = 1;
	static t_img	loading_image;
	TTF_Font		*temp;
	int				len;
	SDL_Event			e;

	if (first_time)
		loading_image = tga_parse(LOADINGSCREENIMG);
	draw_image(*sdl, point_zero(), loading_image, (t_point){sdl->window_w, sdl->window_h});
	if (loading_message != NULL)
	{
		len = ft_strlen(loading_message);
		temp = sdl->font_default->size_default;
		sdl->font_default->size_default = sdl->font_default->sizes[1];
		print_text_boxed(sdl, loading_message, (t_point){((sdl->window_w / 2) - (len / 2 * (2 * FONT_SIZE_DEFAULT))), (sdl->window_h - (sdl->window_h / 5))});
		sdl->font_default->size_default = temp;
	}
	ft_memcpy(sdl->window_surface->pixels, sdl->surface->pixels, sizeof(uint32_t) * sdl->window_w * sdl->window_h);
	while (SDL_PollEvent(&e))
		;
	if (SDL_UpdateWindowSurface(sdl->window) < 0)
	doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
}

void	editor_load_assets(t_sdlcontext *sdl)
{
	t_object	*human;
	int			i;

	editor_load_fonts(sdl);
	editor_loading_screen("LOADING IMAGES", sdl);
	editor_load_images(sdl);
	editor_loading_screen("LOADING ENV TEXTURES", sdl);
	editor_load_env_textures(sdl);
	create_audio(&sdl->audio);
	editor_loading_screen("LOADING SOUNDS", sdl);
	editor_load_sounds(&sdl->audio);
	editor_loading_screen("LOADING MUSIC", sdl);
	editor_load_music(&sdl->audio);
	editor_loading_screen("LOADING OBJECTS", sdl);
	editor_load_objects(sdl);
	objects_init(sdl);
	human = get_object_by_name(*sdl, "Human.obj");
	editor_loading_screen("LOADING ANIMATIONS", sdl);
	editor_load_anims("anim", human);
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
