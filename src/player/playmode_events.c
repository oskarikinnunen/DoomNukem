/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_events.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 19:41:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 19:41:41 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int	playmode_events(t_game *game)
{
	static SDL_Event	e;

	game->hid.mouse.scroll_delta = 0;
	game->hid.alphakey_pressed = 0;
	SDL_GetRelativeMouseState(&game->hid.mouse.delta.x,
		&game->hid.mouse.delta.y);
	while (SDL_PollEvent(&e))
	{
		toggle_keystates(&game->hid, e);
		mouse_event(e, &game->hid.mouse);
		if (e.type == SDL_KEYDOWN)
		{
			if (iskey(e, SDLK_F1))
				game->player.noclip = !game->player.noclip;
			if (iskey(e, SDLK_ESCAPE))
			{
				if ((game->hid.keystate >> KEYS_SHIFTMASK) & 1)
					exit(0);
				return (game_exit);
			}
		}
		controller_events(e, &game->hid);
	}
	updateinput_new(&game->hid.input, game->hid);
	return (game_continue);
}
