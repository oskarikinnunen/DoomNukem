/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:25:00 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/04 17:25:31 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"


//TODO:
//	remove t_clock struct and just have uint32 time in game struct and ed struct
void	update_deltatime(t_clock *c)
{
	uint32_t		time_now;
	static uint32_t	prev_fpstime;
	static uint32_t	add;
	static uint32_t	betweenframes;

	time_now = SDL_GetTicks();
	c->delta = time_now - c->prev_time;
	c->prev_time = time_now;
	add += c->delta;
	betweenframes++;
	if (time_now >= prev_fpstime + 1000)
	{
		c->fps = 1000 / (add / betweenframes);
		add = 0;
		betweenframes = 0;
		prev_fpstime = time_now;
	}
}
