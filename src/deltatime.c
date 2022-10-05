/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:25:00 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/05 12:56:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	update_deltatime(t_clock *c)
{
	uint32_t			time_now;

	time_now = SDL_GetTicks();
	c->delta = time_now - c->prev_time;
	c->prev_time = time_now;
}
