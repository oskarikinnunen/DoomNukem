/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:40:03 by vlaine            #+#    #+#             */
/*   Updated: 2023/01/23 04:41:41 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void set_bounding_box(t_vector3 *max, t_vector3 *min, t_vector3 *vector, int count)
{
	t_vector3 v;
	int	i;

	max->x = -1000000000;
	max->y = -1000000000;
	max->z = -1000000000;
	min->x = 1000000000;
	min->y = 1000000000;
	min->z = 1000000000;
	i = 0;
	while (i < count)
	{
		v = vector[i];
		if (v.x < min->x)
			min->x = v.x;
		if (v.x > max->x)
			max->x = v.x;
		if (v.y < min->y)
			min->y = v.y;
		if (v.y > max->y)
			max->y = v.y;
		if (v.z < min->z)
			min->z = v.z;
		if (v.z > max->z)
			max->z = v.z;
		i++;
	}
}

static float get_box_sphere_radius(t_vector3 origin, t_vector3 max, t_vector3 min)
{
	float	min_radius;
	float	max_radius;

	min_radius = vector3_dist(origin, min);
	max_radius = vector3_dist(origin, max);
	if (max_radius > min_radius)
		return(max_radius);
	else
		return(min_radius);
}

static float get_wall_radius(t_bounds bounds)
{
	float min_radius;
	float max_radius;

	min_radius = vector3_dist(bounds.origin, bounds.box.v[0]);
	max_radius = vector3_dist(bounds.origin, bounds.box.v[3]);
	if (max_radius > min_radius)
		return(max_radius);
	else
		return(min_radius);
}

void update_object_bounds(t_object *obj)
{
	t_vector3	max;
	t_vector3	min;

	set_bounding_box(&max, &min, obj->vertices, obj->vertice_count);
	obj->bounds.origin = vector3_lerp(max, min, 0.5f);
	obj->bounds.radius = get_box_sphere_radius(obj->bounds.origin, max, min);
	obj->bounds.box.v[0] = (t_vector3){.x = max.x, .y = max.y, min.z};
	obj->bounds.box.v[1] = (t_vector3){.x = max.x, .y = min.y, min.z};
	obj->bounds.box.v[2] = (t_vector3){.x = min.x, .y = min.y, min.z};
	obj->bounds.box.v[3] = (t_vector3){.x = min.x, .y = max.y, min.z};
	obj->bounds.box.v[4] = (t_vector3){.x = max.x, .y = max.y, max.z};
	obj->bounds.box.v[5] = (t_vector3){.x = max.x, .y = min.y, max.z};
	obj->bounds.box.v[6] = (t_vector3){.x = min.x, .y = min.y, max.z};
	obj->bounds.box.v[7] = (t_vector3){.x = min.x, .y = max.y, max.z};
	obj->bounds.type = bt_box;
}

void update_floor_bounds(t_meshtri *f)
{
	float	dist;
	int		i;

	f->entity->obj->bounds.origin = vector3_div(vector3_add(vector3_add(f->entity->obj->vertices[0], f->entity->obj->vertices[1]), f->entity->obj->vertices[2]), 3.0f);
	dist = vector3_dist(f->entity->obj->vertices[0], f->entity->obj->vertices[1]);
	i = 1;
	while (i < 3)
	{
		if (dist < vector3_dist(f->entity->obj->vertices[i], f->entity->obj->vertices[(i + 1) % 3]))
		{
			dist = vector3_dist(f->entity->obj->vertices[i], f->entity->obj->vertices[(i + 1) % 3]);
		}
		i++;
	}
	f->entity->obj->bounds.radius = dist;
	f->entity->obj->bounds.type = bt_ignore;
}

void update_wall_bounds(t_wall *w)
{
	t_object	*obj;
	t_vector2	start;
	t_vector2	end;

	obj = w->entity->obj;
	start = *w->edgeline.start;
	end = *w->edgeline.end;

	int	i = 0;
	while (i < 4)
	{
		obj->bounds.box.v[i] = obj->vertices[i];
		i++;
	}
	/*obj->bounds.box.v[0] = (t_vector3){start.x, start.y, 0.0f};
	obj->bounds.box.v[1] = (t_vector3){end.x, end.y, 0.0f};
	obj->bounds.box.v[2] = (t_vector3){start.x, start.y, w->height};
	obj->bounds.box.v[3] = (t_vector3){end.x, end.y, w->height};*/
	bzero(&obj->bounds.box.v[4], sizeof(t_vector3) * 4);
	obj->bounds.origin = vector3_lerp(obj->bounds.box.v[0], obj->bounds.box.v[3], 0.5f);
	obj->bounds.radius = get_wall_radius(obj->bounds);
	obj->bounds.type = bt_plane;
}

void default_entity_occlusion_settings(t_entity *e, t_world *world)
{
	e->occlusion.is_backface_cull = true;
	e->occlusion.is_occluded = false;
}

void default_floor_occlusion_settings(t_meshtri *f, t_world *world)
{
	f->entity->occlusion.is_backface_cull = false;
	f->entity->occlusion.is_occluded = false;
}

void default_wall_occlusion_settings(t_wall *w, t_world *world)
{
	w->entity->occlusion.is_backface_cull = false;
	w->entity->occlusion.is_occluded = false;
}

t_bitmask	init_bitmask(t_sdlcontext *sdl)
{
	t_bitmask	bitmask;
	
	bitmask.tile = malloc(sizeof(t_tile) * ((sdl->window_h * sdl->window_w) / 64));
	bitmask.bitmask_chunks.x = sdl->window_w / 16;
	bitmask.bitmask_chunks.y = sdl->window_h / 8;
	bitmask.tile_chunks.x = sdl->window_w / 8;
	bitmask.tile_chunks.y = sdl->window_h / 8;
	return(bitmask);
}

bool is_entity_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	uint32_t	clr;

	if (render->occlusion.occlusion == false)
		return(false);
	if (entity->occlusion.is_occluded == false)
	{
		entity->occlusion.is_occluded = is_entity_occlusion_culled(sdl, render, entity);
		if (render->occlusion.occluder_box == true)
		{
			if (entity->occlusion.is_occluded == true)
				clr = CLR_RED;
			else
			{
				draw_wireframe(*sdl, entity->occlusion.clip, CLR_TURQ);
				clr = CLR_GREEN;
			}
			draw_wireframe(*sdl, entity->occlusion.box, clr);
		}
		if (sdl->render.occlusion.slow_render == true)
		{
			memcpy(sdl->window_surface->pixels, sdl->surface->pixels, sizeof(uint32_t) * sdl->window_w * sdl->window_h);
			SDL_UpdateWindowSurface(sdl->window);
			usleep(100000);
		}
	}
	return(entity->occlusion.is_occluded);
}
