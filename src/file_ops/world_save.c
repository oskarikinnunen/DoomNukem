/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_save.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 13:57:45 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 19:19:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"
#include "filechunks.h"

char	*world_filename(char *worldname, char *suffix)
{
	static char	full[128];

	ft_bzero(full, sizeof(full));
	ft_strcat(full, "worlds/");
	ft_strcat(full, worldname);
	ft_strcat(full, suffix);
	return (full);
}

t_list	*entitycache_to_list(t_entitycache *cache)
{
	int				i;
	int				found;
	t_list			*entitylist;

	i = 0;
	found = 0;
	entitylist = NULL;
	while (found < cache->existing_entitycount && i < cache->alloc_count)
	{
		if (cache->entities[i].status != es_free)
		{
			cache->entities[i].id = found;
			list_push(&entitylist, &cache->entities[i], sizeof(t_entity));
			found++;
		}
		i++;
	}
	return (entitylist);
}

static void	_world_remove_all_room_entities(t_world *world)
{
	t_list	*l;
	t_area	*room;

	l = world->arealist;
	while (l != NULL)
	{
		room = (t_area *)l->content;
		room_remove_entities(room, world);
		l = l->next;
	}
}

void	_world_sanitize_all_room_pointers(t_world *world)
{
	t_list	*l;
	t_area	*room;
	int		i;

	l = world->arealist;
	while (l != NULL)
	{
		room = (t_area *)l->content;
		i = 0;
		while (i < room->wallcount)
		{
			room->walls[i].edgeline.start = NULL;
			room->walls[i].edgeline.end = NULL;
			i++;
		}
		l = l->next;
	}
}

/*
Save .amap
Save .basic_ent
Save .full_ent
*/

static void	_world_save_amap(char *level, t_world world)
{
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"saving .amap to", level, NULL});
	_world_remove_all_room_entities(&world);
	_world_sanitize_all_room_pointers(&world);
	save_chunk(level, "AREA", world.arealist);
}

static void	_entitylist_basicify(t_list *ent_list)
{
	t_list		*l;
	t_entity	*e;

	l = ent_list;
	while (l != NULL)
	{
		e = (t_entity *)l->content;
		l->content_size = sizeof(t_gamestring)
			+ sizeof(t_transform) + sizeof(t_componenttype) + sizeof(uint32_t);
		l = l->next;
	}
}

static void _world_save_basic_ent(char *level, t_world world)
{
	t_list	*entitylist;

	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"saving .basic_ent to", level, NULL});
	entitylist = entitycache_to_list(&world.entitycache);
	_entitylist_basicify(entitylist);
	save_chunk(level, "BENT", entitylist);
}

static void	_world_save_asset_files_rene(char *level, char *asset_list)
{
	int		fd;
	int		ret;
	char	*filename;

	filename = NULL;
	fd = fileopen(asset_list, O_RDONLY);
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
	fileclose(fd, asset_list);
}

void	clean_create_level_file(char *level)
{
	fileclose(fileopen(level, O_CREAT | O_RDWR | O_TRUNC), level);
	doomlog_mul(LOG_NORMAL, (char *[5]){\
			"cleaned/created the level:", level, "succesfully"});
}

void	loading_screen_show(t_sdlcontext *sdl, char *loading_message)
{
	static t_img	*loading_image = NULL;
	TTF_Font		*temp;
	int				len;

	if (loading_image == NULL)
		loading_image = get_image_by_name(*sdl, "loading_screen.tga");
	draw_image(*sdl, point_zero(), *loading_image, (t_point){sdl->window_w, sdl->window_h});
	if (loading_message != NULL)
	{
		len = ft_strlen(loading_message);
		temp = sdl->font_default->size_default;
		sdl->font_default->size_default = sdl->font_default->sizes[1];
		sdl->font_default->color = color32_to_sdlcolor(CLR_BLACK);
		print_text(sdl, loading_message, (t_point){((sdl->window_w / 2) - (len / 2 * (2 * FONT_SIZE_DEFAULT))) - 5, (sdl->window_h - (sdl->window_h / 5))});
		sdl->font_default->size_default = temp;
	}
	memcpy(sdl->window_surface->pixels, sdl->surface->pixels, sizeof(uint32_t) * sdl->window_w * sdl->window_h);
	if (SDL_UpdateWindowSurface(sdl->window) < 0)
		doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
}

void	world_save_to_file(t_world world)
{
	char	level_path[256];

	ft_strcpy(level_path, "worlds/");
	ft_strncat(level_path, world.name, 200);
	
	doomlog(LOG_NORMAL, "SAVING WORLD");

	clean_create_level_file(level_path);

	loading_screen_show(world.sdl, "SAVING AREAS, ENTITIES AND ROOMS");
	_world_save_amap(level_path, world);
	_world_save_basic_ent(level_path, world);
	_world_init_rooms(&world);

	// THESE NEED TO BE IN THE SAME ORDER AS LOADING ATM
	loading_screen_show(world.sdl, "SAVING FONTS");
	pack_file_to_level(level_path, FONTLISTPATH);
	_world_save_asset_files_rene(level_path, FONTLISTPATH);
	
	loading_screen_show(world.sdl, "SAVING IMAGES");
	pack_file_to_level(level_path, IMGLISTPATH);
	_world_save_asset_files_rene(level_path, IMGLISTPATH);

	loading_screen_show(world.sdl, "SAVING ENV TEXTURES");
	pack_file_to_level(level_path, IMGENVLISTPATH);
	_world_save_asset_files_rene(level_path, IMGENVLISTPATH);

	loading_screen_show(world.sdl, "SAVING SOUNDS");
	pack_file_to_level(level_path, SOUNDLISTPATH);
	_world_save_asset_files_rene(level_path, SOUNDLISTPATH);
	
	loading_screen_show(world.sdl, "SAVING MUSIC");
	pack_file_to_level(level_path, MUSICLISTPATH);
	_world_save_asset_files_rene(level_path, MUSICLISTPATH);
	
	loading_screen_show(world.sdl, "SAVING OBJECTS");
	pack_file_to_level(level_path, OBJLISTPATH);
	_world_save_asset_files_rene(level_path, OBJLISTPATH);

	loading_screen_show(world.sdl, "SAVING MATERIALS");
	_world_save_asset_files_rene(level_path, MTLLISTPATH);
	
	loading_screen_show(world.sdl, "SAVING ANIMATIONS");
	pack_file_to_level(level_path, ANIMLISTPATH);
	_world_save_asset_files_rene(level_path, ANIMLISTPATH);
	pack_file_to_level(level_path, ANIMLEGENDPATH);

	loading_screen_show(world.sdl, "PRESS ANY KEY TO CONTINUE");

	static SDL_Event	e;

	while (1)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (iskey(e, SDLK_ESCAPE))
					exit(0);
				else
					return ;
			}
		}
	}
	
}
