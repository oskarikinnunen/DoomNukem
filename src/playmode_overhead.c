/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_overhead.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:46:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/15 12:55:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"


void	move_overhead(t_game *game)
{
	t_vector2	move_vector;
	
	move_vector = vector2_zero();
	move_vector.x += (game->keystate >> KEYS_LEFTMASK) & 1;
	move_vector.x -= (game->keystate >> KEYS_RGHTMASK) & 1;
	move_vector.y += (game->keystate >> KEYS_UPMASK) & 1;
	move_vector.y -= (game->keystate >> KEYS_DOWNMASK) & 1;
	move_vector = vector2_mul(move_vector, game->clock.delta * OVERHEADCAMSPEED);
	game->overheadcam_pos = vector2_add(game->overheadcam_pos, move_vector);
}

static void	draw_walls(t_list *linelist, t_sdlcontext *sdl, t_vector2 offset)
{
	t_list	*node;
	t_line	line;

	node = linelist;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		line.start = point_mul(line.start, TILESIZE);
		line.start = point_add(line.start, (t_point){offset.x * TILESIZE, offset.y * TILESIZE});
		line.end = point_mul(line.end, TILESIZE);
		line.end = point_add(line.end, (t_point){offset.x * TILESIZE, offset.y * TILESIZE});
		drawline(sdl->surface->pixels, line.start, line.end, CLR_TURQ);
		node = node->next;
	}
}

//TODO: add some kind of "camera position" abstraction, this just draws from (0,0) perspective and all the walls are shifted
//TODO: re-add player_ball draw for overhead_absolute camera mode
//TODO: player position in "overhead follow" is off by half a window size
void	render_overhead(t_game *game, t_sdlcontext *sdl)
{
	t_point		player_ball; //player coordinates  in screenspace
	t_point		look_to; // line end coordinate that is towards player look direction
	t_vector2	walls_offset;

	look_to.x = sin(game->player.angle) * 40;
	look_to.y = cos(game->player.angle) * 40;
	if (game->cam_mode == overhead_absolute)
	{
		player_ball = point_zero(); //for now
		//TODO: Calculate screen coordinates for player "ball" here
		//player_pos = vector2_to_point(game->player.position);
		draw_walls(game->linelst, sdl, game->overheadcam_pos);
	}
	else if  (game->cam_mode == overhead_follow)
	{
		player_ball.x = WINDOW_W / 2;
		player_ball.y = WINDOW_H / 2;
		walls_offset = vector2_negative(game->player.position);
		draw_walls(game->linelst, sdl, walls_offset);
	}
	look_to = point_add(look_to, player_ball);
	if (game->cam_mode == overhead_follow) {
		drawcircle(sdl->surface->pixels, player_ball, PLAYERRADIUS, CLR_PRPL);
		drawline(sdl->surface->pixels, player_ball, look_to, CLR_PRPL);
	}
}