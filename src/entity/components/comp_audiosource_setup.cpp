/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_audiosource_setup.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:44:02 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:29:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

void	comp_audiosource_ui_update(t_entity *entity, t_world *world)
{
	t_audiosource	*source;
	static t_img	*audio_off_img;
	static t_img	*audio_on_img;
	t_img			*cur_img;
	float			qdist;

	if (audio_off_img == NULL || audio_on_img == NULL)
	{
		audio_off_img = get_image_by_name(*world->sdl, "audio_off_icon");
		audio_on_img = get_image_by_name(*world->sdl, "audio_on_icon");
	}
	source = entity->component.data;
	if (source == NULL || source->sample.sound == NULL)
	{
		return ;
	}
	if (source_is_playing(source))
		cur_img = audio_on_img;
	else
		cur_img = audio_off_img;
	draw_entity_icon(entity, cur_img, world);
}

//TODO: add datapresets here
void	comp_audiosource_gui_edit(t_entity *entity,
		t_autogui *gui, t_world *world)
{
	t_audiosource	*source;

	source = entity->component.data;
	if (source == NULL)
	{
		gui_label("!NULL source", gui);
		return ;
	}
	if (gui_button("Play audio", gui))
		source->queue_play = true;
}

void	comp_audiosource_load_assets(t_entity *entity, t_world *world)
{
	t_audiosource	*source;

	source = entity->component.data;
	source->sample = get_sample(world->sdl, source->sample.name);
}

void	comp_audiosource_allocate(t_entity *entity, t_world *world)
{
	t_audiosource	*source;

	entity->component.data = prot_memalloc(sizeof(t_audiosource));
	entity->component.data_size = sizeof(t_audiosource);
	source = (t_audiosource *)entity->component.data;
	source->sample = get_sample(world->sdl, "amb_dogbark1.wav");
	source->volume = 1.0f;
	source->range = 80.0f;
	source->channel = NULL;
}

void	assign_component_audiosource(t_component *component)
{
	component->data_size = sizeof(t_audiosource);
	component->func_allocate = comp_audiosource_allocate;
	component->func_update = comp_audiosource_update;
	component->func_gui_edit = comp_audiosource_gui_edit;
	component->func_ui_update = comp_audiosource_ui_update;
	component->func_load_assets = comp_audiosource_load_assets;
}
