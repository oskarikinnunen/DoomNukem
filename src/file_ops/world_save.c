/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_save.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 13:57:45 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/13 14:50:59 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"

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
static void	_world_save_basic_entities(t_world world)
{
	int		fd;
	char	*filename;

	filename = world_filename(world.name, ".basic_ent");
	doomlog_mul(LOG_NORMAL, (char *[3]){\
		"saving .basic_ent to", filename, NULL});
	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC);
	//_world_remove_all_room_entities()
	//_world_remove_all_room_entities(&world);
	//_world_sanitize_all_room_pointers(&world);
	save_chunk(filename, "AREA", world.roomlist);
}

static void	_world_save_amap(t_world world)
{
	int		fd;
	char	*filename;

	filename = world_filename(world.name, ".amap");
	doomlog_mul(LOG_NORMAL, (char *[3]){\
		"saving .amap to", filename, NULL});
	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC);
	_world_remove_all_room_entities(&world);
	_world_sanitize_all_room_pointers(&world);
	save_chunk(filename, "AREA", world.roomlist);
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

static void _world_save_basic_ent(t_world world)
{
	int		fd;
	char	*filename;
	t_list	*entitylist;

	filename = world_filename(world.name, ".basic_ent");
	doomlog_mul(LOG_NORMAL, (char *[3]){\
		"saving .basic_ent to", filename, NULL});
	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC);
	entitylist = entitycache_to_list(&world.entitycache);
	_entitylist_basicify(entitylist);
	save_chunk(filename, "BENT", entitylist);
}

static void _world_save_full_ent(t_world world)
{
	int		fd;
	char	*filename;
	t_list	*entitylist;

	filename = world_filename(world.name, ".full_ent");
	doomlog_mul(LOG_NORMAL, (char *[3]){\
		"saving .full_ent to", filename, NULL});
	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC);
	entitylist = entitycache_to_list(&world.entitycache);
	save_chunk(filename, "FENT", entitylist);
}

static void	_world_save_images(t_world *world, char *image_list)
{
	int		fd;
	int		ret;
	char	*filename;

	(void)world;
	filename = NULL;
	fd = fileopen(image_list, O_RDONLY);
	ret = get_next_line(fd, &filename);
	while (ret)
	{
		if (filename)
		{
			force_pack_file(".tgapack", filename);
			free(filename);
			filename = NULL;
		}
		ret = get_next_line(fd, &filename);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "_world_save_images");
	fileclose(fd, image_list);
}

void	world_save_to_file(t_world world)
{
	doomlog(LOG_NORMAL, "SAVING WORLD");
	_world_save_amap(world);
	_world_save_basic_ent(world);
	_world_save_full_ent(world);
	_world_init_rooms(&world);
	//_world_save_images(&world, "assets/.image_list.txt");
	//_world_save_images(&world, "assets/.image_list_env.txt");
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