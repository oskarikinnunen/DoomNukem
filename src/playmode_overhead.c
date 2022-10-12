/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_overhead.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:46:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/11 15:54:46 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"

//TODO: add "camera position", this just draws from (0,0) perspective

void	move_overhead(t_game *game)
{
	float	move_f2[2];
	
	ft_bzero(move_f2, sizeof(float[2]));
	move_f2[X] += (game->keystate >> KEYS_LEFTMASK) & 1;
	move_f2[X] -= (game->keystate >> KEYS_RIGHTMASK) & 1;
	move_f2[Y] += (game->keystate >> KEYS_UPMASK) & 1;
	move_f2[Y] -= (game->keystate >> KEYS_DOWNMASK) & 1;
	f2mul(move_f2, game->clock.delta * OVERHEADCAMSPEED);
	f2add(game->overheadcam_pos, move_f2);
}

static void	draw_walls(t_list *linelist, t_sdlcontext *sdl, float offset[2])
{
	t_list	*node;
	t_line	line;
	int		offset_i[2];

	node = linelist;
	f2tov2(offset, offset_i);
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		v2mul(line.start, TILESIZE);
		v2mul(line.end, TILESIZE);
		v2add(line.start, offset_i);
		v2add(line.end, offset_i);
		drawline(sdl->surface->pixels, line.start, line.end, CLR_TURQ);
		node = node->next;
	}
}

void	render_overhead(t_game *game, t_sdlcontext *sdl)
{
	int		player_pos[2]; //player coordinates (screenspace) in integer vector
	int		look_to[2]; // line end coordinate that is towards player look direction
	float	walls_offset[2];

	look_to[X] = sin(game->player.angle[X]) * 40;
	look_to[Y] = cos(game->player.angle[X]) * 40;
	if (game->cam_mode == overhead_absolute)
	{
		f2tov2(game->player.position, player_pos);
		int temp[2];
		f2tov2(game->overheadcam_pos, temp);
		v2add(player_pos, temp);
		draw_walls(game->linelst, sdl, game->overheadcam_pos);
	}
	else if  (game->cam_mode == overhead_follow)
	{
		player_pos[X] = WINDOW_W / 2;
		player_pos[Y] = WINDOW_H / 2;
		f2add(walls_offset, game->player.position);
		walls_offset[X] = -game->player.position[X];
		walls_offset[Y] = -game->player.position[Y];
		walls_offset[X] += player_pos[X];
		walls_offset[Y] += player_pos[Y];
		draw_walls(game->linelst, sdl, walls_offset);
	}
	v2add(look_to, player_pos);
	drawline(sdl->surface->pixels, player_pos, look_to, CLR_PRPL);
	if (game->cam_mode == overhead_follow)
		drawcircle(sdl->surface->pixels, player_pos, PLAYERRADIUS, CLR_PRPL);
	else
		drawcircle(sdl->surface->pixels, player_pos, PLAYERRADIUS, CLR_GRAY);
}