/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 14:32:17 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"

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

static bool checkwallcollisions(t_game *game, float potential_pos[2])
{
	t_list	*node;
	t_line	line;

	node = game->linelst;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		//v2mul(line.start, TILESIZE);
		//v2mul(line.end, TILESIZE);
		if (linecirclecollision(line, potential_pos, PLAYERRADIUS * TILESIZE) == true)
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
	f2cpy(potential_plr_pos, move_f2);
	f2add(potential_plr_pos, game->player.position);
	if (checkwallcollisions(game, potential_plr_pos) == false)
		f2add(game->player.position, move_f2);
}