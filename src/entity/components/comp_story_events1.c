/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_story_events1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:39:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_storyevent_update(t_entity *entity, t_world *world)
{
	t_storyevent	*storyevent;
	float			dist;

	storyevent = entity->component.data;
	if (storyevent == NULL)
		return ;
	dist = vector3_sqr_dist(entity->transform.position, \
			world->player->transform.position);
	if (dist < 5000.0f && !storyevent->triggered)
	{
		storyevent->triggered = true;
		storyevent->trigger_time = world->clock.time;
	}
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_story_event(t_component *component)
{
	component->data_size = sizeof(t_storyevent);
	component->func_allocate = comp_storyevent_allocate;
	component->func_update = comp_storyevent_update;
	component->func_gui_edit = comp_storyevent_gui_edit;
	component->func_ui_update = comp_storyevent_ui_update;
	component->func_load_assets = comp_storyevent_load_assets;
}
