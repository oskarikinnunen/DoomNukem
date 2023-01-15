/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 10:34:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/15 04:21:59 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "entity.h"

void	comp_interactable_ui_update(t_entity *entity, t_world *world)
{
	t_interactable	*inter;

	inter = entity->component.data;
	if (inter == NULL)
	{
		render_ball(world->sdl, entity->transform.position, 20.0f, CLR_RED);
		//TODO: error ball
		return ;
	}
	render_ball(world->sdl, entity->transform.position, inter->radius, CLR_GREEN);
	float dist = vector3_sqr_dist(entity->transform.position, world->player->transform.position);
	if (dist < inter->radius * 100.0f && !entity->occlusion.is_occluded)
		print_text_boxed(world->sdl, "[E] interact", point_div(world->sdl->screensize, 2));
}

void	comp_interactable_update(t_entity *entity, t_world *world)
{
	t_interactable	*inter;

	inter = entity->component.data;
	if (inter == NULL)
	{
		return ;
	}
	/*float dist = vector3_sqr_dist(entity->transform.position, world->player->transform.position);

	if (dist < 2000.0f)
		print_text_boxed(world->sdl, "[E] interact", point_div(world->sdl->screensize, 2));*/
		
	/*
		vector3 sqrdist to player
		printf
	
	
	*/
}

void	comp_light_update(t_entity *entity, t_world *world)
{
	t_pointlight	*pointlight;

	pointlight = entity->component.data;
	if (pointlight == NULL)
	{
		return ;
	}
}

void	draw_entity_icon(t_entity *entity, t_img *img, t_world *world)
{
	float	qdist;
	qdist = vector3_sqr_dist(entity->transform.position, world->player->transform.position);
	t_point p = vector3_to_screenspace(entity->transform.position, *world->sdl);
	if (qdist < 16000.0f && !entity->occlusion.is_occluded)
		draw_image(*world->sdl, p, *img, (t_point){32,32});
	else
		draw_image(*world->sdl, p, *img, (t_point){16,16});
}

void	comp_light_ui_update(t_entity *entity, t_world *world)
{
	static	t_img	*icon;
	t_pointlight	*pointlight;

	if (icon == NULL)
		icon = get_image_by_name(*world->sdl, "light_icon");
	pointlight = entity->component.data;
	if (pointlight == NULL)
	{
		render_ball(world->sdl, entity->transform.position, 20.0f, CLR_RED);
		return ;
	}
	draw_entity_icon(entity, icon, world);
	//render_ball(world->sdl, entity->transform.position, pointlight->radius, AMBER_4);
	/*float dist = vector3_sqr_dist(entity->transform.position, world->player->transform.position);
	if (dist < inter->radius * 100.0f && !entity->occlusion.is_occluded)
		print_text_boxed(world->sdl, "[E] interact", point_div(world->sdl->screensize, 2));*/
}

bool	source_is_playing(t_audiosource *source)
{
	FMOD_BOOL	isplaying;

	isplaying = false;
	if (source->channel != NULL)
	{
		if (FMOD_Channel_IsPlaying(source->channel, &isplaying) != FMOD_OK)
			isplaying = false;
	}
	return (isplaying);
}

uint32_t	pseudorandom(t_world *world)
{
	static uint32_t r = INT_MAX / 2;

	r += INT_MAX / 4;
	r = r << (int)(world->clock.prev_time);
	return (r);
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
	{
		if (entity->occlusion.is_occluded)
			source->_realrange = ft_fmovetowards(source->_realrange, source->range * 0.35f, world->clock.delta * 0.20f);
		else
			source->_realrange = ft_fmovetowards(source->_realrange, source->range, world->clock.delta * 0.20f);
		FMOD_Channel_Set3DAttributes(source->channel, &entity->transform.position, &((t_vector3){0}));
		FMOD_Channel_Set3DMinMaxDistance(source->channel, source->_realrange, 10000.0f);
		FMOD_Channel_SetVolume(source->channel, source->volume * world->sdl->audio.sfx_volume);
	}
	if (!isplaying && source->play_always)
	{
		if (source->random_delay_min != 0)
		{
			if (source->_nextstart == 0)
			{
				uint32_t rand = pseudorandom(world) % ((source->random_delay_max - source->random_delay_min) * 1000);
				source->_nextstart = world->clock.prev_time + (source->random_delay_min * 1000) + rand;
				printf("pseudorandom was %i \n", rand);
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
		audiosource_start(world->sdl, source, &entity->transform.position);
		source->queue_play = false;
	}
	if (source->queue_stop)
	{
		FMOD_Channel_Stop(source->channel);
		source->queue_stop = false;
	}
	/*if (!isplaying && source->loop)
	{
		audiosource_start(world->sdl, source, &entity->transform.position);
	}*/
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
	//render_ball(world->sdl, entity->transform.position, source->maxdistance, CLR_RED);
	/*float dist = vector3_sqr_dist(entity->transform.position, world->player->transform.position);
	if (dist < inter->radius * 100.0f && !entity->occlusion.is_occluded)
		print_text_boxed(world->sdl, "[E] interact", point_div(world->sdl->screensize, 2));*/
}

void	component_init(t_entity	*entity)
{
	if (entity->component.type == pft_none)
	{
		entity->component.update = NULL;
		entity->component.ui_update = NULL;
	}
	if (entity->component.type == pft_audiosource)
	{
		t_audiosource	*source;
		source = entity->component.data;
		//if (source != NULL)
		//	source->
		entity->component.update = comp_audiosource_update;
		entity->component.ui_update = comp_audiosource_ui_update;
	}
	if (entity->component.type == pft_interactable)
	{
		entity->component.update = comp_interactable_update;
		entity->component.ui_update = comp_interactable_ui_update;
	}
	if (entity->component.type == pft_light)
	{
		entity->component.update = NULL;
		entity->component.ui_update = comp_light_ui_update;
		t_pointlight *light = entity->component.data;
		light->done = true;
	}
	
	
}