#include "doomnukem.h"

void create_lightmap_for_entity(t_entity *entity, struct s_world *world)
{
	t_object *obj;
	t_vector2 max;
	int i;
	int j;

	obj = entity->obj;
	//printf("obj mat count %i \n", obj->material_count);
	entity->lightmap = ft_memalloc(sizeof(t_lightmap) * obj->material_count);
	max.x = -10000.0f;
	max.y = -10000.0f;
	i = 0;
	while (i < obj->face_count && obj->uv_count != 0)
	{
		j = 0;
		while (j < 3)
		{
			t_vector2 uv;

			uv = obj->uvs[obj->faces[i].uv_indices[j] - 1];
			//printf("face %i uv: %f %f \n", i, uv.x, uv.y);
			if (uv.x > max.x)
				max.x = uv.x;
			if (uv.y > max.y)
				max.y = uv.y;
			j++;
		}
		if (i + 1 == obj->face_count || obj->faces[i].materialindex != obj->faces[i + 1].materialindex)
		{
			t_lightmap *lightmap;
			t_img *img;
			lightmap = &entity->lightmap[obj->faces[i].materialindex];
			lightmap->progress = 0;
			img = obj->materials[obj->faces[i].materialindex].img;
			if (!img)
				return;
			if (max.x > 1.0f)
				lightmap->size.x = max.x * img->size.x;
			else
				lightmap->size.x = img->size.x;
			if (max.y > 1.0f)
			{
				lightmap->size.y = max.y * img->size.y;
				//printf("max y exceeds 1.0f, max y: %f, img size.y %i\n", max.y, img->size.y);
			}
				
			else
				lightmap->size.y = img->size.y;
			lightmap->data = malloc(sizeof(uint8_t) * lightmap->size.x * lightmap->size.y);
			//printf("lighting ambient is %i \n", world->lighting.ambient_light);
			/*printf("max %f x %f \n", max.x, max.y);
			printf("lightmap size: %i x %i \n", lightmap->size.x, lightmap->size.y);*/
			memset(lightmap->data, world->lighting.ambient_light, lightmap->size.x * lightmap->size.y);
			// ft_bzero(lightmap->data, sizeof(uint8_t) * lightmap->size.x * lightmap->size.y);
			max.x = -10000.0f;
			max.y = -10000.0f;
		}
		i++;
	}
}

void create_dynamic_map_for_entity(t_entity *entity, struct s_world *world)
{
	t_object *obj;
	t_lightmap *lightmap;
	t_img *img;
	uint8_t light;

	int index;
	int i;

	light = 0;
	obj = entity->obj;
	if (entity->map == NULL)
		entity->map = malloc(sizeof(t_map) * entity->obj->material_count);
	if (obj->uv_count == 0)
	{
		free(entity->map);
		entity->map = NULL;
	}
	i = 0;
	while (i < obj->face_count && obj->uv_count != 0)
	{
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			lightmap = &entity->lightmap[index];
			light = lightmap->dynamic_data;
			img = obj->materials[index].img;
			entity->map[index].size.x = lightmap->size.x;
			entity->map[index].size.y = lightmap->size.y;
			if (!img)
			{
				entity->map = NULL;
				return;
			}
			entity->map[index].img_size = img->size;
			if (entity->map[index].data == NULL)
				entity->map[index].data = malloc(sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			for (int e = 0; e < entity->map[index].size.y; e++)
			{
				for (int j = 0; j < entity->map[index].size.x; j++)
				{
					uint32_t clr;
					clr = img->data[(e % (img->size.y)) * img->size.x + (j % (img->size.x))];
					Uint32 alpha = clr & 0xFF000000;
					Uint32 red = ((clr & 0x00FF0000) * light) >> 8;
					Uint32 green = ((clr & 0x0000FF00) * light) >> 8;
					Uint32 blue = ((clr & 0x000000FF) * light) >> 8;
					clr = flip_channels(alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF));
					entity->map[index].data[e * entity->map[index].size.x + j] = clr;
				}
			}
		}
		i++;
	}
}

uint8_t	average(t_point sample, t_lightmap *lmap, t_map *map)
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
	//printf("hit %i \n", hit);
	//if (hit == 0)
	//    return (lmap->data[sample.x * lmap->size.x + sample.y]);
	return ((uint8_t)(avg / hit));
}

/*uint8_t *smooth_lightmap(t_lightmap *lmap)
{
	t_point	sample;
	uint8_t *temp;

	sample = point_zero();
	temp = ft_memdup(lmap->data, lmap->size.x * lmap->size.y);
	while (sample.y < lmap->size.y)
	{
		sample.x = 0;
		while (sample.x < lmap->size.y)
		{
			temp[sample.x + (sample.y * lmap->size.x)] = average(sample, lmap);
			sample.x++;
		}
		sample.y++;
	}
	return (temp);
}*/

void create_map_for_entity(t_entity *entity, struct s_world *world)
{
	t_object *obj;
	t_lightmap *lightmap;
	t_img *img;
	int index;
	int i;

	obj = entity->obj;
	entity->map = malloc(sizeof(t_map) * entity->obj->material_count);
	
	if (obj->uv_count == 0)
	{
		free(entity->map);
		entity->map = NULL;
	}
	i = 0;
	while (i < obj->face_count && obj->uv_count != 0)
	{
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			lightmap = &entity->lightmap[index];
			img = obj->materials[index].img;
			entity->map[index].size.x = lightmap->size.x;
			entity->map[index].size.y = lightmap->size.y;
			if (!img)
			{
				entity->map = NULL;
				return;
			}
			entity->map[index].img_size = img->size;
			entity->map[index].data = malloc(sizeof(uint32_t) * entity->map[index].size.x * entity->map[index].size.y);
			for (int e = 0; e < entity->map[index].size.y; e++)
			{
				for (int j = 0; j < entity->map[index].size.x; j++)
				{
					uint32_t clr;
					clr = img->data[(e % (img->size.y)) * img->size.x + (j % (img->size.x))];
					uint8_t light = average((t_point){j, e}, lightmap, entity->map);
					//uint8_t light = //lightmap->data[e * entity->map[index].size.x + j];
					Uint32 alpha = clr & 0xFF000000;
					Uint32 red = ((clr & 0x00FF0000) * (uint8_t)(light * 0.4f)) >> 8;
					Uint32 green = ((clr & 0x0000FF00) * (uint8_t)(light * 0.5f)) >> 8;
					Uint32 blue = ((clr & 0x000000FF) * (uint8_t)(light * 0.9f)) >> 8;
					clr = flip_channels(alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF));
					entity->map[index].data[e * entity->map[index].size.x + j] = clr;
					/*light = light / 2;
					if (e - 1 > 0 && lightmap->data[(e - 1) * entity->map[index].size.x + j] < light)
					{
						clr = img->data[((e - 1) % (img->size.y)) * img->size.x + (j % (img->size.x))];
						Uint32 alpha = clr & 0xFF000000;
						Uint32 red = ((clr & 0x00FF0000) * light) >> 8;
						Uint32 green = ((clr & 0x0000FF00) * light) >> 8;
						Uint32 blue = ((clr & 0x000000FF) * light) >> 8;
						clr = flip_channels(alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF));
						entity->map[index].data[(e - 1) * entity->map[index].size.x + j] = clr;
					}
					if (e + 1 < entity->map[index].size.y && lightmap->data[(e + 1) * entity->map[index].size.x + j] < light)
					{
						clr = img->data[((e + 1) % (img->size.y)) * img->size.x + (j % (img->size.x))];
						Uint32 alpha = clr & 0xFF000000;
						Uint32 red = ((clr & 0x00FF0000) * light) >> 8;
						Uint32 green = ((clr & 0x0000FF00) * light) >> 8;
						Uint32 blue = ((clr & 0x000000FF) * light) >> 8;
						clr = flip_channels(alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF));
						entity->map[index].data[(e + 1) * entity->map[index].size.x + j] = clr;
					}
					if (j + 1 < entity->map[index].size.x && lightmap->data[e * entity->map[index].size.x + (j + 1)] < light)
					{
						clr = img->data[(e % (img->size.y)) * img->size.x + ((j + 1) % (img->size.x))];
						Uint32 alpha = clr & 0xFF000000;
						Uint32 red = ((clr & 0x00FF0000) * light) >> 8;
						Uint32 green = ((clr & 0x0000FF00) * light) >> 8;
						Uint32 blue = ((clr & 0x000000FF) * light) >> 8;
						clr = flip_channels(alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF));
						entity->map[index].data[e * entity->map[index].size.x + (j + 1)] = clr;
					}
					if (j - 1 > 0 && lightmap->data[e * entity->map[index].size.x + (j - 1)] < light)
					{
						clr = img->data[(e % (img->size.y)) * img->size.x + ((j - 1) % (img->size.x))];
						Uint32 alpha = clr & 0xFF000000;
						Uint32 red = ((clr & 0x00FF0000) * light) >> 8;
						Uint32 green = ((clr & 0x0000FF00) * light) >> 8;
						Uint32 blue = ((clr & 0x000000FF) * light) >> 8;
						clr = flip_channels(alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF));
						entity->map[index].data[e * entity->map[index].size.x + (j - 1)] = clr;
					}*/
				}
			}
		}
		i++;
	}
}

void start_lightbake(t_render *render, t_world *world)
{
	int i;
	t_entity *ent;
	uint32_t time_start;
	uint32_t time_end;
	uint32_t time;
	//uint8_t yes : 2;

	world->lights_count = 1;
	world->lights[0].origin = world->player->transform.position;
	world->lights[0].radius = 250.0f;
	world->lights[0].shadows = true;
	for_all_entities(world, create_lightmap_for_entity);
	i = 0;
	while (i < world->lights_count)
	{
		world->lights[i].done = false;
		i++;
	}
	world->lighting_baked = false;
	// for_all_entities(world, create_map_for_entity);

	/*i = 0;
	while (i < world->lights_count)
	{
		calculate_pointlight(&world->lights[i], world, render);
		i++;
	}*/

	/*time_end = SDL_GetTicks();
	time = time_end - time_start;*/
	printf("lighting start tick: %i\n", SDL_GetTicks());
}

void	save_entity_lightmap(t_entity *entity)
{
	//entity->map.
}

void dynamic_light(t_entity *entity, t_world *world)
{
	uint8_t	light;
	//world->lights[0].origin = world->player->transform.position;
	/*if (entity->lightmap == NULL)
		create_lightmap_for_entity(entity, world);*/
	if (entity->lightmap != NULL && entity->lightmap->dynamic)
	{
		light = 0;
		float dist = vector3_dist(entity->transform.position, world->lights[0].origin);
		if (dist <= world->lights[0].radius /*&& l.drawnbuff[x + l.lightmap->size.x * y] == false*/)
		{
			dist = 1.0f - (dist / world->lights[0].radius);
			light = ft_clamp((dist * 255), 0, 255);
		}
		entity->lightmap->dynamic_data = light;
		//entity->lightmap->dynamic_data = 255 - entity->lightmap->dynamic_data;
		//create_lightmap_for_entity(entity, world);
		create_dynamic_map_for_entity(entity, world);
	}
}

void bake_lights(t_render *render, t_world *world)
{
	int i;
	i = 0;

	for_all_active_entities(world, dynamic_light);
	if (world->lighting_baked)
		return;
	i = 0;
	while (i < world->lights_count)
	{
		if (!world->lights[i].done)
		{
			calculate_pointlight_step(&world->lights[i], world, render);
			if (i == world->lights_count - 1 && world->lights[i].done)
			{
				world->lighting_baked = true;
				printf("bake light done %i \n", SDL_GetTicks());
				// for_all_entities(world, create_map_for_entity);
			}
			break;
		}
		i++;
	}
}

void bake_lighting_shadows(t_render *render, t_world *world)
{
	int i;
	t_entity *ent;
	uint32_t time_start;
	uint32_t time_end;
	uint32_t time;

	time_start = SDL_GetTicks();
	for_all_entities(world, create_lightmap_for_entity);
	// world->lights_count = 1;
	// world->lights[0].origin = (t_vector3){500.0f, 500.0f, 50.0f};
	world->lights_count++;

	world->lights[world->lights_count - 1].origin = world->player->transform.position;
	world->lights[world->lights_count - 1].radius = 250.0f;
	world->lights[world->lights_count - 1].shadows = true;
	/*world->lights[1].origin = (t_vector3){500.0f, 20.0f, 50.0f};
	world->lights[1].radius = 500.0f;
	world->lights[1].shadows = true;
	world->lights[2].origin = (t_vector3){500.0f, 500.0f, 500.0f};
	world->lights[2].radius = 5000.0f;
	world->lights[2].shadows = true;*/
	i = 0;
	while (i < world->lights_count)
	{
		calculate_pointlight(&world->lights[i], world, render);
		i++;
	}

	for_all_entities(world, create_map_for_entity);
	time_end = SDL_GetTicks();
	time = time_end - time_start;
	printf("lighting took %i seconds, %i ticks \n", time / 1000, time);
}

void bake_lighting(t_render *render, t_world *world)
{
	int i;
	t_entity *ent;
	uint32_t time_start;
	uint32_t time_end;
	uint32_t time;

	time_start = SDL_GetTicks();
	for_all_entities(world, create_lightmap_for_entity);
	world->lights_count++; // = 1;
	// world->lights[0].origin = (t_vector3){500.0f, 500.0f, 50.0f};
	world->lights[world->lights_count - 1].origin = world->player->transform.position;
	world->lights[world->lights_count - 1].radius = 250.0f;
	world->lights[world->lights_count - 1].shadows = false;
	/*world->lights[1].origin = (t_vector3){500.0f, 20.0f, 50.0f};
	world->lights[1].radius = 500.0f;
	world->lights[1].shadows = false;
	world->lights[2].origin = (t_vector3){500.0f, 500.0f, 500.0f};
	world->lights[2].radius = 5000.0f;
	world->lights[2].shadows = false;*/
	i = 0;
	while (i < world->lights_count)
	{
		calculate_pointlight(&world->lights[i], world, render);
		i++;
	}
	for_all_entities(world, create_map_for_entity);
	world->lighting_baked = true;
	time_end = SDL_GetTicks();
	time = time_end - time_start;
	printf("lighting took %i seconds, %i ticks \n", 1000 / time, time);
}