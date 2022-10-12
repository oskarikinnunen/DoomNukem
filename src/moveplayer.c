/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 18:31:25 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"

 // Might need the whole gamecontext but I'm trying to avoid it, (trying to avoid global state)
 // TODO: normalize movement vector
static void	updatemovementvector(float move_f2[2], int32_t keystate, float angle)
{
	//TODO: next parts could be done with some kind of "rotatevector" function
	if ((keystate >> KEYS_UPMASK) & 1) 
	{
		move_f2[X] += sin(angle);
		move_f2[Y] += cos(angle);
	}
	if ((keystate >> KEYS_DOWNMASK) & 1)
	{
		move_f2[X] -= sin(angle);
		move_f2[Y] -= cos(angle);
	}
	// strafe
	if ((keystate >> KEYS_LEFTMASK) & 1)
	{
		move_f2[X] += sin(angle + RAD90);
		move_f2[Y] += cos(angle + RAD90);
	}
	if ((keystate >> KEYS_RGHTMASK) & 1)
	{
		move_f2[X] += -sin(angle + RAD90);
		move_f2[Y] += -cos(angle + RAD90);
	}
}

//Only checks start and end point collisions, TODO: fix linecirclecollision
static bool checkwallcollisions(t_game *game, float potential_pos[2])
{
	t_list		*node;
	t_line		line;
	float		test_line_pos[2];
	t_bresenham	b;
	

	node = game->linelst;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		v2mul_to_f2(line.start, TILESIZE, test_line_pos);
		if (pointcirclecollision(test_line_pos, potential_pos, PLAYERRADIUS))
			return (true);
		v2mul_to_f2(line.end, TILESIZE, test_line_pos);
		if (pointcirclecollision(test_line_pos, potential_pos, PLAYERRADIUS))
			return (true);
		node = node->next;
	}
	return (false);
}

void	moveplayer(t_game *game)
{
	float	move_f2[2];
	float	potential_plr_pos[2];
	float	angle;

	ft_bzero(move_f2, sizeof(float [2]));
	angle = 0;
	angle -= game->mouse.p_delta[X] * MOUSESPEED;
	//	rotate character with arrow keys, doesn't work correctly with strafe
	//	but if you need it it's here, just uncomment the next 2 lines
	//angle -= (keystate >> KEYS_RGHTMASK) & 1;
	//angle += (keystate >> KEYS_LEFTMASK) & 1;
	angle *= game->clock.delta * ROTATESPEED;
	game->player.angle += angle;
	updatemovementvector(move_f2, game->keystate, game->player.angle);
	f2mul(move_f2, game->clock.delta * MOVESPEED);
	#ifndef COLLISION_ON
		f2add(game->player.position, move_f2);
	#else
		f2cpy(potential_plr_pos, move_f2);
		f2add(potential_plr_pos, game->player.position);
		if (checkwallcollisions(game, potential_plr_pos) == false)
			f2add(game->player.position, move_f2);
	#endif
}