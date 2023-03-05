/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_window_events.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/02/07 15:54:56 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	check_for_quit(t_parent *p)
{
	if ((p->ew.event.type == SDL_WINDOWEVENT && \
		p->ew.event.window.event == SDL_WINDOWEVENT_CLOSE) || \
		(p->ew.event.type == SDL_KEYDOWN && \
		p->ew.event.key.keysym.sym == SDLK_ESCAPE))
		exit (1);
}

void	error_window_events(t_parent *p)
{
	while (SDL_PollEvent(&p->ew.event))
	{
		check_for_quit(p);
		ft_bzero(p->sdl.window_surface->pixels, sizeof(uint32_t) * 600 * 300);
		ft_bzero(p->sdl.surface->pixels, sizeof(uint32_t) * 600 * 300);
		mouse_event(p->ew.event, &p->ew.hid.mouse);
		if (p->message_count > 21)
			p->ew.i = ft_clamp((p->ew.i - p->ew.hid.mouse.scroll_delta), 0, \
							(p->message_count - 21));
		p->ew.j = p->ew.i;
		while (p->ew.j < p->message_count)
		{
			if (p->ew.j >= 0 && p->ew.j < p->message_count)
				print_text(&p->sdl, p->messages[p->ew.j], \
					(t_point){10, (10 + (p->ew.j - p->ew.i) * 13)});
			p->ew.j++;
		}
		memcpy(p->sdl.window_surface->pixels, p->sdl.surface->pixels, \
				sizeof(uint32_t) * 600 * 300);
		if (SDL_UpdateWindowSurface(p->sdl.window) < 0)
		{
			ft_putendl_fd("can't update sdl surface for error message", 2);
			exit (1);
		}
	}
}
