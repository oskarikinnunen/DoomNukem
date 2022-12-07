/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npcs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 13:49:27 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/07 08:52:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NPCS_H
# define NPCS_H

#include "entity.h"
#include "colliders.h"

typedef struct s_npc
{
	t_entity			*entity;
	uint16_t			saved_entityid;
	t_vector3			destination;
	t_humancolliders	colliders;
	/*t_capsulecollider	handcolliders[2];
	t_capsulecollider	bodycollider;*/
	bool		active;
}	t_npc;

# endif