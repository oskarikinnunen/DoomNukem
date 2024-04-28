/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_interactable_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 22:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:47:21 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "render.h"

void	comp_interactable_ui_update(t_entity *entity, t_world *world)
{
	t_interactable	*inter;

	inter = entity->component.data;
	if (inter == NULL)
		return ;
	if (vector3_sqr_dist(world->player->transform.position,
			entity->transform.position) < 1000.0f)
	{
		print_text_boxed(world->sdl, "[E]",
			point_div(world->sdl->screensize, 2));
	}
}

void	comp_interactable_allocate(t_entity *entity, t_world *world)
{
	t_interactable	*inter;

	entity->component.data = prot_memalloc(sizeof(t_interactable));
	entity->component.data_size = sizeof(t_interactable);
	inter = (t_interactable *)entity->component.data;
	inter->radius = 7.0f;
	inter->velocity = vector3_zero();
	inter->wiggle.frame_rate = 30.0f;
	inter->wiggle.last_frame = 60.0f;
}

void	assign_component_interactable(t_component *component)
{
	component->data_size = sizeof(t_audiosource);
	component->func_allocate = comp_interactable_allocate;
	component->func_update = comp_interactable_update;
	component->func_ui_update = comp_interactable_ui_update;
	component->func_gui_edit = NULL;
	component->func_load_assets = NULL;
}
