/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/28 17:45:38 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"
#include "objects.h"
#include "libft.h"

 // Might need the whole gamecontext but I'm trying to avoid it, (trying to avoid global state)
 // TODO: normalize movement vector
/*
static t_vector3	movementvector(int32_t keystate, float angle)
{
	t_vector3	movement;

	movement = vector3_zero();
	if ((keystate >> KEYS_UPMASK) & 1) 
	{
		movement.x += sin(angle);
		movement.y += cos(angle);
	}
	if ((keystate >> KEYS_DOWNMASK) & 1)
	{
		movement.x -= sin(angle);
		movement.y -= cos(angle);
	}
	// strafe
	if ((keystate >> KEYS_LEFTMASK) & 1)
	{
		movement.x += sin(angle + RAD90);
		movement.y += cos(angle + RAD90);
	}
	if ((keystate >> KEYS_RIGHTMASK) & 1)
	{
		movement.x += -sin(angle + RAD90);
		movement.y += -cos(angle + RAD90);
	}
	//flying
	if ((keystate >> KEYS_SPACEMASK) & 1)
	{
		movement.z += 0.25f;
	}
	if ((keystate >> KEYS_CTRLMASK) & 1)
	{
		movement.z -= 0.25f;
	}
	movement = vector3_clamp_magnitude(movement, MAXMOVEMENTSPEED);
	return (movement);
}
*/

/* Previous working version before implementing updateinput */
/*
static t_vector3	player_movementvector(int32_t keystate, t_vector3 lookdir)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(player->lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	if ((keystate >> KEYS_UPMASK) & 1) 
		movement = vector3_add(movement, forward);
	if ((keystate >> KEYS_DOWNMASK) & 1)
		movement = vector3_sub(movement, forward);
	if ((keystate >> KEYS_LEFTMASK) & 1)
		movement = vector3_sub(movement,
			vector3_crossproduct(forward, vector3_up()));
	if ((keystate >> KEYS_RIGHTMASK) & 1)
		movement = vector3_add(movement,
			vector3_crossproduct(forward, vector3_up()));
	speed = 1.0f + (float)((keystate >> KEYS_SHIFTMASK) & 1);
	if ((keystate >> KEYS_CTRLMASK) & 1)
	{
		speed *= 0.5f;
		player->height = 30.0f;
	}
	else
		player->height = 60.0f;
		
	if (player->jump.active)
		movement.z += cos(player->jump.lerp * 4.5f) * 1.8f;
	else if (player->position.z > player->height && movement.z >= 0.0f)
		movement.z -= 4.0f;
	else if ((keystate >> KEYS_SPACEMASK) & 1 && !player->jump.active)
		start_anim(&player->jump, anim_forwards);
	movement = vector3_mul_vector3(movement, (t_vector3){speed, speed, 1.0f});
	movement = vector3_clamp_magnitude(movement, speed);
	return (movement);

*/

static t_vector3	player_movementvector(t_input input, t_player player)
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
	speed = 1.0f + input.run;
	movement.z += 1.5f * input.jump;
	movement.z -= 1.5f * input.crouch;
	movement = vector3_mul(movement, speed);
	movement = vector3_clamp_magnitude(movement, speed);
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

void	updateguntransform(t_game *game, t_player *player)
{
	static float	lerp;
	t_gun			*gun;

	gun = player->gun;
	if (game->input.aim)
		lerp += 0.02f * game->clock.delta;
	else
		lerp -= 0.02f * game->clock.delta;
	lerp = ft_clampf(lerp, 0.0f, 1.0f);
	gun->entity.transform.location = vector3_lerp(gun->holsterpos, gun->aimpos, lerp);
	if (game->input.shoot && gun->readytoshoot)
	{
		gun->shoot_anim.framerate = 120;
		gun->shoot_anim.loop = false;
		gun->shoot_anim.lastframe = 2;
		start_anim(&gun->shoot_anim, anim_forwards);
		gun->readytoshoot = false;
	}
	else if (!game->input.shoot && !gun->shoot_anim.active)
		gun->readytoshoot = true;
	update_anim(&gun->shoot_anim, game->clock.delta);
	gun->entity.transform.location = vector3_add(gun->entity.transform.location, vector3_mul(vector3_up(), gun->shoot_anim.lerp * -1.0f));
	gun->entity.transform.location = vector3_add(gun->entity.transform.location, vector3_mul((t_vector3){.x = 1.0f}, gun->shoot_anim.lerp * -0.25f));
	gun->entity.transform.location.z += vector2_magnitude(game->input.move) * cosf((game->clock.prev_time * 0.007f)) * 0.2f;
	gun->entity.transform.rotation.z += vector2_magnitude(game->input.move) * cosf((game->clock.prev_time * 0.007f)) * ft_degtorad(0.15f);
	gun->entity.transform.rotation.y = fmovetowards(gun->entity.transform.rotation.y, ft_degtorad(game->input.move.y * 1.15f), 0.0006f * game->clock.delta);
	gun->entity.transform.rotation.y += ft_flerp(0.0f, ft_degtorad(10.0f), gun->shoot_anim.lerp);
	gun->entity.transform.rotation.y = ft_clampf(gun->entity.transform.rotation.y, ft_degtorad(-0.5f), ft_degtorad(15.0f));
	gun->entity.transform.rotation.x = ft_flerp(0.0f, ft_degtorad(2.0f), gun->shoot_anim.lerp);
	float	zturn = game->input.move.x * 2.0f;
	zturn += game->input.turn.x * 5.0f;
	zturn = ft_clampf(zturn, ft_degtorad(-2.5f), ft_degtorad(2.5f));
	gun->entity.transform.rotation.z = fmovetowards(gun->entity.transform.rotation.z, zturn, 0.0004f * game->clock.delta);
	player->transform.rotation.y += gun->shoot_anim.lerp * game->clock.delta * 0.001f;
}

void	moveplayer(t_game *game)
{
	t_vector3	move_vector;
	t_vector3	potential_pos; //Unused right now, will be used when collision is reimplemented
	float	angle;

	updateguntransform(game, &game->player);
	move_vector = vector3_zero();
	t_vector2 delta_angle = vector2_mul(game->input.turn, game->clock.delta);
	game->player.transform.rotation = vector3_sub(game->player.transform.rotation, (t_vector3){delta_angle.x, delta_angle.y, 0.0f}); //TODO: this
	game->player.transform.rotation.y = ft_clampf(game->player.transform.rotation.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	game->player.lookdir = lookdirection((t_vector2){game->player.transform.rotation.x, game->player.transform.rotation.y});
	move_vector = player_movementvector(game->input, game->player);
	move_vector = vector3_mul(move_vector, game->clock.delta * MOVESPEED);
	game->player.speed = move_vector;
	game->player.transform.location = vector3_add(game->player.transform.location, move_vector);
	game->player.transform.location.z = ft_clampf(game->player.transform.location.z, game->player.height, 1000.0f);
}