#include "doomnukem.h"

void create_lightmap_for_entity(t_entity *entity, struct s_world *world)
{
	t_object		*obj;
	t_vector2		max;
	int				i;
	int				j;

	obj = entity->obj;
	entity->lightmap = malloc(sizeof(t_lightmap) * obj->material_count);
	max.x = -10000.0f;
	max.y = -10000.0f;
	i = 0;
	while (i < obj->face_count && obj->uv_count != 0)
	{
		j = 0;
		while (j < 3)
		{
			t_vector2	uv;

			uv = obj->uvs[obj->faces[i].uv_indices[j] - 1];
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
			img = obj->materials[obj->faces[i].materialindex].img;
			if (!img)
				return;
			if (max.x > 1.0f)
				lightmap->size.x = max.x * img->size.x;
			else
				lightmap->size.x = img->size.x;
			if (max.y > 1.0f)
				lightmap->size.y = max.y * img->size.y;
			else
				lightmap->size.y = img->size.y;
			lightmap->data = malloc(sizeof(uint8_t) * lightmap->size.x * lightmap->size.y);
			memset(lightmap->data, world->sdl->ambient_light, sizeof(uint8_t) * lightmap->size.x * lightmap->size.y);
			max.x = -10000.0f;
			max.y = -10000.0f;
		}
		i++;
	}
}

void create_map_for_entity(t_entity *entity, struct s_world *world)
{
	t_object		*obj;
	t_lightmap		*lightmap;
	t_img			*img;
	int				index;
	int				i;

	obj = entity->obj;
	entity->map	= malloc(sizeof(t_map) * entity->obj->material_count);
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
			if(!img)
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
					uint32_t	clr;
					clr = img->data[(e % (img->size.y)) * img->size.x + (j % (img->size.x))];
					uint8_t light = lightmap->data[e * entity->map[index].size.x + j];
					Uint32 alpha = clr&0xFF000000;
					Uint32 red = ((clr&0x00FF0000)*light)>>8;
					Uint32 green = ((clr&0x0000FF00)*light)>>8;
					Uint32 blue = ((clr&0x000000FF)*light)>>8;
					clr = flip_channels(alpha|(red&0x00FF0000)|(green&0x0000FF00)|(blue&0x000000FF));
					entity->map[index].data[e * entity->map[index].size.x + j] = clr;
					light = light / 2;
					if (e - 1 > 0 && lightmap->data[(e - 1) * entity->map[index].size.x + j] < light)
					{
						clr = img->data[((e - 1) % (img->size.y)) * img->size.x + (j % (img->size.x))];
						Uint32 alpha = clr&0xFF000000;
						Uint32 red = ((clr&0x00FF0000)*light)>>8;
						Uint32 green = ((clr&0x0000FF00)*light)>>8;
						Uint32 blue = ((clr&0x000000FF)*light)>>8;
						clr = flip_channels(alpha|(red&0x00FF0000)|(green&0x0000FF00)|(blue&0x000000FF));
						entity->map[index].data[(e - 1) * entity->map[index].size.x + j] = clr;
					}
					if (e + 1 < entity->map[index].size.y && lightmap->data[(e + 1) * entity->map[index].size.x + j] < light)
					{
						clr = img->data[((e + 1) % (img->size.y)) * img->size.x + (j % (img->size.x))];
						Uint32 alpha = clr&0xFF000000;
						Uint32 red = ((clr&0x00FF0000)*light)>>8;
						Uint32 green = ((clr&0x0000FF00)*light)>>8;
						Uint32 blue = ((clr&0x000000FF)*light)>>8;
						clr = flip_channels(alpha|(red&0x00FF0000)|(green&0x0000FF00)|(blue&0x000000FF));
						entity->map[index].data[(e + 1) * entity->map[index].size.x + j] = clr;
					}
					if (j + 1 < entity->map[index].size.x && lightmap->data[e * entity->map[index].size.x + (j + 1)] < light)
					{
						clr = img->data[(e % (img->size.y)) * img->size.x + ((j + 1) % (img->size.x))];
						Uint32 alpha = clr&0xFF000000;
						Uint32 red = ((clr&0x00FF0000)*light)>>8;
						Uint32 green = ((clr&0x0000FF00)*light)>>8;
						Uint32 blue = ((clr&0x000000FF)*light)>>8;
						clr = flip_channels(alpha|(red&0x00FF0000)|(green&0x0000FF00)|(blue&0x000000FF));
						entity->map[index].data[e * entity->map[index].size.x + (j + 1)] = clr;
					}
					if (j - 1 > 0 && lightmap->data[e * entity->map[index].size.x + (j - 1)] < light)
					{
						clr = img->data[(e % (img->size.y)) * img->size.x + ((j - 1) % (img->size.x))];
						Uint32 alpha = clr&0xFF000000;
						Uint32 red = ((clr&0x00FF0000)*light)>>8;
						Uint32 green = ((clr&0x0000FF00)*light)>>8;
						Uint32 blue = ((clr&0x000000FF)*light)>>8;
						clr = flip_channels(alpha|(red&0x00FF0000)|(green&0x0000FF00)|(blue&0x000000FF));
						entity->map[index].data[e * entity->map[index].size.x + (j - 1)] = clr;
					}
				}
			}
		}
		i++;
	}
}

void bake_lighting(t_render *render, t_world *world)
{
	int			i;
	t_entity	*ent;

	for_all_entities(world, create_lightmap_for_entity);
	world->lights_count = 2;
	world->lights[0].origin = (t_vector3){500.0f, 500.0f, 50.0f};
	world->lights[0].radius = 500.0f;
	world->lights[0].shadows = false;
	world->lights[1].origin = (t_vector3){500.0f, 20.0f, 50.0f};
	world->lights[1].radius = 500.0f;
	world->lights[1].shadows = false;
	world->lights[2].origin = (t_vector3){500.0f, 500.0f, 500.0f};
	world->lights[2].radius = 5000.0f;
	world->lights[2].shadows = false;
	i = 0;
	while (i < world->lights_count)
	{
		calculate_pointlight(&world->lights[i], world, render);
		i++;
	}
	for_all_entities(world, create_map_for_entity);
}