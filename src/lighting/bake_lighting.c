/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bake_lighting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:00:29 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/21 16:17:41 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

float		clockwise(t_vector2 p[3])
{
	return ((p[1].x - p[0].x) * (p[2].y - p[0].y) - (p[2].x - p[0].x) * (p[1].y - p[0].y));
}

/*
calculate lighting camera buffers for all lights

allocate space for maps for all entities

calculate lighting maps for all entities

combine lightmap value with img to get texture for all entities
*/

void	recalculate_lighting(t_world *world)
{
	recalculate_pointlight(world);
	allocate_map_for_entities(world);
	calculate_lighting(world);
	calculate_texture_for_entities(world);
}
