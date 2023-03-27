/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_save.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 13:57:45 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"

static void	pack_multiple_files_to_level(char *level, char *asset_list)
{
	int		fd;
	int		ret;
	char	*filename;

	filename = NULL;
	fd = ft_fileopen(asset_list, O_RDONLY);
	ret = get_next_line(fd, &filename);
	while (ret)
	{
		if (filename)
		{
			pack_file_to_level(level, filename);
			free(filename);
			filename = NULL;
		}
		ret = get_next_line(fd, &filename);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, asset_list);
	ft_fileclose(fd, asset_list);
}

static void	print_saving_message(char *saving_message, t_sdlcontext *sdl)
{
	int			len;
	TTF_Font	*temp;

	if (saving_message != NULL)
	{
		len = ft_strlen(saving_message);
		temp = sdl->font_default->size_default;
		sdl->font_default->size_default = sdl->font_default->sizes[0];
		print_text_boxed(sdl, saving_message, \
				(t_point){((sdl->window_w / 2) - \
				(len / 2 * FONT_SIZE_DEFAULT)), \
				((sdl->window_h / 2) + (FONT_SIZE_DEFAULT * 1))});
		sdl->font_default->size_default = temp;
		ft_memcpy(sdl->window_surface->pixels, sdl->surface->pixels, \
				sizeof(uint32_t) * sdl->window_w * sdl->window_h);
		if (SDL_UpdateWindowSurface(sdl->window) < 0)
			doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
	}
}

// THESE NEED TO BE IN THE SAME EXACT ORDER AS IN WORLD_LOAD
static void	pack_assets1(t_sdlcontext *sdl, char *level_path,
							char *surface_cache, size_t surface_size)
{
	print_saving_message("SAVING FONTS", sdl);
	pack_file_to_level(level_path, FONTLISTPATH);
	pack_multiple_files_to_level(level_path, FONTLISTPATH);
	ft_memcpy(sdl->surface->pixels, surface_cache, surface_size);
	print_saving_message("SAVING IMAGES", sdl);
	pack_file_to_level(level_path, IMGLISTPATH);
	pack_multiple_files_to_level(level_path, IMGLISTPATH);
	ft_memcpy(sdl->surface->pixels, surface_cache, surface_size);
	print_saving_message("SAVING ENV TEXTURES", sdl);
	pack_file_to_level(level_path, IMGENVLISTPATH);
	pack_multiple_files_to_level(level_path, IMGENVLISTPATH);
	ft_memcpy(sdl->surface->pixels, surface_cache, surface_size);
	print_saving_message("SAVING SOUNDS", sdl);
	pack_file_to_level(level_path, SOUNDLISTPATH);
	pack_multiple_files_to_level(level_path, SOUNDLISTPATH);
	ft_memcpy(sdl->surface->pixels, surface_cache, surface_size);
	print_saving_message("SAVING MUSIC", sdl);
	pack_file_to_level(level_path, MUSICLISTPATH);
	pack_multiple_files_to_level(level_path, MUSICLISTPATH);
	ft_memcpy(sdl->surface->pixels, surface_cache, surface_size);
	print_saving_message("SAVING OBJECTS", sdl);
	pack_file_to_level(level_path, OBJLISTPATH);
	pack_multiple_files_to_level(level_path, OBJLISTPATH);
	ft_memcpy(sdl->surface->pixels, surface_cache, surface_size);
}

// THESE NEED TO BE IN THE SAME EXACT ORDER AS THE ONES IN PLAYMODE_LOAD_ASSETS
static void	pack_assets2(t_sdlcontext *sdl, char *level_path,
							char *surface_cache, size_t surface_size)
{
	print_saving_message("SAVING MATERIALS", sdl);
	pack_multiple_files_to_level(level_path, MTLLISTPATH);
	ft_memcpy(sdl->surface->pixels, surface_cache, surface_size);
	print_saving_message("SAVING ANIMATIONS", sdl);
	pack_file_to_level(level_path, ANIMLISTPATH);
	pack_multiple_files_to_level(level_path, ANIMLISTPATH);
	pack_file_to_level(level_path, ANIMLEGENDPATH);
}

void	world_save_to_file(t_world world)
{
	char	level_path[256];
	char	*surface_cache;
	size_t	surface_size;

	doomlog(LOG_NORMAL, "SAVING WORLD");
	ft_strncpy_term(level_path, "worlds/", 250);
	ft_strncat(level_path, world.name, 200);
	clean_create_level_file(level_path);
	surface_size = sizeof(uint32_t) * \
					world.sdl->surface->h * world.sdl->surface->w;
	surface_cache = ft_memdup(world.sdl->surface->pixels, surface_size);
	if (surface_cache == NULL)
		doomlog(LOG_EC_MALLOC, "world_save_to_file");
	print_saving_message("SAVING AREAS ENTITIES AND ROOMS", world.sdl);
	world_save_amap(level_path, world);
	world_save_basic_ent(level_path, world);
	world_init_rooms(&world);
	ft_memcpy(world.sdl->surface->pixels, surface_cache, surface_size);
	pack_assets1(world.sdl, level_path, surface_cache, surface_size);
	pack_assets2(world.sdl, level_path, surface_cache, surface_size);
	free(surface_cache);
}
