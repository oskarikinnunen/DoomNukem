#include "doomnukem.h"

uint8_t	average(t_point sample, t_map *lmap, t_map *map)
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
			if (avgsample.x >= 0 && avgsample.y >= 0 && avgsample.x < map->size.x - 1 && avgsample.y < map->size.y - 1)
			{
			avg += lmap->data[avgsample.y * map->size.x + avgsample.x];
			hit++;
			}
			subsample.x++;
		}
		subsample.y++;
	}
	return ((uint8_t)(avg / hit));
}

void create_map_for_entity(t_entity *entity, struct s_world *world)
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
	entity->map = malloc(sizeof(t_map) * entity->obj->material_count);
	if (entity->map == NULL)
		doomlog(LOG_FATAL, "Malloc fail in bake_lighting.c");
	lighting.world = world;
	i = 0;
	start = 0;
	max.x = -10000.0f;
	max.y = -10000.0f;
	while (i < obj->face_count)
	{
		int j = 0;
		while (j < 3)
		{
			t_vector2 uv;

			uv = obj->uvs[obj->faces[i].uv_indices[j] - 1];
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
			max.x = ft_maxf(max.x, 1.0f);
			max.y = ft_maxf(max.y, 1.0f);
			entity->map[index].size = (t_point){max.x * img->size.x, max.y * img->size.y};
			entity->map[index].img_size = img->size;
			entity->map[index].data = malloc(sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			if (entity->map[index].data == NULL)
				doomlog(LOG_FATAL, "Malloc fail in bake_lighting.c");
			lighting.map = &entity->map[index];
			lighting.img = img->data;
			while (start <= i && 0)
			{
				t_triangle_polygon temp;

				for (int vertex = 0; vertex < 3; vertex++)
				{
					temp.p3[vertex] = entity->world_triangles[start].p[vertex].v;
					temp.p2[vertex].x = entity->world_triangles[start].t[vertex].u * (entity->map[index].img_size.x - 1);
					temp.p2[vertex].y = entity->world_triangles[start].t[vertex].v * (entity->map[index].img_size.y - 1);
					temp.uv[vertex].x = entity->world_triangles[start].t[vertex].u * (img->size.x - 1);
					temp.uv[vertex].y = entity->world_triangles[start].t[vertex].v * (img->size.y - 1);
				}
				rasterize_light(temp, &lighting);
				start++;
			}
			for (int e = 0; e < entity->map[index].size.y && 1; e++)
			{
				for (int j = 0; j < entity->map[index].size.x; j++)
				{
					uint8_t	light = 200;
					uint32_t clr;
					clr = img->data[(e % (img->size.y)) * img->size.x + (j % (img->size.x))];
					Uint32 alpha = clr & 0xFF000000;
					Uint32 red = ((clr & 0x00FF0000) * light) >> 8;
					Uint32 green = ((clr & 0x0000FF00) * light) >> 8;
					Uint32 blue = ((clr & 0x000000FF) * light) >> 8;
					clr = alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF);
					entity->map[index].data[e * entity->map[index].size.x + j] = clr;
				}
			}
			max.x = -10000.0f;
			max.y = -10000.0f;
			//start = i;
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
				create_map_for_entity(ent, world);
			}
			found++;
		}
		i++;
	}
}