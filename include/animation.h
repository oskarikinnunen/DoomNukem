/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 00:52:45 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/31 00:55:42 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATION_H
# define ANIMATION_H

# include <inttypes.h>
# include <stdbool.h>

//TODO: add ping-pong, repeat
typedef enum	e_anim_mode
{
	anim_forwards,
	anim_backwards
} t_anim_mode;

typedef struct s_anim
{
	bool		active;
	uint32_t	time;
	int32_t		frame;
	int32_t		lastframe;
	uint8_t		framerate;
	float		lerp;
	t_anim_mode	mode;
}	t_anim;

void	update_anim(t_anim *anim, uint32_t delta);
void	start_anim(t_anim *anim, t_anim_mode mode); //TODO: make this return t_anim?

#endif