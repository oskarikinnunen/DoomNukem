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

static void sample_img(t_lighting *lighting, int x, int y, t_triangle_polygon poly)
{
    t_vector3       loc;
    t_quaternion    q;
	uint32_t		clr;
	uint32_t		light_amount;

	clr = lighting->img[(y % (lighting->map->img_size.y)) * lighting->map->img_size.x + (x % (lighting->map->img_size.x))];
	loc = texcoord_to_loc(poly.p3, poly.uv, (t_vector2){x, y});
	light_amount = get_lighting_for_pixel(&lighting->world->entitycache, loc);
	clr = update_pixel_brightness(ft_clamp(light_amount, 0, 255), clr);
	x = ft_clamp(x, 0, lighting->map->size.x - 1);
	y = ft_clamp(y, 0, lighting->map->size.y - 1);
	lighting->map->data[y * lighting->map->size.x + x] = clr;
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
			t_texture uv;

			uv = entity->world_triangles[i].t[j];
			if (uv.u > max.x)
				max.x = uv.u;
			if (uv.v > max.y)
				max.y = uv.v;
			j++;
		}
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			img = obj->materials[index].img;
			//max.x = fmaxf(max.x, 1.0f);
			//max.y = fmaxf(max.y, 1.0f);
			entity->map[index].size = (t_point){ceilf(max.x) + 10, ceilf(max.y) + 10};
			entity->map[index].img_size = img->size;
			entity->map[index].data = malloc(sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			if (entity->map[index].data == NULL)
				doomlog(LOG_FATAL, "Malloc fail in bake_lighting.c");
			lighting.map = &entity->map[index];
			lighting.img = img->data;
			while (start <= i && 1)
			{
				t_triangle_polygon temp;

				for (int vertex = 0; vertex < 3; vertex++)
				{
					temp.p3[vertex] = entity->world_triangles[start].p[vertex].v;
					temp.p2[vertex].x = (entity->world_triangles[start].t[vertex].u);
					temp.p2[vertex].y = (entity->world_triangles[start].t[vertex].v);
					t_vector2 uv = obj->uvs[obj->faces[start].uv_indices[vertex] - 1];
					temp.uv[vertex] = uv;
					if (temp.p2[vertex].x < 0.0f || temp.p2[vertex].y < 0.0f)
						printf("less than 0 %s \n", obj->name);
					//temp.p2[vertex] = vector2_add_xy(temp.p2[vertex], 0.5f);
				}
				t_vector2 midpoint = vector2_div(vector2_add(vector2_add((temp.p2[0]), (temp.p2[1])), (temp.p2[2])), 3.0f);
				printf("start\n");
				for (int vertex = 0; vertex < 3 && 0; vertex++)
				{
					t_vector2 dir = vector2_normalise(vector2_sub((temp.p2[vertex]), midpoint));
					if (isnan(dir.x) || isnan(dir.y))
					{
						start++;
						continue;
					}
					temp.p2[vertex] = vector2_add((temp.p2[vertex]), dir);
					//temp.p2[vertex].x = ft_clampf(temp.p2[vertex].x, 1.0f, lighting.map->size.x - 1);
					//temp.p2[vertex].y = ft_clampf(temp.p2[vertex].y, 1.0f, lighting.map->size.y - 1);
				}
				printf("end\n");
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
						t_triangle_polygon temp;
						for (int vertex = 0; vertex < 3; vertex++)
						{
							temp.p3[vertex] = entity->world_triangles[x].p[vertex].v;
							temp.p2[vertex].x = (entity->world_triangles[x].t[vertex].u);
							temp.p2[vertex].y = (entity->world_triangles[x].t[vertex].v);
						}
						t_vector2 bary = barycentric_coordinates(temp.p2, (t_vector2){ceilf((float)j + 0.0f), ceilf((float)e + 0.0f)});
						if (bary.x >= -0.01f && bary.y >= -0.01f && bary.x + bary.y <= 1.01f)
							entity->map[index].data[e * entity->map[index].size.x + j] = CLR_RED;
					}
					//entity->map[index].data[e * entity->map[index].size.x + j] = clr;
				}
			}
			start = i;
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