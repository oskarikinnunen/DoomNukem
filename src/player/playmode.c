/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:44:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 22:53:47 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "input.h"

/*main game loop*/
static void	gameloop(t_sdlcontext *sdl, t_game *game)
{
	update_clock(&game->world.clock);
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
void	playmode(t_app_argument app_argument, t_sdlcontext sdl)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	playmode_loading_screen("LOADING WORLD", &sdl);
	game.world = load_world(app_argument, &sdl);
	game.world.sdl->render.world = &game.world;
	playmode_loading_screen("SETTING UP WORLD", &sdl);
	set_up_world(&sdl, &game);
	playmode_loading_screen("CREATING LIGHTMAPS", &sdl);
	recalculate_lighting(&game.world);
	playmode_loading_screen("CREATING NAVMESH", &sdl);
	navmesh(&game.world);
	playmode_loading_screen_loop("PRESS ANY KEY TO PLAY", &sdl);
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
		doomlog(LOG_EC_SDL_SETRELATIVEMOUSEMODE, NULL);
	protagonist_play_audio(&game.player, &game.world, "protag_letsdo.wav");
	gameloop(&sdl, &game);
}
