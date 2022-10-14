/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:25:00 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 12:30:41 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	update_deltatime(t_clock *c)
{
	uint32_t			time_now;
	static uint32_t			prev_time;

	time_now = SDL_GetTicks();
	c->delta = time_now - prev_time;
	prev_time = time_now;
}
