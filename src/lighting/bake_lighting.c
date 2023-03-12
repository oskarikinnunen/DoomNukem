#include "doomnukem.h"

uint8_t	average(t_point sample, t_map map)
{
	t_point		subsample;
	t_point		avgsample;
	t_point		start;
	uint32_t	avg;
	int			hit;

	subsample.x = 0;
	subsample.y = 0;
	start = point_sub(sample, point_one());
	avg = 0;
	hit = 0;
	while (subsample.y < 3)
	{
		subsample.x = 0;
		while (subsample.x < 3)
		{
			avgsample = point_add(start, subsample);
			if (avgsample.x >= 0 && avgsample.y >= 0 && avgsample.x < map.size.x - 1 && avgsample.y < map.size.y - 1)
			{
			avg += map.lightmap[avgsample.y * map.size.x + avgsample.x];
			hit++;
			}
			subsample.x++;
		}
		subsample.y++;
	}
	return ((uint8_t)ft_clamp(avg / hit, 0, 255));
}

static void sample_pixel(int x, int y, int index, t_entity *entity)
{
	uint32_t	light;
	uint32_t	clr;
	t_img		*img;

	img = entity->obj->materials[index].img;
	//light = average((t_point){x, y}, entity->map[index]);
	light = entity->map->lightmap[y * entity->map->size.x + x];
	clr = img->data[(y % (img->size.y)) * img->size.x + (x % (img->size.x))];
	//light = 200;
	entity->map[index].data[y * entity->map[index].size.x + x] = update_pixel_brightness(light, clr);
}

void create_map_for_entity(t_entity *entity, struct s_world *world)
{
	int			y;
	int			x;
	int			index;

	if (!entity->map)
		return;
	index = 0;
	while (index < entity->obj->material_count)
	{
		y = 0;
		while (y < entity->map->size.y)
		{
			x = 0;
			while (x < entity->map->size.x)
			{
				sample_pixel(x, y, index, entity);
				x++;
			}
			y++;
		}
		index++;
	}
}

void create_lightmap_for_entity(t_entity *entity, struct s_world *world)
{
	t_lighting	lighting;
	t_object	*obj;
	t_img		*img;
	t_vector2	max;
	int			index;
	int			start;
	int			i;

	obj = entity->obj;
	if (obj->uv_count == 0 || entity->world_triangles == NULL)
	{
		entity->map = NULL;
		return;
	}
	if (!entity->map)
	{
		entity->map = malloc(sizeof(t_map) * entity->obj->material_count);
		bzero(entity->map, sizeof(t_map) * entity->obj->material_count);
	}
	if (entity->map == NULL)
		doomlog(LOG_FATAL, "Malloc fail in bake_lighting.c");
	lighting.world = world;
	lighting.entity = entity;
	i = 0;
	start = 0;
	max.x = 0.0f;
	max.y = 0.0f;
	while (i < obj->face_count)
	{
		int j = 0;
		while (j < 3)
		{
			t_vector3 uv;

			uv = entity->world_triangles[i].t[j];
			if (uv.x > max.x)
				max.x = uv.x;
			if (uv.y > max.y)
				max.y = uv.y;
			j++;
		}
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			img = obj->materials[index].img;
			max.x = fmaxf(max.x, 1.0f);
			max.y = fmaxf(max.y, 1.0f);
			entity->map[index].size = (t_point){ceilf(max.x), ceilf(max.y)};
			entity->map[index].img_size = img->size;
			if (entity->map[index].data)
				free(entity->map[index].data);
			if (entity->map[index].lightmap)
				free(entity->map[index].lightmap);
			entity->map[index].data = malloc(sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			entity->map[index].lightmap = malloc(sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			if (entity->map[index].data == NULL)
				doomlog(LOG_FATAL, "Malloc fail in bake_lighting.c");
			lighting.map = &entity->map[index];
			lighting.img = img->data;
			ft_bzero(entity->map[index].data, sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			ft_bzero(entity->map[index].lightmap, sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			while (start <= i && 1)
			{
				t_point_triangle temp;

				for (int vertex = 0; vertex < 3; vertex++)
				{
					t_vector2 uv = obj->uvs[obj->faces[start].uv_indices[vertex] - 1];
					uv.x = (uv.x * (float)(entity->obj->materials[index].img->size.x));
					uv.y = (uv.y * (float)(entity->obj->materials[index].img->size.y));
					temp.t[vertex] = entity->world_triangles[start].p[vertex].v;
					temp.p[vertex].x = entity->world_triangles[start].t[vertex].x;
					temp.p[vertex].y = entity->world_triangles[start].t[vertex].y;
					temp.p[vertex] = uv;
				//	temp.p[vertex].x = ft_clamp(floorf(temp.p[vertex].x), 0, lighting.map->size.x - 1);
				//	temp.p[vertex].y = ft_clamp(floorf(temp.p[vertex].y), 0, lighting.map->size.y - 1);
				}
				rasterize_light(temp, &lighting);
				start++;
			}
			max.x = 0.0f;
			max.y = 0.0f;
		}
		i++;
	}
}

void	recalculate_lighting(t_world *world)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	t_entity		*ent;

	recalculate_pointlight(world);
	i = 0;
	found = 0;
	cache = &world->entitycache;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free)
		{
			if	(ent->status == es_active && ent->obj != NULL)
			{
				create_lightmap_for_entity(ent, world);
				create_map_for_entity(ent, world);
			}
			found++;
		}
		i++;
	}
}