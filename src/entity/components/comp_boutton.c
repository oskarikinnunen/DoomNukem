/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_boutton.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 18:25:42 by okinnune         ###   ########.fr       */
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
	boutton->dist = vector3_sqr_dist(entity->transform.position, world->player->transform.position);
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_boutton_ui_update(t_entity *entity, t_world *world)
{
	t_boutton	*boutton;

	boutton = entity->component.data;
	if (boutton == NULL)
		return ;
	if (boutton->dist < 5000.0f)
	{
		print_text_boxed(world->sdl, "[E] Press boutton", point_div(world->sdl->screensize, 2));
	}
}

/* Used to edit component values */
void	comp_boutton_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_boutton	*boutton;
	//entity->particleimages[particleanim.];
	boutton = entity->component.data;
	gui_label("GUI for boutton not implemented", gui);
	if (boutton == NULL)
		return ;
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_boutton_loadassets(t_entity *entity, t_world *world)
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

	entity->component.data = ft_memalloc(sizeof(t_boutton));
	entity->component.data_size = sizeof(t_boutton);
	boutton = (t_boutton *)entity->component.data;
	
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