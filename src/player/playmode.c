/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:44:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 21:59:44 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "input.h"

/*main game loop*/
static void	gameloop(t_sdlcontext *sdl, t_game *game)
{
	while (1)
	{
		if (game->player.health > 0)
		{
			update_clock(&game->world.clock);
			playmode_events(game);
			moveplayer(&game->player, &game->hid.input, &game->world);
			update_world3d(&game->world, &sdl->render);
			draw_player_hud(&game->world);
			draw_fps_counter(sdl, game->world.clock.fps);
			update_window_surface(sdl);
			update_audio(&game->world);
			game_random(&game->world);
		}
		else
			playmode_death(game);
	}
}

/*setup and call gameloop*/
void	playmode(char *level, t_sdlcontext sdl)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	playmode_loading_screen("LOADING WORLD", &sdl);
	game.world = load_world(level, &sdl);
	playmode_loading_screen("SETTING UP WORLD", &sdl);
	set_up_world(&sdl, &game);
	playmode_loading_screen("CREATING LIGHTING EFFECTS", &sdl);
	recalculate_lighting(&game.world);
	playmode_loading_screen_loop("PRESS ANY KEY TO PLAY", &sdl);
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
		doomlog(LOG_EC_SDL_SETRELATIVEMOUSEMODE, NULL);
	protagonist_play_audio(&game.player, &game.world, "protag_letsdo.wav");
	gameloop(&sdl, &game);
}
