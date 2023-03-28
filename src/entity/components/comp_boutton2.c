/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_boutton2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 18:04:22 by raho              #+#    #+#             */
/*   Updated: 2023/03/28 10:30:02 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

// Called once per frame after the 3D world has been drawn
// Use this to draw gizmos/rays/whatever*/
void	comp_boutton_ui_update(t_entity *entity, t_world *world)
{
	t_boutton	*boutton;

	boutton = entity->component.data;
	if (boutton == NULL)
		return ;
	if (boutton->can_interact && world->player->health > 0)
	{
		print_text_boxed(world->sdl, "[E] Game over", \
				point_div(world->sdl->screensize, 2));
	}
}

/* Used to edit component values */
void	comp_boutton_gui_edit(t_entity *entity,
								t_autogui *gui, t_world *world)
{
	t_boutton	*boutton;

	boutton = entity->component.data;
	gui_label("GUI for boutton not implemented", gui);
	if (boutton == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_boutton_load_assets(t_entity *entity, t_world *world)
{
	t_boutton	*boutton;

	boutton = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_boutton_allocate(t_entity *entity, t_world *world)
{
	t_boutton	*boutton;

	entity->component.data = prot_memalloc(sizeof(t_boutton));
	entity->component.data_size = sizeof(t_boutton);
	boutton = (t_boutton *)entity->component.data;
}
