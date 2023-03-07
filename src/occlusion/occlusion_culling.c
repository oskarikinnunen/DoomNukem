#include "doomnukem.h"

void calculate_triangles(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	render->world_triangles = entity->occlusion.world_tri;
	render->start_index = 0;
	render->screenspace_ptri_count = 0;
	render->screen_edge.max.x = (float)(sdl.window_w * sdl.resolution_scaling) - 1.0f;
	render->screen_edge.max.y = (float)(sdl.window_h * sdl.resolution_scaling) - 1.0f;
	render->screen_edge.min.x = 0.0f;
	render->screen_edge.min.y = 0.0f;
	if (entity->obj->bounds.type == bt_plane)
		render->end_index = 1;
	else if (entity->obj->bounds.type == bt_box)
		render->end_index = 11;
	else
		render->end_index = 2;
	clipped_point_triangle(render, sdl);
}

void clear_occlusion_buffer(t_sdlcontext *sdl)
{
	t_tile	temp;
	int		i;

	temp.mask = 0;
	temp.max0 = sdl->bitmask.max_dist + 1000.0f;
	temp.max1 = 0;
	i = 0;
	while (i < ((sdl->window_h * sdl->window_w) / 64))
	{
		sdl->bitmask.tile[i] = temp;
		i++;
	}
}

bool is_entity_occlusion_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_square	chunk;
	const __uint128_t max = ~0;
	float w;
	int y;
	int x;

	chunk = entity->occlusion.box;
	w = entity->occlusion.z_dist[1];
	chunk.min.x = chunk.min.x / 8;
	chunk.max.x = ceilf((float)chunk.max.x / 8.0f);
	chunk.min.y = chunk.min.y / 8;
	chunk.max.y = ceilf((float)chunk.max.y / 8.0f);
	entity->occlusion.is_occluded = true;
	y = chunk.min.y;
	while (y <= chunk.max.y)
	{
		x = chunk.min.x;
		while (x <= chunk.max.x)
		{
			if (w < sdl->bitmask.tile[y * sdl->bitmask.tile_chunks.x + x].max0)
			{
				entity->occlusion.is_occluded = false;
				if (entity->occlusion.clip.max.x < x)
					entity->occlusion.clip.max.x = x;
				if (entity->occlusion.clip.max.y < y)
					entity->occlusion.clip.max.y = y;
				if (entity->occlusion.clip.min.x > x)
					entity->occlusion.clip.min.x = x;
				if (entity->occlusion.clip.min.y > y)
					entity->occlusion.clip.min.y = y;
			}
			x++;
		}
		y++;
	}
	if (entity->occlusion.is_occluded == false)
	{
		entity->occlusion.clip.max = point_add_xy(entity->occlusion.clip.max, 1);
		entity->occlusion.clip.min = point_mul(entity->occlusion.clip.min, 8);
		entity->occlusion.clip.max = point_mul(entity->occlusion.clip.max, 8);
	}
	return (entity->occlusion.is_occluded); // unnecessary to return anything
}

