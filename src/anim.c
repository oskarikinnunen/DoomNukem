/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:56:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/09 16:21:13 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"
#include "doomnukem.h"
#include "libft.h"
#include "render.h"
#include <stdio.h>

void	update_anim(t_anim *anim, uint32_t delta)
{
	float	now_second;

	if (anim->active == false)
		return ;
	anim->time += delta;
	now_second = (float)anim->time / 1000.0f;
	if (anim->mode == anim_forwards)
	{
		if (anim->frame >= anim->lastframe)
		{
			anim->frame = anim->startframe;
			if (!anim->loop)
				anim->active = false;
			else
				anim->time = 0;
		}
		else 
		{
			anim->frame = (int)(anim->framerate * now_second) + anim->startframe;
			anim->frame = ft_clampf(anim->frame, anim->startframe, anim->lastframe);
		}
		/* if (anim->audioevent != NULL && anim->frame == anim->audioevent->frame)
		{
			force_play_audio(*anim->audioevent->audio);
		} */
	}
	if (anim->mode == anim_backwards)
	{
		if (anim->frame < 0)
			anim->active = false;
		else
			anim->frame = anim->lastframe - (int)(anim->framerate * now_second);
	}
	anim->lerp = ((float)anim->frame / (float)anim->lastframe);
	anim->lerp = ft_clampf(anim->lerp, 0.0f, 1.0f);
}

void	update_anim_dir(t_anim *anim, uint32_t delta, t_anim_mode mode)
{
	anim->mode = mode;
	update_anim(anim, delta);
}

void	start_human_anim(t_entity *entity, char *name, t_world *world)
{
	static t_human_animation	*anims_l;
	t_human_animation			anim;
	int							i;

	i = 0;
	while (i < world->sdl->human_anim_count)
	{
		anim = world->sdl->human_anims[i];
		if (ft_strequ(name, anim.name))
		{
			entity->animation.startframe = anim.startframe + 1;
			entity->animation.lastframe = anim.endframe;
			entity->animation.framerate = 30;
			entity->animation.loop = true;
			printf("found anim %s, anim frames: %i-%i\n", name, entity->animation.startframe, entity->animation.lastframe);
			start_anim(&entity->animation, anim_forwards);
			return ;
		}
		i++;
	}
}

t_human_animation	*human_anims()
{
	return (NULL);
}

void	start_anim(t_anim *anim, t_anim_mode mode)
{
	anim->mode = mode;
	anim->time = 0;
	anim->active = true;
	if (anim->mode == anim_forwards) //TODO: loop
		anim->frame = anim->startframe;
	else if (anim->mode == anim_backwards)
		anim->frame = anim->lastframe;
	else
	{
		printf("ERROR: ANIM MODE NOT IMPLEMENTED! \n");
		exit(0);
	}
}
