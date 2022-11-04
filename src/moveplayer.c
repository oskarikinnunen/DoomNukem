/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/03 19:53:09 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"

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
static t_vector3	movementvector(int32_t keystate, t_vector3 lookdir)
{
	t_vector3	movement;
	t_vector3	forward;

	movement = vector3_zero();
	forward = vector3_mul_vector3(lookdir, (t_vector3){1, 1, 0});
	if ((keystate >> KEYS_UPMASK) & 1) 
	{
		movement = vector3_add(movement, forward);
	}
	if ((keystate >> KEYS_DOWNMASK) & 1)
	{
		movement = vector3_sub(movement, forward);
	}
	// strafe
	if ((keystate >> KEYS_LEFTMASK) & 1)
	{
		movement = vector3_sub(movement,
			vector3_crossproduct(forward, (t_vector3){0.0f, 0.0f, 1.0f}));
		//movement.x += sin(angle + RAD90);
		//movement.y += cos(angle + RAD90);
	}
	if ((keystate >> KEYS_RIGHTMASK) & 1)
	{
		movement = vector3_add(movement,
			vector3_crossproduct(forward, (t_vector3){0.0f, 0.0f, 1.0f}));
		//movement.x += -sin(angle + RAD90);
		//movement.y += -cos(angle + RAD90);
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

void	moveplayer(t_game *game)
{
	t_vector3	move_vector;
	t_vector3	potential_pos; //Unused right now, will be used when collision is reimplemented
	float	angle;

	move_vector = vector3_zero();
	angle = 0;
	angle -= game->mouse.delta.x * MOUSESPEED;
	angle *= game->clock.delta;
	game->player.angle.x += angle;
	angle = 0;
	angle -= game->mouse.delta.y * MOUSESPEED;
	angle *= game->clock.delta;
	game->player.angle.y += angle;
	game->player.angle.y = ft_clampf(game->player.angle.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	game->player.lookdir = lookdirection(game->player.angle);
	move_vector = movementvector(game->keystate, game->player.lookdir);
	move_vector = vector3_mul(move_vector, game->clock.delta * MOVESPEED);
	game->player.position = vector3_add(game->player.position, move_vector);
}