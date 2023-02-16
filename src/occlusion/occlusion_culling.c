#include "doomnukem.h"

void calculate_triangles(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	render->world_triangles = entity->occlusion.world_tri;
	render->start = 0;
	render->screenspace_ptri_count = 0;
	render->screen_edge.max.x = (float)(sdl.window_w * sdl.resolution_scaling) - 1.0f;
	render->screen_edge.max.y = (float)(sdl.window_h * sdl.resolution_scaling) - 1.0f;
	render->screen_edge.min.x = 0.0f;
	render->screen_edge.min.y = 0.0f;
	if (entity->obj->bounds.type == bt_plane)
		render->end = 1;
	else if (entity->obj->bounds.type == bt_box)
		render->end = 11;
	else if (entity->obj->bounds.type == bt_ignore)
		render->end = 0;
	clipped_point_triangle(render, sdl);
}

void clear_occlusion_buffer(t_sdlcontext *sdl)
{
	t_tile	temp;

	temp.mask = 0;
	temp.max0 = sdl->bitmask.max_dist + 1000.0f;
	temp.max1 = 0;
	for (int i = 0; i < ((sdl->window_h * sdl->window_w) / 64); i++)
	{
		sdl->bitmask.tile[i] = temp;
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
	chunk.max.x = ceilf(chunk.max.x / 8.0f);
	chunk.min.y = chunk.min.y / 8;
	chunk.max.y = ceilf(chunk.max.y / 8.0f);
	entity->occlusion.is_occluded = true;
	entity->occlusion.clip.max.x = 0;
	entity->occlusion.clip.max.y = 0;
	entity->occlusion.clip.min.x = sdl->window_w;
	entity->occlusion.clip.min.y = sdl->window_h;
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
	entity->occlusion.clip.min = point_mul(entity->occlusion.clip.min, 8);
	entity->occlusion.clip.max = point_mul(entity->occlusion.clip.max, 8);
	entity->occlusion.clip.max = point_add_xy(entity->occlusion.clip.max, 8);
	entity->occlusion.clip.max.x = ft_clamp(entity->occlusion.clip.max.x, 0, (sdl->window_w * sdl->resolution_scaling) - 1);
	entity->occlusion.clip.max.y = ft_clamp(entity->occlusion.clip.max.y, 0, (sdl->window_h * sdl->resolution_scaling) - 1);
	entity->occlusion.clip.min.x = ft_clamp(entity->occlusion.clip.min.x, 0, (sdl->window_w * sdl->resolution_scaling) - 1);
	entity->occlusion.clip.min.y = ft_clamp(entity->occlusion.clip.min.y, 0, (sdl->window_h * sdl->resolution_scaling) - 1);
	return (entity->occlusion.is_occluded); // unnecessary to return anything
}

