/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capsule_physics_step.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 18:55:30 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 22:58:02 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"
#include "collision.h"
#include "editor_tools.h"

static void	move_towards_target_z(t_character_physics *charp,
		t_bound bound, uint32_t delta)
{
	float	target_z;

	if (charp->velocity.z <= 0.0f)
		target_z = bound.min;
	else
		target_z = 10000.0f;
	charp->position->z = ft_fmovetowards(charp->position->z, target_z,
			ft_absf(charp->velocity.z) * delta);
	charp->position->z = ft_clampf(charp->position->z,
			bound.min, bound.max - charp->height);
	charp->is_grounded = (charp->position->z <= bound.min);
}

static void	capsule_phys_go_towards_floor(t_character_physics *charp,
		t_bound bound, uint32_t delta)
{
	const float	gravityapply = 0.0012f;
	float		zveltarget;

	if (bound.min <= charp->position->z)
	{
		charp->is_grounded = (charp->position->z <= bound.min);
		zveltarget = GRAVITY;
		if (charp->gravity_override != NULL)
			zveltarget = *charp->gravity_override;
		if (charp->is_grounded)
			zveltarget = 0.0f;
		charp->velocity.z = ft_fmovetowards(charp->velocity.z, zveltarget,
				gravityapply * delta);
		move_towards_target_z(charp, bound, delta);
	}
	else
		charp->position->z = ft_fmovetowards(charp->position->z,
				bound.min, delta * -GRAVITY * 0.5f);
}

static void	capsule_phys_2d_apply_pos(t_character_physics *charp,
									t_world *world)
{
	t_vector3	potential_pos;
	t_vector3	new_pos;
	int			i;

	potential_pos = vector3_add(*charp->position,
			vector3_mul(charp->velocity, world->clock.delta));
	new_pos = potential_pos;
	i = 0;
	while (check_character_collision(world, *charp, new_pos, &new_pos) && i < 5)
		i++;
	if (!check_character_collision(world, *charp, new_pos, &new_pos))
		*charp->position = new_pos;
}

void	capsule_phys_step(t_character_physics *charp, t_world *world)
{
	t_bound	zbound;

	capsule_phys_2d_apply_pos(charp, world);
	zbound = get_bound(charp, world);
	if (zbound.max <= charp->position->z + charp->height
		&& charp->velocity.z < 0.0f)
	{
		charp->velocity.z = 0.0f;
		charp->ceiling_trigger = true;
	}
	capsule_phys_go_towards_floor(charp, zbound, world->clock.delta);
	if (charp->is_grounded && charp->velocity.z != 0)
	{
		charp->landing_trigger = true;
		charp->impact_velocity.z = charp->velocity.z;
		charp->velocity.z = 0.0f;
	}
	charp->is_grounded = (charp->position->z <= zbound.min);
	capsule_damp(charp, world);
}
