/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/24 11:34:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
#include "bresenham.h"
#include "objects.h"
#include "entity.h"
#include "libft.h"
#include "editor_tools.h"
#include "movement_defs.h"

static t_vector3	normalized_inputvector(t_input input, t_player player)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(player.lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	movement = vector3_mul(forward, -input.move.y);
	t_vector3 right = vector3_crossproduct(forward, vector3_up());
	t_vector3 strafe = vector3_mul(right, input.move.x);
	movement = vector3_add(movement, strafe);
	if (player.noclip)
	{
		movement.z += 1.5f * input.jump;
		movement.z -= 1.5f * input.crouch;
	}
	movement = vector3_mul(movement, 1.0f);
	movement = vector3_clamp_magnitude(movement, 1.0f);
	return (movement);
}

float	fmovetowards(float f1, float f2, float delta)
{
	float	result;

	result = f1;
	if (f1 < f2)
	{
		result += delta;
		result = ft_clampf(result, f1, f2);
	}
	else
	{
		result -= delta;
		result = ft_clampf(result, f2, f1);
	}
	return (result);
}

void	updateguntransform(t_input *input, t_clock *clock, t_player *player)
{
	static float	lerp;
	t_gun			*gun;
	t_vector3		neutralpos;

	if (player->gun == NULL || player->gun->disabled)
		return ;
	gun = player->gun;
	if (input->aim)
		lerp += 0.02f * clock->delta;
	else
		lerp -= 0.02f * clock->delta;
	lerp = ft_clampf(lerp, 0.0f, 1.0f);
	neutralpos = vector3_lerp(gun->holsterpos, gun->aimpos, lerp);
	gun->entity.transform.position = neutralpos;
	//neutralpos = gun->entity.transform.location;
	if (input->shoot && gun->readytoshoot)
	{
		start_anim(&gun->shoot_anim, anim_forwards);
		start_anim(&gun->view_anim, anim_forwards);
		//gun->entity.transform
		//TODO: add localposition and position
		//t_vector3 worldspace_position = transformed_vector3(gun->entity.transform, gun->entity.transform.position).v;
		gun->readytoshoot = false;
		gun->lastshottime = clock->prev_time;
	}
	else if (!input->shoot && !gun->shoot_anim.active && clock->prev_time > gun->lastshottime + gun->firedelay)
		gun->readytoshoot = true;
	if (gun->fullauto && clock->prev_time > gun->lastshottime + gun->firedelay)
		gun->readytoshoot = true;
	update_anim(&gun->shoot_anim, clock->delta);
	update_anim(&gun->view_anim, clock->delta);
	//recoil.y
	gun->entity.transform.position = vector3_add(gun->entity.transform.position, vector3_mul(vector3_up(), gun->shoot_anim.lerp * gun->recoiljump.y));
	//gun->entity.transform.location = vector3_movetowards
	//gun->entity.transform.location.z = fmovetowards(neutralpos.z)
	//gun->entity.transform.location = vector3_add(gun->entity.transform.location, vector3_mul((t_vector3){.x = 1.0f}, gun->shoot_anim.lerp * -0.25f));
	//bobbing:
	gun->entity.transform.position.z += vector2_magnitude(input->move) * cosf((clock->prev_time * 0.007f)) * 0.2f;
	gun->entity.transform.rotation.z += vector2_magnitude(input->move) * cosf((clock->prev_time * 0.007f)) * ft_degtorad(0.15f);
	//recoilrecovery
	gun->entity.transform.rotation.y = fmovetowards(gun->entity.transform.rotation.y, ft_degtorad(input->move.y * 1.15f), gun->anglerecovery * clock->delta);
	//recoil.y
	gun->entity.transform.rotation.y += ft_flerp(0.0f, ft_degtorad(gun->recoilangle.y), gun->shoot_anim.lerp);
	gun->entity.transform.rotation.y = ft_clampf(gun->entity.transform.rotation.y, ft_degtorad(-0.5f), ft_degtorad(15.0f));
	gun->entity.transform.rotation.x = ft_flerp(0.0f, ft_degtorad(2.0f), gun->shoot_anim.lerp);
	float	zturn = input->move.x * 2.0f;
	zturn += input->turn.x * 5.0f;
	zturn = ft_clampf(zturn, ft_degtorad(-2.5f), ft_degtorad(2.5f));
	if (player->locked)
		return ;
	gun->entity.transform.rotation.z = fmovetowards(gun->entity.transform.rotation.z, zturn, 0.0004f * clock->delta);
	player->transform.rotation.y += gun->view_anim.lerp * clock->delta * gun->viewrecoil.y; //Separate view jump animation that is longer than gun jump animation?
}

static void noclip_movement(t_player *player, t_vector3 move_vector, t_world *world)
{
	player->transform.position = vector3_add(player->transform.position, move_vector);
}

void	moveplayer(t_player *player, t_input *input, t_world *world)
{
	t_vector3	move_vector;
	float		angle;

	updateguntransform(input, &world->clock, player);
	player->input = *input; //TODO: move to update_input?;
	if (!player->locked)
	{
		if (!player->noclip)
			playermovement_normal(player, world);
		else
			playermovement_noclip(player, world);
	}
	player->headposition = vector3_add(player->transform.position, (t_vector3){.z = player->height * 0.75f});
}
