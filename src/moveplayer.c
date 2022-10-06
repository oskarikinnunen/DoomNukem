/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 11:14:10 by okinnune         ###   ########.fr       */
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

void	moveplayer(int32_t keystate, int mousedelta_x, t_player *plr, uint32_t	delta)
{
	float	move_f2[2];
	float	angle;

	ft_bzero(move_f2, sizeof(float [2]));
	angle = 0;
	angle -= mousedelta_x * MOUSESPEED;
	//	rotate character with arrow keys, doesn't work correctly with strafe
	//	but if you need it it's here, just uncomment the next 2 lines
	//angle -= (keystate >> KEYS_RGHTMASK) & 1;
	//angle += (keystate >> KEYS_LEFTMASK) & 1;
	angle *= delta * ROTATESPEED;
	plr->angle += angle;
	updatemovementvector(move_f2, keystate, plr->angle);
	f2mul(move_f2, delta * MOVESPEED);
	f2add(plr->position, move_f2);
}