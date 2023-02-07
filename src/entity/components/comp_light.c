/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_light.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/07 11:41:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_light_update(t_entity *entity, t_world *world)
{
	t_light	*light;

	light = entity->component.data;
	if (light == NULL)
		return ;
}


void	highlight_bound(t_entity *ent, t_world *world)
{
	/*if (ft_strequ(ent->obj->name, "meshtri"))
	{
		printf("rendering floor bound with radius %f \n", ent->obj->bounds.radius);
		render_ball(world->sdl, ent->obj->bounds.origin, ent->obj->bounds.radius, CLR_RED);
	}*/
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_light_ui_update(t_entity *entity, t_world *world)
{
	t_light			*light;
	static t_img	*icon;

	light = entity->component.data;
	if (light == NULL)
		return ;
	if (icon == NULL)
		icon = get_image_by_name(*world->sdl, "light_icon.cng");
	draw_entity_icon(entity, icon, world);
	for_all_active_entities(world, highlight_bound);
}

/*
	calculate_pointlight(t_entity *light_ent, t_world *world)
	{
		foreach active entity
		if (inrange)
		{
			for (triangles)
				calculate tri poly
				rasterize uv tri(t_entity *, tripoly *, world *)
		}

	}

*/

/* Used to edit component values */
void	comp_light_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_light	*light;

	light = entity->component.data;
	if (light == NULL)
		return;
	gui_labeled_float_slider("Radius: ", &light->radius, 0.1f, gui);
	gui_labeled_vector3_slider("Offset:", &light->origin, 0.1f, gui);
	render_ball(world->sdl, entity->transform.position, light->radius, AMBER_3);
}
/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_light_loadassets(t_entity *entity, t_world *world)
{
	t_light	*light;
	light = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_light_allocate(t_entity *entity, t_world *world)
{
	t_light	*light;

	entity->component.data = ft_memalloc(sizeof(t_light));
	entity->component.data_size = sizeof(t_light);
	light = (t_light *)entity->component.data;
	light->radius = 50.0f;
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_light(t_component *component)
{
	component->data_size = sizeof(t_light);
	component->func_allocate = comp_light_allocate;
	component->func_update = comp_light_update;
	//component->func_gui_edit = comp_light_gui_edit;
	component->func_gui_edit = comp_light_gui_edit;
	component->func_ui_update = comp_light_ui_update;
	component->func_loadassets = comp_light_loadassets;
}