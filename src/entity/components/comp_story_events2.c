/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_story_events2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:50:20 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 11:05:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void	storyevent_get_text(t_component comp, t_storyevent *storyevent)
{
	static char	strs[10][256] = {"Welcome to the debugging room. \
We have many different 'bouttons' for you to try out.", \
"Jump + crouch during the upwards motion of your jump to \
crouchjump over this part.", \
"Here you can see the infamous inclined floor and ceiling.", \
"Please don't tap the glass, the inclined floor and ceiling are \
very sensitive creatures", \
"temp4", \
"temp5"};

	ft_strncpy_term(storyevent->text, strs[comp.data_preset], 120);
}

// Called once per frame after the 3D world has been drawn.
// Use this to draw gizmos/rays/whatever
void	comp_storyevent_ui_update(t_entity *entity, t_world *world)
{
	t_storyevent	*storyevent;
	t_point			point;

	storyevent = entity->component.data;
	if (storyevent == NULL)
		return ;
	point.x = 10;
	point.y = world->sdl->screensize.y - 100;
	if (storyevent->triggered && world->clock.time < \
		storyevent->trigger_time + 7000)
	{
		storyevent_get_text(entity->component, storyevent);
		print_text_boxed(world->sdl, storyevent->text, point);
	}
}

/* Used to edit component values */
void	comp_storyevent_gui_edit(t_entity *entity,
								t_autogui *gui, t_world *world)
{
	t_storyevent	*storyevent;

	storyevent = entity->component.data;
	if (storyevent == NULL)
		return ;
	if (gui_labeled_int_slider("Event index:", \
					(int32_t *)&entity->component.data_preset, 0.1f, gui))
		storyevent->triggered = false;
	entity->component.data_preset = \
			ft_clamp(entity->component.data_preset, 0, 5);
}

/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_storyevent_load_assets(t_entity *entity, t_world *world)
{
	t_storyevent	*storyevent;

	storyevent = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_storyevent_allocate(t_entity *entity, t_world *world)
{
	t_storyevent	*storyevent;

	entity->component.data = prot_memalloc(sizeof(t_storyevent));
	entity->component.data_size = sizeof(t_storyevent);
	storyevent = (t_storyevent *)entity->component.data;
}
