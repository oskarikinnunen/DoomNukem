/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:25:00 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 13:06:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

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
		ft_putstr("FPS: ");
		ft_putnbr(1000 / (add / betweenframes));
		ft_putstr("\n");
		add = 0;
		betweenframes = 0;
		prev_fpstime = time_now;
	}
}
