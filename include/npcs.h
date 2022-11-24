/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npcs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 13:49:27 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/24 15:19:43 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NPCS_H
# define NPCS_H

#include "entity.h"
#include "colliders.h"

typedef struct s_npc
{
	t_entity			entity;
	t_vector3			destination;
	t_humancolliders	colliders;
	/*t_capsulecollider	handcolliders[2];
	t_capsulecollider	bodycollider;*/
	bool		active;
}	t_npc;

# endif