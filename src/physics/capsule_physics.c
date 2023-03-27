/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capsule_physics.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 03:25:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:31:06 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"
#include "collision.h"
#include "editor_tools.h"

bool	touches_edges_ramp(t_vector2 pos, float radius, t_vector3_tri ramp)
{
	t_line	line1;
	t_line	line2;
	t_line	line3;

	line1.start = v3tov2(ramp.a);
	line1.end = v3tov2(ramp.b);
	line2.start = v3tov2(ramp.b);
	line2.end = v3tov2(ramp.c);
	line3.start = v3tov2(ramp.c);
	line3.end = v3tov2(ramp.a);
	return (collision_line_circle(line1, pos, radius)
		|| collision_line_circle(line2, pos, radius)
		|| collision_line_circle(line3, pos, radius));
}

bool	touches_edges(t_vector2 pos, float radius, t_meshtri *floor)
{
	t_line	line1;
	t_line	line2;
	t_line	line3;

	line1.start = v3tov2(floor->v[0]);
	line1.end = v3tov2(floor->v[1]);
	line2.start = v3tov2(floor->v[1]);
	line2.end = v3tov2(floor->v[2]);
	line3.start = v3tov2(floor->v[2]);
	line3.end = v3tov2(floor->v[0]);
	return (collision_line_circle(line1, pos, radius)
		|| collision_line_circle(line2, pos, radius)
		|| collision_line_circle(line3, pos, radius));
}

bool	charphys_ramp_share_z(t_character_physics *cp, t_vector3_tri ramp)
{
	float	fmin;
	float	fmax;

	fmin = ft_minf(ramp.a.z, ft_minf(ramp.b.z, ramp.c.z));
	fmax = ft_maxf(ramp.a.z, ft_maxf(ramp.b.z, ramp.c.z));
	return (fmin <= cp->position->z && fmax >= cp->position->z);
}

void	capsule_damp(t_character_physics *phys, t_world *world)
{
	t_vector2	velocity_xy;

	velocity_xy = v3tov2(phys->velocity);
	velocity_xy = vector2_mul(velocity_xy,
			1.0f - (world->clock.delta * PLAYER_DECELERATION));
	phys->velocity.x = velocity_xy.x;
	phys->velocity.y = velocity_xy.y;
}

void	capsule_add_xy_velocity(t_vector2 vel,
							t_character_physics *phys, t_world *world)
{
	t_vector2	vel_clamped;

	phys->velocity.x += vel.x;
	phys->velocity.y += vel.y;
	vel_clamped = v3tov2(phys->velocity);
	vel_clamped = vector2_clamp_magnitude(vel_clamped, phys->max_velocity);
	phys->velocity.x = vel_clamped.x;
	phys->velocity.y = vel_clamped.y;
}
