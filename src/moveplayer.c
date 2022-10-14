/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/14 20:21:38 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"
#include "bresenham.h"

 // Might need the whole gamecontext but I'm trying to avoid it, (trying to avoid global state)
 // TODO: normalize movement vector
static t_vector2	movementvector(int32_t keystate, float angle)
{
	t_vector2	movement;

	movement = vector2_zero();
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
	if ((keystate >> KEYS_RGHTMASK) & 1)
	{
		movement.x += -sin(angle + RAD90);
		movement.y += -cos(angle + RAD90);
	}
	return (movement);
}

//Only checks start and end point collisions, TODO: fix linecirclecollision
static bool checkwallcollisions(t_game *game, t_vector2 potential_pos)
{
	t_list		*node;
	t_line		line;
	t_vector2	test_line_pos;
	t_bresenham	b;
	

	node = game->linelst;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		//test_line_pos = 
		line.start = point_mul(line.start, TILESIZE);
		line.end = point_mul(line.end, TILESIZE);
		test_line_pos = point_to_vector2(line.start);
		if (pointcirclecollision(test_line_pos, potential_pos, PLAYERRADIUS))
			return (true);
		test_line_pos = point_to_vector2(line.start);
		if (pointcirclecollision(test_line_pos, potential_pos, PLAYERRADIUS))
			return (true);
		node = node->next;
	}
	return (false);
}

void	moveplayer(t_game *game)
{
	t_vector2	move_vector; //
	t_vector2	potential_pos;
	float	angle;

	//ft_bzero(move_f2, sizeof(float [2]));
	move_vector = vector2_zero();
	angle = 0;
	angle -= game->mouse.delta.x * MOUSESPEED;
	//	rotate character with arrow keys, doesn't work correctly with strafe
	//	but if you need it it's here, just uncomment the next 2 lines
	//angle -= ((game->keystate >> KEYS_RGHTMASK) & 1) * ROTATESPEED;
	//angle -= ((game->keystate >> KEYS_LEFTMASK) & 1) * ROTATESPEED;
	//angle += (keystate >> KEYS_LEFTMASK) & 1;
	angle *= game->clock.delta;
	game->player.angle += angle;
	move_vector = movementvector(game->keystate, game->player.angle);
	move_vector = vector2_mul(move_vector, game->clock.delta * MOVESPEED);
	
	game->player.position = vector2_add(game->player.position, move_vector);
}