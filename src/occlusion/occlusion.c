/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:40:03 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/23 16:45:19 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void update_occlusion(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	update_peripheral_culling(sdl, render, entity);//if false
	update_occlusion_culling(sdl, render, entity);
}

bool is_entity_culled(t_sdlcontext sdl, t_render *render, t_entity *entity)//is_entity_occluded
{
	if (is_entity_frustrum_culled(sdl, render, entity) == false)
	{
		if (is_entity_peripheral_culled(sdl, render, entity) == false)
		{
			return (is_entity_occlusion_culled(sdl, render, entity));
		}
	}
	return (true);
}
