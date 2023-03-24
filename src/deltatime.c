/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:25:00 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 19:24:31 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	draw_fps_counter(t_sdlcontext *sdl, t_game *game)
{
	print_text(sdl, s_itoa(game->world.clock.fps), \
				(t_point){sdl->window_w - 80, 10});
}

void	update_clock(t_clock *c)
{
	uint32_t		time_now;
	static uint32_t	prev_fpstime;
	static uint32_t	add;
	static uint32_t	betweenframes;

	time_now = SDL_GetTicks() + (UINT32_MAX - 5000);
	c->delta = time_now - c->time;
	c->time = time_now;
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
