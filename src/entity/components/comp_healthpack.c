/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_healthpack.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:28:24 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_healthpack_update(t_entity *entity, t_world *world)
{
	t_healthpack	*healthpack;

	healthpack = entity->component.data;
	if (healthpack == NULL || world->app_mode == APPMODE_EDIT)
		return ;
	if (world->player->health < MAXHEALTH
		&& vector3_sqr_dist(entity->transform.position,
			world->player->transform.position) < 2000.0f)
	{
		world->player->health += 20;
		world->player->health
			= ft_clampf(world->player->health, 40, MAXHEALTH);
		destroy_entity(world, entity);
	}
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_healthpack_allocate(t_entity *entity, t_world *world)
{
	t_healthpack	*healthpack;

	entity->component.data = prot_memalloc(sizeof(t_healthpack));
	entity->component.data_size = sizeof(t_healthpack);
	healthpack = (t_healthpack *)entity->component.data;
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_healthpack(t_component *component)
{
	component->data_size = sizeof(t_healthpack);
	component->func_allocate = comp_healthpack_allocate;
	component->func_update = comp_healthpack_update;
	component->func_gui_edit = NULL;
	component->func_ui_update = NULL;
	component->func_load_assets = NULL;
}
