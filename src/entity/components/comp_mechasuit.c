/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_mechasuit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/23 07:01:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_mechasuit_update(t_entity *entity, t_world *world)
{
	t_mechasuit	*mechasuit;
	float		qdist;

	mechasuit = entity->component.data;
	if (mechasuit == NULL)
		return ;
	if (world->player->input.use)
		printf("pressed e??\n");
	if (mechasuit->in_use && world->player->input.use)
	{
		printf("dismount??");
		world->player->locked = false;
		mechasuit->in_use = false;
		world->player->input.use = false;
	}
	else
	{
		mechasuit->p_dist = vector3_sqr_dist(world->player->transform.position, entity->transform.position);
		if (mechasuit->p_dist < 300.0f && world->player->input.use && !mechasuit->in_use)
		{
			world->player->locked = true;
			mechasuit->in_use = true;
			world->player->input.use = false;
		}
	}
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_mechasuit_ui_update(t_entity *entity, t_world *world)
{
	t_mechasuit	*mechasuit;

	mechasuit = entity->component.data;
	if (mechasuit == NULL)
		return ;
	if (mechasuit->in_use)
	{
		print_text_boxed(world->sdl, "[E] Dismount", point_div(world->sdl->screensize, 2));
	}
	else if (mechasuit->p_dist < 300.0f)
	{
		print_text_boxed(world->sdl, "[E] Mount", point_div(world->sdl->screensize, 2));
	}
		
}

/* Used to edit component values */

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_mechasuit_loadassets(t_entity *entity, t_world *world)
{
	t_mechasuit	*mechasuit;
	mechasuit = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_mechasuit_allocate(t_entity *entity, t_world *world)
{
	t_mechasuit	*mechasuit;

	entity->component.data = ft_memalloc(sizeof(t_mechasuit));
	entity->component.data_size = sizeof(t_mechasuit);
	mechasuit = (t_mechasuit *)entity->component.data;
	
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_mechasuit(t_component *component)
{
	component->data_size = sizeof(t_mechasuit);
	component->func_allocate = comp_mechasuit_allocate;
	component->func_update = comp_mechasuit_update;
	component->func_gui_edit = NULL;
	component->func_ui_update = comp_mechasuit_ui_update;
	component->func_loadassets = comp_mechasuit_loadassets;
}