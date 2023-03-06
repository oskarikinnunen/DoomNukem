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
			entity->map[index].data = malloc(sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			if (entity->map[index].data == NULL)
				doomlog(LOG_FATAL, "Malloc fail in bake_lighting.c");
			lighting.map = &entity->map[index];
			lighting.img = img->data;
			ft_bzero(entity->map[index].data, sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			for (int e = 0; e < entity->map[index].size.y && 1; e++)
			{
				for (int j = 0; j < entity->map[index].size.x; j++)
				{
					uint32_t clr;
					clr = img->data[(e % (img->size.y)) * img->size.x + (j % (img->size.x))];
					clr = update_pixel_brightness(200, clr);
					entity->map[index].data[e * entity->map[index].size.x + j] = clr;
				}
			}
			while (start <= i && 1)
			{
				t_point_triangle temp;

				for (int vertex = 0; vertex < 3; vertex++)
				{
					t_vector2 uv = obj->uvs[obj->faces[start].uv_indices[vertex] - 1];
					uv.x = roundf(uv.x * (float)(entity->obj->materials[index].img->size.x));
					uv.y = roundf(uv.y * (float)(entity->obj->materials[index].img->size.y));
					//uv = vector2_add_xy(uv, -0.5f);
					//temp.t[vertex] = uv;
					temp.p[vertex] = uv;
					//temp.p[vertex].x = entity->world_triangles[start].t[vertex].x;
					//temp.p[vertex].y = entity->world_triangles[start].t[vertex].y;
				}
				t_vector2	midpoint = vector2_div(vector2_add(vector2_add(temp.p[0], temp.p[1]), temp.p[2]), 3.0f);

				for (int vertex = 0; vertex < 3 && 0; vertex++)
				{
					t_vector2 dir = vector2_normalise(vector2_sub(temp.p[vertex], midpoint));
					//if (vector2_cmp(temp.p[vertex], midpoint))
					//	temp.p[vertex] = vector2_zero();
				//	dir = vector2_mul(dir, 2.0f);
					
					temp.p[vertex] = vector2_add(temp.p[vertex], dir);
					temp.p[vertex].x = fmaxf(temp.p[vertex].x, 0.0f);
					temp.p[vertex].y = fmaxf(temp.p[vertex].y, 0.0f);
					if ((int)(temp.p[vertex].x / (float)lighting.map->size.x) > 0)
						temp.p[vertex].x = (int)(temp.p[vertex].x / (float)lighting.map->size.x) * (lighting.map->size.x);
					if ((int)(temp.p[vertex].y / (float)lighting.map->size.y) > 0)
						temp.p[vertex].y = (int)(temp.p[vertex].y / (float)lighting.map->size.y) * (lighting.map->size.y);
				}
				rasterize_light(temp, &lighting);
				start++;
			}
			for (int e = 0; e < entity->map[index].size.y && 0; e++)
			{
				for (int j = 0; j < entity->map[index].size.x; j++)
				{
					uint32_t clr;
					clr = img->data[(e % (img->size.y)) * img->size.x + (j % (img->size.x))];
					clr = update_pixel_brightness(200, clr);
					for (int x = start; x <= i; x++)
					{
						t_point_triangle temp;
						for (int vertex = 0; vertex < 3; vertex++)
						{
							temp.t[vertex] = entity->world_triangles[x].p[vertex].v;
							temp.p[vertex].x = (entity->world_triangles[x].t[vertex].x);
							temp.p[vertex].y = (entity->world_triangles[x].t[vertex].y);
						}
						t_vector2 bary = barycentric_coordinates(temp.p, (t_vector2){ceilf((float)j + 0.0f), ceilf((float)e + 0.0f)});
						if (bary.x >= -0.01f && bary.y >= -0.01f && bary.x + bary.y <= 1.01f)
							entity->map[index].data[e * entity->map[index].size.x + j] = CLR_RED;
					}
					//entity->map[index].data[e * entity->map[index].size.x + j] = clr;
				}
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
				create_map_for_entity(ent, world);
			}
			found++;
		}
		i++;
	}
}