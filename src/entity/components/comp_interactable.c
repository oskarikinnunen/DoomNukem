/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_interactable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 06:20:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 16:39:36 by raho             ###   ########.fr       */
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
	//phys.position = &entity->transform.position;
	inter->phys.height = 16.0f;
	inter->phys.max_velocity = 12.0f;
	inter->phys.radius = 8.0f;
	inter->phys.gravity_override = NULL;
}

void	comp_interactable_ui_update(t_entity *entity, t_world *world)
{
	t_interactable	*inter;
	inter = entity->component.data;

	if (inter == NULL)
		return ;
	if (vector3_sqr_dist(world->player->transform.position, entity->transform.position) < 1000.0f)
	{
		print_text_boxed(world->sdl, "[E]", point_div(world->sdl->screensize, 2));
	}
}

void	comp_interactable_update(t_entity *entity, t_world *world)
{
	t_interactable	*inter;
	static t_vector2 pushv;
	
	inter = entity->component.data;
	if (inter == NULL)
		return ;
	if (vector3_sqr_dist(world->player->transform.position, entity->transform.position) < 250.0f)
	{
		//world->player->velocity = vector3_mul(world->player->velocity, (world->clock.delta * 0.05f) - 1.0f);
		//world->player->velocity = vector3_sub(vector3_zero(), world->player->velocity); //Only for z
		inter->velocity = vector3_sub(entity->transform.position, world->player->transform.position);
		inter->velocity = vector3_normalise(inter->velocity);
		float invf = 250.0f - vector3_sqr_dist(world->player->transform.position, entity->transform.position);
		inter->velocity = vector3_mul(inter->velocity, invf * 0.0001f * world->clock.delta);
		t_vector2 vel2 = v3tov2(inter->velocity);
		capsule_add_xy_velocity(vel2, &inter->phys, world);
		if (!inter->wiggle.active && invf > 100.0f) //TODO: check for delta velocity
		{
			pushv = vector2_normalise(v3tov2(inter->velocity));
			start_anim(&inter->wiggle, anim_forwards);
			entity->transform.rotation.y = -pushv.x * 0.3f;
			entity->transform.rotation.z = -pushv.y * 0.3f;
		}
	}
	t_vector2	velocity_xy = v3tov2(inter->velocity);
	if (inter->wiggle.active)
	{
		update_anim(&inter->wiggle, world->clock.delta);
		float angleamount = (1.0f - inter->wiggle.lerp) * 0.2f;
		entity->transform.rotation.y = ft_fmovetowards(entity->transform.rotation.y, angleamount * cosf(world->clock.time * 0.005f), world->clock.delta * 0.0007f * ft_absf(pushv.x));
		entity->transform.rotation.z = ft_fmovetowards(entity->transform.rotation.z, angleamount * sinf(world->clock.time * 0.005f), world->clock.delta * 0.0007f * ft_absf(pushv.y));
	}
	else
	{
		entity->transform.rotation.z = ft_fmovetowards(entity->transform.rotation.z, 0.0f, world->clock.delta * 0.0008f);
		entity->transform.rotation.y = ft_fmovetowards(entity->transform.rotation.y, 0.0f, world->clock.delta * 0.0008f);
	}
	update_physics(entity, inter);
	capsule_applygravity_new(&inter->phys, world);
	capsule_damp(&inter->phys, world);
	//inter->velocity.x = ft_fmovetowards(inter->velocity.x, 0.0f, world->clock.delta * 0.012f);
	//inter->velocity.x = ft_fmovetowards(inter->velocity.y, 0.0f, world->clock.delta * 0.012f);
	//capsule_applygravity(inter_physics(entity, inter), world);
	//capsule_applygravity_new(inter_physics())
}

void	comp_interactable_allocate(t_entity *entity, t_world *world)
{
	t_interactable	*inter;

	entity->component.data = prot_memalloc(sizeof(t_interactable));
	entity->component.data_size = sizeof(t_interactable);
	inter = (t_interactable *)entity->component.data;
	inter->radius = 7.0f;
	inter->velocity = vector3_zero();
	inter->wiggle.framerate = 30.0f;
	inter->wiggle.lastframe = 60.0f;
}

void	assign_component_interactable(t_component *component)
{
	component->data_size = sizeof(t_audiosource);
	component->func_allocate = comp_interactable_allocate;
	component->func_update = comp_interactable_update;
	component->func_ui_update = comp_interactable_ui_update;
	component->func_gui_edit = NULL;
	component->func_loadassets = NULL;
	//spawn_entity
	//addcomponent(pft_audiosource)
}