#include "doomnukem.h"

static void render_shadowmap(t_world *world, t_lighting *lighting)
{
	int			i;
	int			found;
	t_entity	*ent;

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
			{
				if (lighting->light->ignoreself == false || (lighting->light->ignoreself && ent->id != lighting->entity->id))
					render_zbuffer(lighting, ent);
			}
			found++;
		}
		i++;
	}
}

static void calculate_pointlight(t_world *world, t_entity *entity)
{
	int				index;
	t_lighting		lighting;

	lighting.light = entity->component.data;
	lighting.resolution = lighting.light->cubemap.resolution;
	lighting.entity = entity;
	index = 0;
	while (index < 6)
	{
		lighting.camera = lighting.light->cubemap.cameras[index];
		lighting.zbuffer = lighting.light->cubemap.shadowmaps[index];
		ft_bzero(lighting.zbuffer, sizeof(float) * lighting.light->cubemap.resolution.x * lighting.light->cubemap.resolution.y);
		render_shadowmap(world, &lighting);
		index++;
	}
}

void	recalculate_pointlight(t_world *world)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	t_entity		*ent;

	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free)
		{
			if	(ent->status == es_active && ent->component.type == COMP_LIGHT)
			{
				calculate_pointlight(world, ent);
			}
			found++;
		}
		i++;
	}
}