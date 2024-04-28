/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:56:20 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:01:22 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"
#include "doomnukem.h"
#include "render.h"

static void	anim_end(t_anim *anim)
{
	if (!anim->loop && !anim->persist)
		anim->active = false;
	else
		anim->time = 0;
	if (anim->persist)
		anim->frame = anim->last_frame;
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
		if (anim->frame >= anim->last_frame)
		{
			anim_end(anim);
		}
		else
		{
			anim->frame = (int)(anim->frame_rate * now_second)
				+ anim->start_frame;
			anim->frame = ft_clampf(anim->frame,
					anim->start_frame, anim->last_frame);
		}
	}
	anim->lerp = ((float)anim->frame / (float)anim->last_frame);
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
			entity->animation.start_frame = anim.start_frame + 1;
			entity->animation.last_frame = anim.endframe;
			entity->animation.frame_rate = 30;
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
		anim->frame = anim->start_frame;
	else if (anim->mode == anim_backwards)
		anim->frame = anim->last_frame;
	else
		doomlog(LOG_FATAL, "ERROR: ANIM MODE NOT IMPLEMENTED!");
}
