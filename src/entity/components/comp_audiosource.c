/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_audiosource.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:45:27 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void	comp_audiosource_queue(t_entity *entity,
		t_audiosource *source, t_world *world)
{
	uint32_t	r;

	if (source->random_delay_min != 0)
	{
		if (source->_next_start == 0)
		{
			r = game_random_range(world,
					source->random_delay_min * 1000,
					source->random_delay_max * 1000);
			source->_next_start = world->clock.time + r;
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

static void	audiosource_fmod_update(t_entity *entity,
		t_audiosource *source, t_world *world)
{
	if (entity->occlusion.is_occluded)
		source->_real_range = ft_fmovetowards(source->_real_range,
				source->range * 0.35f, world->clock.delta * 0.20f);
	else
		source->_real_range = ft_fmovetowards(source->_real_range,
				source->range, world->clock.delta * 0.20f);
	FMOD_Channel_Set3DAttributes(source->channel,
		(FMOD_VECTOR *)&entity->transform.position, &((FMOD_VECTOR){0}));
	FMOD_Channel_Set3DMinMaxDistance(source->channel,
		source->_real_range, 10000.0f);
	FMOD_Channel_SetVolume(source->channel, source->volume
		* world->sdl->audio.sfx_volume);
}

void	comp_audiosource_update(t_entity *entity, t_world *world)
{
	t_audiosource	*source;
	bool			isplaying;

	source = entity->component.data;
	if (source == NULL || source->sample.sound == NULL)
	{
		return ;
	}
	isplaying = source_is_playing(source);
	if (isplaying)
		audiosource_fmod_update(entity, source, world);
	if (!isplaying && source->play_always)
		comp_audiosource_queue(entity, source, world);
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
