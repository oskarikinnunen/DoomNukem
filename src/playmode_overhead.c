/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_overhead.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:46:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 11:32:32 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

//TODO: add "camera position", this just draws from (0,0) perspective

static void	draw_walls(t_list *linelist, t_sdlcontext *sdl, int offset[2])
{
	t_list	*node;
	t_line	line;

	node = linelist;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		v2mul(line.start, TILESIZE);
		v2mul(line.end, TILESIZE);
		v2add(line.start, offset);
		v2add(line.end, offset);
		drawline(sdl->surface->pixels, line.start, line.end, CLR_TURQ);
		node = node->next;
	}
}

void	render_overhead(t_game *game, t_sdlcontext *sdl)
{
	int	player_pos[2]; //player coordinates in integer vector
	int	look_to[2]; // line end coordinate that is towards player look direction
	int	walls_offset[2];

	look_to[X] = sin(game->player.angle) * 40;
	look_to[Y] = cos(game->player.angle) * 40;
	if (game->cam_mode == overhead_absolute)
	{
		f2tov2(game->player.position, player_pos);
		draw_walls(game->linelst, sdl, (int [2]){0, 0});
	}
	else if  (game->cam_mode == overhead_follow)
	{
		player_pos[X] = WINDOW_W / 2;
		player_pos[Y] = WINDOW_H / 2;
		v2cpy(walls_offset, player_pos);
		v2add(walls_offset,
			(int [2]) {-game->player.position[X], -game->player.position[Y]});
		draw_walls(game->linelst, sdl, walls_offset);
	}
	v2add(look_to, player_pos);
	drawline(sdl->surface->pixels, player_pos, look_to, CLR_PRPL);
	drawcircle(sdl->surface->pixels, player_pos, 16, CLR_PRPL);
}