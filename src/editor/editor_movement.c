/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 18:03:37 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/05 16:33:24 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "doomnukem.h"
#include "inputhelp.h"

static t_vector3	editor_movementvector(int32_t keystate, t_vector3 lookdir)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
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
	movement.z += 1.5f * ((keystate >> KEYS_SPACEMASK) & 1);
	movement.z -= 1.5f * ((keystate >> KEYS_CTRLMASK) & 1);
	movement = vector3_mul(movement, speed);
	movement = vector3_clamp_magnitude(movement, speed);
	return (movement);
}

void	move_editor(t_editor *ed)
{
	t_vector3	move_vector;
	//t_vector3	forward
	t_vector2	new_angle;

	new_angle.x = -ed->mouse.delta.x;
	new_angle.y = -ed->mouse.delta.y;
	new_angle = vector2_mul(new_angle, MOUSESPEED * (float)ed->clock.delta);
	ed->angle = vector2_add(ed->angle, new_angle);
	ed->angle.y = ft_clampf(ed->angle.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	ed->forward = lookdirection(ed->angle);
	move_vector = editor_movementvector(ed->keystate, ed->forward);
	move_vector = vector3_mul(move_vector, ed->clock.delta * MOVESPEED);
	ed->position = vector3_add(ed->position, move_vector);
}

/*void	moveplayer(t_game *game)
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
}*/