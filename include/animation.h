/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 00:52:45 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:24:06 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATION_H
# define ANIMATION_H

# include <inttypes.h>
# include <stdbool.h>

struct	s_audio;
struct	s_entity;
struct	s_world;

//TODO: add ping-pong, repeat
typedef enum e_anim_mode
{
	anim_forwards,
	anim_backwards,
}	t_anim_mode;

typedef struct s_audio_event
{
	uint32_t		frame;
	struct s_audio	*audio;
	char			audio_name[64];
}	t_audio_event;

typedef struct s_anim
{
	bool			active;
	bool			loop;
	bool			persist;
	uint8_t			frame_rate;
	uint32_t		time;
	int32_t			frame;
	int32_t			start_frame;
	int32_t			last_frame;
	float			lerp;
	t_anim_mode		mode;
	t_audio_event	*audio_event;
}	t_anim;

void	update_anim(t_anim *anim, uint32_t delta);
void	anim_set_frame(t_anim *anim, uint32_t frame);
void	update_anim_dir(t_anim *anim, uint32_t delta, t_anim_mode mode);
//TODO: make this return t_anim?
void	start_anim(t_anim *anim, t_anim_mode mode);
void	start_human_anim(struct s_entity *entity, char *name, \
							struct s_world *world);

#endif