/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_interactable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 06:20:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 11:52:31 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "render.h"

static void	update_physics(t_entity *entity, t_interactable *inter)
{
	float				lerp;
	static float		override;

	inter->phys.position = &entity->transform.position;
	inter->phys.height = 16.0f;
	inter->phys.max_velocity = 12.0f;
	inter->phys.radius = 8.0f;
	inter->phys.gravity_override = NULL;
}

static void	comp_interactable_push(t_entity *entity,
		t_interactable *inter, t_world *world)
{
	float		invf;
	t_vector2	vel2;

	inter->velocity = vector3_sub(entity->transform.position,
			world->player->transform.position);
	inter->velocity = vector3_normalise(inter->velocity);
	invf = 250.0f - vector3_sqr_dist(world->player->transform.position,
			entity->transform.position);
	inter->velocity = vector3_mul(inter->velocity, invf
			* 0.0001f * world->clock.delta);
	vel2 = v3tov2(inter->velocity);
	capsule_add_xy_velocity(vel2, &inter->phys, world);
	if (!inter->wiggle.active && invf > 100.0f)
	{
		inter->pushv = vector2_normalise(v3tov2(inter->velocity));
		start_anim(&inter->wiggle, anim_forwards);
		entity->transform.rotation.y = -inter->pushv.x * 0.3f;
		entity->transform.rotation.z = -inter->pushv.y * 0.3f;
	}
}

static void	comp_interactable_wiggle(t_entity *entity,
		t_interactable *inter, t_world *world)
{
	float	angleamount;

	if (inter->wiggle.active)
	{
		update_anim(&inter->wiggle, world->clock.delta);
		angleamount = (1.0f - inter->wiggle.lerp) * 0.2f;
		entity->transform.rotation.y = ft_fmovetowards(
				entity->transform.rotation.y, angleamount
				* cosf(world->clock.time * 0.005f),
				world->clock.delta * 0.0007f * ft_absf(inter->pushv.x));
		entity->transform.rotation.z = ft_fmovetowards(
				entity->transform.rotation.z,
				angleamount * sinf(world->clock.time * 0.005f),
				world->clock.delta * 0.0007f * ft_absf(inter->pushv.y));
	}
	else
	{
		entity->transform.rotation.z = ft_fmovetowards(
				entity->transform.rotation.z,
				0.0f, world->clock.delta * 0.0008f);
		entity->transform.rotation.y = ft_fmovetowards(
				entity->transform.rotation.y, 0.0f,
				world->clock.delta * 0.0008f);
	}
}

void	comp_interactable_update(t_entity *entity, t_world *world)
{
	t_interactable	*inter;

	inter = entity->component.data;
	if (inter == NULL || world->app_mode == APPMODE_EDIT)
		return ;
	if (vector3_sqr_dist(world->player->transform.position,
			entity->transform.position) < 250.0f)
		comp_interactable_push(entity, inter, world);
	comp_interactable_wiggle(entity, inter, world);
	update_physics(entity, inter);
	capsule_phys_step(&inter->phys, world);
}
