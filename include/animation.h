/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 00:52:45 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/06 17:05:06 by okinnune         ###   ########.fr       */
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

struct s_audio ;

typedef struct s_audioevent
{
	uint32_t		frame;
	struct s_audio	*audio;
	char			audioname[64];
}	t_audioevent;

typedef struct s_anim
{
	bool			active;
	bool			loop;
	uint32_t		time;
	int32_t			frame;
	int32_t			lastframe;
	uint8_t			framerate;
	float			lerp;
	t_anim_mode		mode;
	t_audioevent	*audioevent;
}	t_anim;

void	update_anim(t_anim *anim, uint32_t delta);
void	update_anim_dir(t_anim *anim, uint32_t delta, t_anim_mode mode);
void	start_anim(t_anim *anim, t_anim_mode mode); //TODO: make this return t_anim?

#endif