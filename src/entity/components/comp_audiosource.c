/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_audiosource.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:40:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

void	comp_audiosource_update(t_entity *entity, t_world *world)
{
	t_audio_source	*source;
	bool			isplaying;

	source = entity->component.data;
	if (source == NULL || source->sample.sound == NULL)
	{
		return ;
	}
	isplaying = source_is_playing(source);
	if (isplaying)
	{
		if (entity->occlusion.is_occluded)
			source->_real_range = ft_fmovetowards(source->_real_range, source->range * 0.35f, world->clock.delta * 0.20f);
		else
			source->_real_range = ft_fmovetowards(source->_real_range, source->range, world->clock.delta * 0.20f);
		FMOD_Channel_Set3DAttributes(source->channel,
			(FMOD_VECTOR *)&entity->transform.position, &((FMOD_VECTOR){0}));
		FMOD_Channel_Set3DMinMaxDistance(source->channel, source->_real_range, 10000.0f);
		FMOD_Channel_SetVolume(source->channel, source->volume * world->sdl->audio.sfx_volume);
	}
	if (!isplaying && source->play_always)
	{
		if (source->random_delay_min != 0)
		{
			if (source->_next_start == 0)
			{
				uint32_t	r = game_random_range(world,
					source->random_delay_min * 1000,
					source->random_delay_max * 1000);
				source->_next_start = world->clock.time + r;
				printf("pseudorandom was %i \n", r);
			}
			else if (world->clock.time > source->_next_start)
			{
				source->_next_start = 0;
				source->queue_play = true;
			}
		}
		else
			source->queue_play = true;
	}
	if (source->queue_play == true && !isplaying)
	{
		_audiosource_start(world->sdl, source, &entity->transform.position);
		source->queue_play = false;
	}
	if (source->queue_stop)
	{
		FMOD_Channel_Stop(source->channel);
		source->queue_stop = false;
	}
}

void	comp_audiosource_ui_update(t_entity *entity, t_world *world)
{
	t_audio_source	*source;
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

void	comp_audiosource_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_audio_source	*source;
	static bool		toggle_select;
	source = entity->component.data;
	
	if (source == NULL)
	{
		gui_label("!NULL source", gui);
		return ;
	}
	gui_start_horizontal(gui);
	gui_label("Sample: ", gui);
	gui_label(source->sample.name, gui);
	gui_end_horizontal(gui);
	if (gui_highlighted_button_if("Select audio", gui, toggle_select))
		toggle_select = !toggle_select;
	if (toggle_select)
	{
		int	i;
		t_audio_sample	sample;
		i = 0;
		gui->offset.x = 15;
		while (i < gui->sdl->audio.samplecount)
		{
			sample = gui->sdl->audio.samples[i];
			if (gui_button(sample.name, gui))
			{
				source->queue_stop = true;
				source->sample = sample;
			}
			i++;
		}
		gui->offset.x = 0;
	}
	gui_labeled_bool_edit("Play always", &source->play_always, gui);
	gui_labeled_float_slider("Volume", &source->volume, 0.1f, gui);
	source->volume = ft_clampf(source->volume, 0.0f, 1.0f);
	gui_labeled_float_slider("Range", &source->range, 0.5f, gui);
	gui_labeled_int_slider("Random delay(min)", &source->random_delay_min, 0.5f, gui);
	gui_labeled_int_slider("Random delay(max)", &source->random_delay_max, 0.5f, gui);
	source->random_delay_min = ft_clamp(source->random_delay_min, 0, source->random_delay_max);
	source->random_delay_max = ft_clamp(source->random_delay_max, source->random_delay_min, 2000);
	source->range = ft_clampf(source->range, 2.5f, 1000.0f);
	if (gui_button("Play audio", gui))
		source->queue_play = true;
}

void	comp_audiosource_loadassets(t_entity *entity, t_world *world)
{
	t_audio_source	*source;
	source = entity->component.data;
	source->sample = get_sample(world->sdl, source->sample.name);
}

void	comp_audiosource_allocate(t_entity *entity, t_world *world)
{
	t_audio_source	*source;

	entity->component.data = prot_memalloc(sizeof(t_audio_source));
	entity->component.data_size = sizeof(t_audio_source);
	source = (t_audio_source *)entity->component.data;
	source->sample = get_sample(world->sdl, "amb_dogbark1.wav");
	source->volume = 1.0f;
	source->range = 80.0f;
	source->channel = NULL;
}

void	assign_component_audiosource(t_component *component)
{
	component->data_size = sizeof(t_audio_source);
	component->func_allocate = comp_audiosource_allocate;
	component->func_update = comp_audiosource_update;
	component->func_gui_edit = comp_audiosource_gui_edit;
	component->func_ui_update = comp_audiosource_ui_update;
	component->func_load_assets = comp_audiosource_loadassets;
}
