/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_audiosource.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/18 10:32:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/*
uint32_t	starttime;
uint32_t	lifetime;
t_anim	particleanim;
t_img	particleimages[16];
*/

void	comp_audiosource_update(t_entity *entity, t_world *world)
{
	t_audiosource	*source;
	bool			isplaying;
	//entity->particleimages[particleanim.];
	source = entity->component.data;
	if (source == NULL || source->sample.sound == NULL)
	{
		return ;
	}
	
	/*if (world->clock.prev_time > starttime + lifetime)
	{
		//free_object()
		destroy_entity(world, entity);
		return ;
	}*/

	isplaying = source_is_playing(source);
	if (isplaying)
	{
		if (entity->occlusion.is_occluded)
			source->_realrange = ft_fmovetowards(source->_realrange, source->range * 0.35f, world->clock.delta * 0.20f);
		else
			source->_realrange = ft_fmovetowards(source->_realrange, source->range, world->clock.delta * 0.20f);
		FMOD_Channel_Set3DAttributes(source->channel,
			(FMOD_VECTOR *)&entity->transform.position, &((FMOD_VECTOR){0}));
		FMOD_Channel_Set3DMinMaxDistance(source->channel, source->_realrange, 10000.0f);
		FMOD_Channel_SetVolume(source->channel, source->volume * world->sdl->audio.sfx_volume);
	}
	if (!isplaying && source->play_always)
	{
		if (source->random_delay_min != 0)
		{
			if (source->_nextstart == 0)
			{
				uint32_t	r = game_random_range(world,
					source->random_delay_min * 1000,
					source->random_delay_max * 1000);
				source->_nextstart = world->clock.prev_time + r;
				printf("pseudorandom was %i \n", r);
			}
			else if (world->clock.prev_time > source->_nextstart)
			{
				source->_nextstart = 0;
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
	render_ball(world->sdl, entity->transform.position, source->range * 1.0f, CLR_BLUE);
}

void	comp_audiosource_gui_edit(t_component *component, t_autogui *gui)
{
	t_audiosource	*source;
	static bool		toggle_select;
	source = component->data;
	
	if (source == NULL)
	{
		//start_anim()
		gui_label("!NULL source", gui);
		return ;
	}
	gui_starthorizontal(gui);
	gui_label("Sample: ", gui);
	gui_label(source->sample.name, gui);
	gui_endhorizontal(gui);
	if (gui_highlighted_button_if("Select audio", gui, toggle_select))
		toggle_select = !toggle_select;
	if (toggle_select)
	{
		int	i;
		t_audiosample	sample;
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
	{
		source->queue_play = true;
		//audiosource_start(gui->sdl, source, &entity->transform.position);
	}
}

void	comp_audiosource_loadassets(t_entity *entity, t_world *world)
{
	t_audiosource	*source;
	source = entity->component.data;
	if (source == NULL)
		error_log(EC_MALLOC);
	source->sample = get_sample(world->sdl, source->sample.name);
}

/*typedef struct s_savedvalue
{
	//char	name[32];
	uint32_t	id; //TODO: figure out how to iterate through all of these...
	
}	t_savedvalue,*/

void	comp_audiosource_allocate(t_entity *entity, t_world *world)
{
	t_audiosource	*source;

	entity->component.data = ft_memalloc(sizeof(t_audiosource));
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
	component->func_loadassets = comp_audiosource_loadassets;
	//spawn_entity
	//addcomponent(pft_audiosource)
}
