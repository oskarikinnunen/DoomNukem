/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capsulephysics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 03:25:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 17:30:07 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"
#include "physics.h"
#include "editor_tools.h"
// #include ""

static bool touches_edges_ramp(t_vector2 pos, float radius, t_vector3_tri ramp)
{
	t_line line1;
	t_line line2;
	t_line line3;

	line1.start = v3tov2(ramp.a);
	line1.end = v3tov2(ramp.b);
	line2.start = v3tov2(ramp.b);
	line2.end = v3tov2(ramp.c);
	line3.start = v3tov2(ramp.c);
	line3.end = v3tov2(ramp.a);
	return (collision_line_circle(line1, pos, radius) || collision_line_circle(line2, pos, radius) || collision_line_circle(line3, pos, radius));
}

static bool touches_edges(t_vector2 pos, float radius, t_meshtri *floor)
{
	t_line line1;
	t_line line2;
	t_line line3;

	line1.start = v3tov2(floor->v[0]);
	line1.end = v3tov2(floor->v[1]);
	line2.start = v3tov2(floor->v[1]);
	line2.end = v3tov2(floor->v[2]);
	line3.start = v3tov2(floor->v[2]);
	line3.end = v3tov2(floor->v[0]);
	return (collision_line_circle(line1, pos, radius) || collision_line_circle(line2, pos, radius) || collision_line_circle(line3, pos, radius));
}

static bool charphys_floor_share_z(t_character_physics *cp, t_meshtri *floor)
{
	return (floor->v->z >= cp->position->z && floor->v->z <= cp->position->z + cp->height);
}

static bool charphys_ramp_share_z(t_character_physics *cp, t_vector3_tri ramp)
{
	float fmin;
	float fmax;

	fmin = ft_minf(ramp.a.z, ft_minf(ramp.b.z, ramp.c.z));
	fmax = ft_maxf(ramp.a.z, ft_maxf(ramp.b.z, ramp.c.z));
	return (fmin <= cp->position->z && fmax >= cp->position->z);
}

static bool charphys_ceil_share_z(t_character_physics *cp, t_meshtri *floor)
{
	return (floor->v->z >= cp->position->z + (cp->height / 2.0f) && floor->v->z <= cp->position->z + cp->height);
}

static bool is_in_ceil(t_character_physics *cp, t_meshtri *ceil)
{
	t_ray r;
	t_raycastinfo info;

	r.origin = vector3_add(*cp->position, (t_vector3){.z = 100.0f});
	r.dir = (t_vector3){.z = -1.0f};
	info.distance = 10000.0f;
	if (!ceil->entity)
		return (false);
	if ((raycast_entity(r, &info, ceil->entity) || touches_edges(v3tov2(*cp->position), cp->radius, ceil)) && charphys_ceil_share_z(cp, ceil))
	{
		return (true);
	}
	return (false);
}

static bool is_in_floor(t_character_physics *cp, t_meshtri *floor)
{
	t_ray r;
	t_raycastinfo info;

	r.origin = vector3_add(*cp->position, (t_vector3){.z = 100.0f});
	r.dir = (t_vector3){.z = -1.0f};
	info.distance = 10000.0f;
	if (!floor->entity)
		return (false);
	if ((raycast_entity(r, &info, floor->entity) || touches_edges(v3tov2(*cp->position), cp->radius, floor)) && charphys_floor_share_z(cp, floor))
	{
		return (true);
	}
	return (false);
}

static bool is_in_ramp(t_character_physics *cp, t_vector3_tri *ramp)
{
	t_ray r;
	t_raycastinfo info;

	r.origin = vector3_add(*cp->position, (t_vector3){.z = 100.0f});
	r.dir = (t_vector3){.z = -1.0f};
	info.distance = 10000.0f;
	if ((raycast_tri(r, *ramp, &info.distance) || touches_edges_ramp(v3tov2(*cp->position), cp->radius, *ramp)) && charphys_ramp_share_z(cp, *ramp))
	{
		return (true);
	}
	return (false);
}

static float get_z_from_areas(t_character_physics *cp, t_world *world)
{
	t_list *list;
	t_area *area;
	int i;
	float z;

	z = 0;
	list = world->arealist;
	while (list != NULL)
	{
		area = (t_area *)list->content;
		i = 0;
		while (i < area->floorcount)
		{
			if (is_in_floor(cp, &area->floors[i]))
				z = ft_maxf(z, (float)area->height);
			i++;
		}
		list = list->next;
	}
	return (z);
}

static float sample_ramp_z(t_vector2 position, t_vector3_tri tri, int mod)
{
	float ff;
	float trihigh;
	float trilow;
	t_line line;
	float rampz;

	trihigh = ft_maxf(ft_maxf(tri.a.z, tri.b.z), tri.c.z);
	trilow = ft_minf(ft_minf(tri.a.z, tri.b.z), tri.c.z);
	if (mod == 0)
	{
		line.start = v3tov2(tri.c);
		line.end = v3tov2(tri.a);
	}
	else
	{
		line.start = v3tov2(tri.c);
		line.end = v3tov2(tri.b);
	}
	ff = vector2_dist_along_line(position, line);
	rampz = ft_flerp(trilow, trihigh, ff);
	return (rampz);
}

static float get_z_from_ramps(t_character_physics *cp, t_world *world)
{
	t_vector3_tri *tri;
	t_list *list;
	float z;
	int i;

	list = world->ramps;
	z = 0.0f;
	i = 0;
	while (list != NULL)
	{
		tri = (t_vector3_tri *)list->content;
		if (is_in_ramp(cp, tri))
			z = ft_maxf(z, sample_ramp_z(v3tov2(*cp->position), *tri, i % 2));
		i++;
		list = list->next;
	}
	return (z);
}

// get_z_from_areas
// get_z_from_ramps
static float get_z_floor(t_character_physics *cp, t_world *world)
{
	float z;

	z = 0.0f;
	z = ft_maxf(z, get_z_from_areas(cp, world));
	z = ft_maxf(z, get_z_from_ramps(cp, world));
	return (z);
}

static float get_z_ceil(t_character_physics *cp, t_world *world)
{
	t_list *list;
	t_area *room;
	int i;
	float z;

	z = INFINITY;
	list = world->arealist;
	while (list != NULL)
	{
		room = (t_area *)list->content;
		i = 0;
		while (i < room->ceilingcount)
		{
			if (is_in_ceil(cp, &room->ceilings[i]))
				z = ft_minf(z, (float)(room->height + room->ceiling_height));
			i++;
		}
		list = list->next;
	}
	return (z);
}

t_bound get_bound(t_character_physics *cp, t_world *world)
{
	t_bound bound;

	bound.min = get_z_floor(cp, world);
	bound.max = get_z_ceil(cp, world);
	return (bound);
}

void capsule_damp(t_character_physics *phys, t_world *world)
{
	t_vector2 velocity_xy;

	velocity_xy = v3tov2(phys->velocity);
	velocity_xy = vector2_mul(velocity_xy,
							  1.0f - (world->clock.delta * PLAYER_DECELERATION));
	phys->velocity.x = velocity_xy.x;
	phys->velocity.y = velocity_xy.y;
}

void capsule_add_xy_velocity(t_vector2 vel,
							 t_character_physics *phys, t_world *world)
{
	t_vector2 vel_clamped;

	phys->velocity.x += vel.x;
	phys->velocity.y += vel.y;
	vel_clamped = v3tov2(phys->velocity);
	vel_clamped = vector2_clamp_magnitude(vel_clamped, phys->max_velocity);
	phys->velocity.x = vel_clamped.x;
	phys->velocity.y = vel_clamped.y;
}

// TODO: norminette for this...
void capsule_applygravity_new(t_character_physics *charp, t_world *world)
{
	t_vector3 potential_pos;
	t_vector3 new_pos;
	t_bound zbound;

	potential_pos = vector3_add(*charp->position,
								vector3_mul(charp->velocity, world->clock.delta));
	new_pos = potential_pos;
	int i = 0;
	while (check_character_collision(world, *charp, new_pos, &new_pos) && i < 5)
		i++;
	if (!check_character_collision(world, *charp, new_pos, &new_pos))
		*charp->position = new_pos;
	zbound = get_bound(charp, world);
	float floorz = zbound.min;
	if (zbound.max <= charp->position->z + charp->height && charp->velocity.z < 0.0f)
	{
		charp->velocity.z = 0.0f;
		charp->ceilingtrigger = true;
	}
	if (floorz <= charp->position->z)
	{
		charp->isgrounded = (charp->position->z <= floorz);
		float target_z = floorz;
		float gravityapply = 0.0012f;
		float zveltarget;
		zveltarget = GRAVITY;
		if (charp->gravity_override != NULL)
			zveltarget = *charp->gravity_override;
		if (charp->isgrounded)
			zveltarget = 0.0f;
		charp->velocity.z = ft_fmovetowards(charp->velocity.z, zveltarget, gravityapply * world->clock.delta);
		if (charp->velocity.z <= 0.0f)
			target_z = floorz;
		else
			target_z = 10000.0f;
		charp->position->z = ft_fmovetowards(charp->position->z, target_z, ft_absf(charp->velocity.z) * world->clock.delta);
		charp->position->z = ft_clampf(charp->position->z, floorz, zbound.max - charp->height);
		charp->isgrounded = (charp->position->z <= floorz);
	}
	else
		charp->position->z = ft_fmovetowards(charp->position->z, floorz, world->clock.delta * -GRAVITY * 0.5f);
	if (charp->isgrounded && charp->velocity.z != 0)
	{
		charp->landingtrigger = true;
		charp->impactvelocity.z = charp->velocity.z;
		charp->velocity.z = 0.0f;
	}
	charp->isgrounded = (charp->position->z <= floorz);
	capsule_damp(charp, world);
}
