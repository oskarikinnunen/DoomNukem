/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_interactable_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 22:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 12:09:00 by okinnune         ###   ########.fr       */
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
}

void	comp_interactable_allocate(t_entity *entity, t_world *world)
{
	t_interactable	*inter;

	entity->component.data = prot_memalloc(sizeof(t_interactable));
	entity->component.data_size = sizeof(t_interactable);
	entity->dynamic_lit = true;
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
