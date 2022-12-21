/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:56:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/21 17:01:37 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"
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
			anim->frame = 0;
			if (!anim->loop)
			{
				anim->active = false;
			}
			else
				anim->time = 0;
		}
		else 
		{
			anim->frame = (int)(anim->framerate * now_second);
			anim->frame = ft_clampf(anim->frame, 0, anim->lastframe);
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

void	start_anim(t_anim *anim, t_anim_mode mode)
{
	anim->mode = mode;
	anim->time = 0;
	anim->active = true;
	if (anim->mode == anim_forwards) //TODO: loop
		anim->frame = 0;
	else if (anim->mode == anim_backwards)
		anim->frame = anim->lastframe;
	else
	{
		printf("ERROR: ANIM MODE NOT IMPLEMENTED! \n");
		exit(0);
	}
}
