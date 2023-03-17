/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_events.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 19:41:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/17 14:46:28 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int	playmode_events(t_game *game)
{
	static SDL_Event	e;
	
	game->hid.mouse.scroll_delta = 0; //Needs to be reset
	game->hid.alphakey_pressed = 0; //Needs to be reset
	SDL_GetRelativeMouseState(&game->hid.mouse.delta.x, &game->hid.mouse.delta.y);
		/*if (ed->hid.mouse.safe_delta && ((ed->hid.mouse.delta.x) > 30 || ft_abs(ed->hid.mouse.delta.y) > 30))
			ed->hid.mouse.delta = point_zero();*/
	while (SDL_PollEvent(&e))
	{
		toggle_keystates(&game->hid, e);
		mouse_event(e, &game->hid.mouse);
		if (e.type == SDL_KEYDOWN)
		{
			if (iskey(e, SDLK_F1))
				game->player.noclip = !game->player.noclip;
			if (iskey(e, SDLK_ESCAPE))
				exit(0);
		}
		if (e.type == SDL_CONTROLLERBUTTONDOWN)
		{
			if (e.cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
				return (game_switchmode);
			if (e.cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
				return (game_exit);
		}
	}
	updateinput_new(&game->hid.input, game->hid);
	return (game_continue);
}
