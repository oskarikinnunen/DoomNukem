/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:40:03 by vlaine            #+#    #+#             */
/*   Updated: 2022/12/01 13:16:26 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int32_t		get_id(t_world *world)
{
	t_list		*l;
	t_entity	*ent;
	t_wall		wall;
	int32_t		id;
	int			i;
	
	id = 0;
	l = world->roomlist;
	while (l != NULL)
	{
		t_room room = *(t_room *)l->content;
		i = 0;
		while (i < room.wallcount)
		{
			id++;
			i++;
		}
		i = 0;
		while (i < room.floorcount)
		{
			id++;
			i++;
		}
		l = l->next;
	}
	l = world->entitylist;
	while (l != NULL)
	{
		id++;
		l = l->next;
	}
	id++;
	return(id);
}

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

void update_entity_bounds(t_entity *e)
{
	t_object	*obj;
	t_vector3	max;
	t_vector3	min;

	obj = e->obj;
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

//no occlusion for floors/ceilings yet
void update_floor_bounds(t_meshtri *f)
{
	f->entity.obj->bounds.origin = vector3_lerp(vector3_lerp(f->entity.obj->vertices[0], f->entity.obj->vertices[1], 0.5f), f->entity.obj->vertices[2], 0.5f);
	f->entity.obj->bounds.type = bt_ignore;
}

void update_wall_bounds(t_wall *w)
{
	t_object *obj;

	obj = w->entity.obj;
	obj->bounds.box.v[0] = (t_vector3){w->line.start.x, w->line.start.y, 0.0f};
	obj->bounds.box.v[1] = (t_vector3){w->line.end.x, w->line.end.y, 0.0f};
	obj->bounds.box.v[2] = (t_vector3){w->line.start.x, w->line.start.y, w->height};
	obj->bounds.box.v[3] = (t_vector3){w->line.end.x, w->line.end.y, w->height};
	bzero(&obj->bounds.box.v[4], sizeof(t_vector3) * 4);
	obj->bounds.origin = vector3_lerp(obj->bounds.box.v[0], obj->bounds.box.v[3], 0.5f);
	obj->bounds.radius = get_wall_radius(obj->bounds);
	obj->bounds.type = bt_plane;
}

void default_entity_occlusion_settings(t_entity *e, t_world *world)
{
	e->id = get_id(world);
	e->occlusion.is_backface_cull = true;
	e->occlusion.is_occluded = false;
	e->occlusion.occlude = false;
	e->occlusion.cull = true;
}

//id can be same for floors because they arent considered occluders, id is used to check if occluder is trying to cull itself.
void default_floor_occlusion_settings(t_meshtri *f, t_world *world)
{
	f->entity.id = -2;
	f->entity.occlusion.cull = true;
	f->entity.occlusion.occlude = false;
	f->entity.occlusion.is_backface_cull = false;
	f->entity.occlusion.is_occluded = false;
}

//id is not set here, because the current id system takes the id from combining all the lists and calculating their length. and walls get added, in groups, so get_id doesn't update accordingly
void default_wall_occlusion_settings(t_wall *w, t_world *world)
{
	w->entity.occlusion.is_backface_cull = false;
	w->entity.occlusion.is_occluded = false;
	w->entity.occlusion.occlude = true;
	w->entity.occlusion.cull = true;
}

static void update_room_occlusion(t_sdlcontext sdl, t_render *render, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if (render->occlusion.occluder_box == true)
			draw_edges(sdl, render, &room->walls[i].entity, CLR_BLUE);
		if (room->walls[i].entity.occlusion.occlude == true)
		{
			update_occlusion_culling(sdl, render, &room->walls[i].entity);
			update_bitmask_culling(sdl, render, &room->walls[i].entity);
		}
		i++;
	}
}

void update_occlusion(t_sdlcontext sdl, t_render *render)
{
	t_list		*l;
	t_entity	*ent;
	t_wall		wall;
	int			i;

	if (render->occlusion.occlusion == false)
		return;
	l = render->world->roomlist;
	while (l != NULL)
	{
		update_room_occlusion(sdl, render, (t_room *)l->content);
		l = l->next;
	}
	l = render->world->entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		if (render->occlusion.occluder_box == true)
			draw_edges(sdl, render, ent, CLR_BLUE);
		update_occlusion_culling(sdl, render, ent);
		l = l->next;
	}
}

bool is_entity_culled(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	if (render->occlusion.occlusion == false)
		return(false);
	if (is_entity_frustrum_culled(sdl, render, entity) == false)
	{
		if (is_entity_peripheral_culled(sdl, render, entity) == false)
		{
			if (is_entity_bitmask_culled(sdl, render, entity) == false)
			{
				if (entity->occlusion.cull == true)
				{
					if (is_entity_occlusion_culled(sdl, render, entity) == false)
					{
						if (render->occlusion.cull_box == true)
							draw_wireframe(sdl, render, entity, CLR_GREEN);
						return(false);
					}
					else
					{
						if (render->occlusion.cull_box == true)
							draw_wireframe(sdl, render, entity, CLR_RED);
						render->rs.occlusion_cull_amount++;
						return(true);
					}
				}
				else
				{
					return(false);
				}
			}
		}
	}
	return (false);
}
