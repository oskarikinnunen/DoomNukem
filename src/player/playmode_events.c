/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_events.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 19:41:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 10:41:36 by kfum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void	playmode_quit(t_game *game)
{
	SDL_Quit();
	exit(0);
}

void	key_event(SDL_Event e, t_game *game)
{
	if (iskey(e, SDLK_F1))
		game->player.no_clip = !game->player.no_clip;
	if (iskey(e, SDLK_F2))
		game->world.debug_gui->hidden = !game->world.debug_gui->hidden;
	if (iskey(e, SDLK_ESCAPE))
		playmode_quit(game);
}

void	playmode_events(t_game *game)
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
			key_event(e, game);
		if (e.type == SDL_QUIT)
			playmode_quit(game);
		controller_events(e, &game->hid);
	}
	update_input(&game->hid.input, game->hid);
}
