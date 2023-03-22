/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:56:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 14:51:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"
#include "doomnukem.h"
#include "libft.h"
#include "render.h"
#include <stdio.h>

static void	anim_end(t_anim *anim)
{
	if (!anim->loop && !anim->persist)
		anim->active = false;
	else
		anim->time = 0;
	if (anim->persist)
		anim->frame = anim->lastframe;
}

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
			anim_end(anim);
		}
		else
		{
			anim->frame = (int)(anim->framerate * now_second)
				+ anim->startframe;
			anim->frame = ft_clampf(anim->frame,
					anim->startframe, anim->lastframe);
		}
	}
	anim->lerp = ((float)anim->frame / (float)anim->lastframe);
	anim->lerp = ft_clampf(anim->lerp, 0.0f, 1.0f);
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
			start_anim(&entity->animation, anim_forwards);
			return ;
		}
		i++;
	}
}

void	start_anim(t_anim *anim, t_anim_mode mode)
{
	anim->mode = mode;
	anim->time = 0;
	anim->active = true;
	if (anim->mode == anim_forwards)
		anim->frame = anim->startframe;
	else if (anim->mode == anim_backwards)
		anim->frame = anim->lastframe;
	else
	{
		printf("ERROR: ANIM MODE NOT IMPLEMENTED! \n");
		exit(0);
	}
}
