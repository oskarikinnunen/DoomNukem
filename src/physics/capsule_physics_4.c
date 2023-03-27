/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capsule_physics_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 19:16:58 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 19:22:47 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"
#include "collision.h"
#include "editor_tools.h"

bool	charphys_floor_share_z(t_character_physics *cp, t_meshtri *floor)
{
	return (floor->v->z >= cp->position->z
		&& floor->v->z <= cp->position->z + cp->height);
}
