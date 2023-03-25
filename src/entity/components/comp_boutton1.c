/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_boutton.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 18:05:11 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_boutton_update(t_entity *entity, t_world *world)
{
	t_boutton	*boutton;

	boutton = entity->component.data;
	if (boutton == NULL)
		return ;
	boutton->can_interact
		= (world->player->raycastinfo.hit_entity == entity
			&& world->player->raycastinfo.distance < 50.0f);
	if (boutton->can_interact && world->player->input.use
		&& world->clock.time > boutton->last_interact_time + 5000)
	{
		boutton->last_interact_time = world->clock.time;
		world->sdl->lighting_toggled = !world->sdl->lighting_toggled;
	}
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_boutton(t_component *component)
{
	component->data_size = sizeof(t_boutton);
	component->func_allocate = comp_boutton_allocate;
	component->func_update = comp_boutton_update;
	component->func_gui_edit = comp_boutton_gui_edit;
	component->func_ui_update = comp_boutton_ui_update;
	component->func_loadassets = comp_boutton_loadassets;
}
