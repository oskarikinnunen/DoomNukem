/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_healthpack.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 12:08:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void	comp_ammo_udate(t_world *world)
{
	if (world->player->ammo_arr[AM_ASSAULT] < 205)
		world->player->ammo_arr[AM_ASSAULT] += 50;
	else if (world->player->ammo_arr[AM_ASSAULT] >= 205 && \
			world->player->ammo_arr[AM_ASSAULT] < 250)
		world->player->ammo_arr[AM_ASSAULT] = 255;
	if (world->player->ammo_arr[AM_SNIPER] < 205)
		world->player->ammo_arr[AM_SNIPER] += 50;
	else if (world->player->ammo_arr[AM_SNIPER] >= 205 && \
			world->player->ammo_arr[AM_SNIPER] < 250)
		world->player->ammo_arr[AM_SNIPER] = 255;
}

/* Called once per frame, use this to update your entitys state */
void	comp_healthpack_update(t_entity *entity, t_world *world)
{
	t_healthpack	*healthpack;
	bool			player_close_enough;

	healthpack = entity->component.data;
	if (healthpack == NULL || world->app_mode == APPMODE_EDIT)
		return ;
	player_close_enough = vector3_sqr_dist(entity->transform.position,
			world->player->transform.position) < 2000.0f;
	if (!player_close_enough)
		return ;
	if (healthpack->is_actually_ammo)
	{
		comp_ammo_udate(world);
		destroy_entity(world, entity);
	}
	else if (world->player->health < MAXHEALTH)
	{
		world->player->health += 40;
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
	if (ft_strcmp(entity->obj->name, "ammo_pickup.obj") == 0)
		healthpack->is_actually_ammo = true;
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
