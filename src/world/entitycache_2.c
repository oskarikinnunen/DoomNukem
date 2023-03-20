/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitycache_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:51:01 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 12:03:15 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"
#include "editor_tools.h"

void	destroy_entity(t_world *world, t_entity *ent)
{
	t_entitycache	*cache;

	cache = &world->entitycache;
	if (ent->id >= cache->alloc_count)
		doomlog_mul(LOG_EC_MALLOC, (char *[4]){
			"Tried to remove entity ", s_itoa(ent->id),
			", which exceeds the allocated cache size.", NULL
		});
	cache->entities[ent->id].status = es_free;
	cache->entities[ent->id].obj = NULL;
	free(ent->world_triangles);
	if (cache->existing_entitycount == 0)
		doomlog(LOG_EC_MALLOC, "Tried to remove entity -1\n");
	cache->existing_entitycount--;
}

t_entity	*find_entity_with_comp(t_world	*world, t_componenttype comp)
{
	int				i;
	t_entitycache	*cache;

	cache = &world->entitycache;
	i = 0;
	while (i < cache->alloc_count)
	{
		if (cache->entities[i].status == es_active
			&& cache->entities[i].component.type == comp)
			return (&cache->entities[i]);
		i++;
	}
	return (NULL);
}

void	load_component(t_entity	*entity, char	*filename)
{
	t_list	*temp;
	int		fd;
	char	*str;
	char	comp_filename[64];

	printf("trying to load component\n");
	str = ft_itoa(entity->id);
	ft_strcpy(comp_filename, str);
	free(str);
	ft_strcat(comp_filename, ".comp");
	fd = load_filecontent_fd(filename, comp_filename);
	if (fd != -1)
	{
		close(fd);
		temp = load_chunk(comp_filename, "COMP", entity->component.data_size);
		if (temp != NULL && temp->content != NULL)
		{
			printf("found component \n");
			entity->component.data = temp->content;
		}
		remove(comp_filename);
	}
}

void	load_cache_from_list(char *filename, t_world *world, t_list *l)
{
	t_entity	*list_entity;
	t_entity	*world_entity;
	char		comp_filename[64];
	char		*str;

	while (l != NULL)
	{
		list_entity = l->content;
		if (list_entity->component.type != COMP_NONE)
		{
			load_component(list_entity, filename);
		}
		world_entity = spawn_entity(world, NULL);
		ft_memcpy(world_entity, list_entity, sizeof(t_entity));
		l = l->next;
	}
}
