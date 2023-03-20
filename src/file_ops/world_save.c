/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_save.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 13:57:45 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 15:32:48 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"
#include "filechunks.h"

static void	save_entities(char *filename, t_list	*entitylist)
{
	t_list		*l;
	t_list		temp;
	t_entity	*ent;
	char		componentfilename[64];

	l = entitylist;
	temp.next = NULL;
	while (l != NULL)
	{
		ent = l->content;
		if (ent->component.type != COMP_NONE)
		{
			temp.content = ent->component.data;
			temp.content_size = ent->component.data_size;
			char	*str;
			int		fd;
			str = ft_itoa(ent->id);
			ft_strcpy(componentfilename, str);
			free(str);
			ft_strcat(componentfilename, ".comp");
			fd = fileopen(componentfilename, O_RDWR | O_CREAT | O_TRUNC);
			close(fd);
			printf("saving component to file %s \n", componentfilename);
			save_chunk(componentfilename, "COMP", &temp);
			pack_file(filename, componentfilename);
			remove(componentfilename);
			//TODO: remove .comp
			ent->component.data = NULL;
			//ent->component.type = pft_none;
		}
		l = l->next;
	}
	save_chunk(filename, "ENT_", entitylist);
}

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
	//printf("cache has %i existing entities\n", cache->existing_entitycount);
	while (found < cache->existing_entitycount && i < cache->alloc_count)
	{
		if (cache->entities[i].status != es_free)
		{
			//printf("found %i \n", found);
			cache->entities[i].id = found;
			//cache->entities[i].component.data = NULL;
			//cache->entities[i].component.type = pft_none; //TODO: save component to file with entity id
			//printf("entitylist null status before push %i \n", entitylist == NULL);
			list_push(&entitylist, &cache->entities[i], sizeof(t_entity));
			found++;
		}
		i++;
	}
	return (entitylist);
}

static void _world_remove_all_room_entities(t_world *world)
{
	t_list	*l;
	t_area	*room;

	l = world->roomlist;
	while (l != NULL)
	{
		room = (t_area *)l->content;
		room_remove_entities(room, world);
		l = l->next;
	}
}

void _world_sanitize_all_room_pointers(t_world *world)
{
	t_list	*l;
	t_area	*room;
	int		i;

	l = world->roomlist;
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
	save_chunk(level, "AREA", world.roomlist);
}

static void _entitylist_basicify(t_list *ent_list)
{
	t_list		*l;
	t_entity	*e;
	l = ent_list;
	while (l != NULL)
	{
		e = (t_entity *)l->content;
		l->content_size = sizeof(t_gamestring) + sizeof(t_transform);
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

static void _world_save_full_ent(char *level, t_world world)
{
	t_list	*entitylist;

	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"saving .full_ent to", level, NULL});
	entitylist = entitycache_to_list(&world.entitycache);
	save_chunk(level, "FENT", entitylist);
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

void	world_save_to_file(t_world world)
{
	char	level_path[256];

	ft_strcpy(level_path, "worlds/");
	ft_strncat(level_path, world.name, 200);
	
	doomlog(LOG_NORMAL, "SAVING WORLD");

	clean_create_level_file(level_path);

	_world_save_amap(level_path, world);
	_world_save_basic_ent(level_path, world);
	_world_save_full_ent(level_path, world);
	_world_init_rooms(&world);

	// THESE NEED TO BE IN THE SAME ORDER AS LOADING ATM
	pack_file_to_level(level_path, FONTLISTPATH);
	_world_save_asset_files_rene(level_path, FONTLISTPATH);
	pack_file_to_level(level_path, IMGLISTPATH);
	_world_save_asset_files_rene(level_path, IMGLISTPATH);
	pack_file_to_level(level_path, IMGENVLISTPATH);
	_world_save_asset_files_rene(level_path, IMGENVLISTPATH);
	pack_file_to_level(level_path, SOUNDLISTPATH);
	_world_save_asset_files_rene(level_path, SOUNDLISTPATH);
	pack_file_to_level(level_path, MUSICLISTPATH);
	_world_save_asset_files_rene(level_path, MUSICLISTPATH);
	pack_file_to_level(level_path, OBJLISTPATH);
	_world_save_asset_files_rene(level_path, OBJLISTPATH);
	_world_save_asset_files_rene(level_path, MTLLISTPATH);
	pack_file_to_level(level_path, ANIMLISTPATH);
	_world_save_asset_files_rene(level_path, ANIMLISTPATH);
	pack_file_to_level(level_path, ANIMLEGENDPATH);
}

void	save_world(char *namename, t_world world)
{
	/*int		fd;
	char	*filename;

	printf("worldname '%s'\n", namename);
	filename = full_worldpath(namename);
	printf("saving world to '%s'\n", filename);
	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	close(fd);
	printf("Saving world\n");
	_world_remove_all_room_entities(&world);
	_world_sanitize_all_room_pointers(&world);
	//t_list	*entitylist = entitycache_to_list(&world.entitycache);
	//save_entities(filename, entitylist);

	printf("Saving rooms\n");
	save_chunk(filename, "AREA", world.roomlist);*/
}
