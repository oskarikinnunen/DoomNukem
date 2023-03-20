/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_gun_pickup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/16 14:16:09 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_gun_pickup_update(t_entity *entity, t_world *world)
{
	t_gun_pickup	*gun_pickup;
	
	gun_pickup = entity->component.data;
	if (gun_pickup == NULL || world->gamemode == MODE_EDITOR)
		return ;
	entity->transform.rotation.x += world->clock.delta * 0.001f;
	entity->transform.position.z = gun_pickup->original_z + (sinf(world->clock.time * 0.001f) * 10.0f);
	if (vector3_sqr_dist(world->player->transform.position, entity->transform.position) < 1000.0f)
	{
		int	i;

		i = 0;
		while (i < GUNPRESETCOUNT)
		{
			if (ft_strcmp(world->player->guns[i].stats.object_name, entity->obj->name) == 0)
			{
				world->player->guns[i].player_owned = true;
			}
			i++;
		}
		ft_strcpy(world->player->gui_notif.str.str, "You picked up a new gun");
		world->player->gui_notif.starttime = world->clock.time;
		destroy_entity(world, entity);
	}
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_gun_pickup_ui_update(t_entity *entity, t_world *world)
{
	t_gun_pickup	*gun_pickup;

	gun_pickup = entity->component.data;
	if (gun_pickup == NULL)
		return ;
}

/* Used to edit component values */
void	comp_gun_pickup_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_gun_pickup	*gun_pickup;
	//entity->particleimages[particleanim.];
	gun_pickup = entity->component.data;
	gui_label("GUI for gun_pickup not implemented", gui);
	if (gun_pickup == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_gun_pickup_loadassets(t_entity *entity, t_world *world)
{
	t_gun_pickup	*gun_pickup;
	gun_pickup = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_gun_pickup_allocate(t_entity *entity, t_world *world)
{
	t_gun_pickup	*gun_pickup;

	entity->component.data = ft_memalloc(sizeof(t_gun_pickup));
	entity->component.data_size = sizeof(t_gun_pickup);
	gun_pickup = (t_gun_pickup *)entity->component.data;
	gun_pickup->original_z = entity->transform.position.z;
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
	component->func_loadassets = comp_gun_pickup_loadassets;
}