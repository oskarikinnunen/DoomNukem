/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:56:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/17 20:43:40 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "doomnukem.h"

void	update_anim(t_anim *anim, uint32_t delta)
{
	float	now_second;

	if (!anim->active)
		return ;
	anim->time += delta;
	now_second = (float)anim->time / 1000.0f;
	if (anim->mode == anim_forwards)
	{
		if (anim->frame >= anim->lastframe)
			anim->active = false;
		else
			anim->frame = (int)(anim->framerate * now_second);
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

void	start_anim(t_anim *anim, t_anim_mode mode)
{
	anim->mode = mode;
	anim->time = 0;
	anim->active = true;
	if (anim->mode == anim_forwards)
		anim->frame = 0;
	else if (anim->mode == anim_backwards)
		anim->frame = anim->lastframe;
	else
	{
		printf("ERROR: ANIM MODE NOT IMPLEMENTED! \n");
		exit(0);
	}
}
