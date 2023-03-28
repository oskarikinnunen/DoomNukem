/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_gun_pickup1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 23:03:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void	gun_pickup_change_player_gun(t_entity *entity,
		int i, t_world *world)
{
	int	index;

	if (ft_strcmp(world->player->guns[i].stats.object_name, \
		entity->obj->name) == 0)
	{
		index = 0;
		while (index < GUNPRESETCOUNT)
		{
			world->player->guns[index].entity->hidden = true;
			index++;
		}
		world->player->guns[i].player_owned = true;
		world->player->guns[i].entity->hidden = false;
		world->player->gun = &world->player->guns[i];
	}
}

/* Called once per frame, use this to update your entitys state */
void	comp_gun_pickup_update(t_entity *entity, t_world *world)
{
	t_gun_pickup	*gun_pickup;
	int				i;

	gun_pickup = entity->component.data;
	if (gun_pickup == NULL || world->app_mode == APPMODE_EDIT)
		return ;
	entity->transform.rotation.x += world->clock.delta * 0.001f;
	entity->transform.position.z = \
			gun_pickup->original_z + (ft_absf(sinf(world->clock.time
					* 0.001f)) * 10.0f);
	if (vector3_sqr_dist(world->player->transform.position, \
						entity->transform.position) < 1000.0f)
	{
		i = 0;
		while (i < GUNPRESETCOUNT)
		{
			gun_pickup_change_player_gun(entity, i, world);
			i++;
		}
		ft_strncpy_term(world->player->gui_notif.str.str, \
						"You picked up a new gun", 60);
		world->player->gui_notif.starttime = world->clock.time;
		destroy_entity(world, entity);
	}
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_gun_pickup(t_component *component)
{
	component->data_size = sizeof(t_gun_pickup);
	component->func_allocate = comp_gun_pickup_allocate;
	component->func_update = comp_gun_pickup_update;
	component->func_gui_edit = comp_gun_pickup_gui_edit;
	component->func_ui_update = comp_gun_pickup_ui_update;
	component->func_load_assets = comp_gun_pickup_load_assets;
}
