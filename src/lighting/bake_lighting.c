/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bake_lighting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:00:29 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 22:27:15 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
/*
calculate lighting camera buffers for all lights

allocate space for maps for all entities

calculate lighting maps for all entities

combine lightmap value with img to get texture for all entities
*/

void	recalculate_lighting(t_world *world)
{
	for_all_active_entities(world, calculate_triangles_for_entity);
	allocate_map_for_entities(world);
	calculate_lighting(world);
	calculate_texture_for_entities(world);
	world->sdl->lighting_toggled = true;
}
