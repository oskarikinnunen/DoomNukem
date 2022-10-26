/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_overhead.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:46:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/26 15:09:35 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"


void	move_overhead(t_game *game)
{
	t_vector2	move_vector;
	
	move_vector = vector2_zero();
	move_vector.x += (game->keystate >> KEYS_LEFTMASK) & 1;
	move_vector.x -= (game->keystate >> KEYS_RIGHTMASK) & 1;
	move_vector.y += (game->keystate >> KEYS_UPMASK) & 1;
	move_vector.y -= (game->keystate >> KEYS_DOWNMASK) & 1;
	move_vector = vector2_mul(move_vector, game->clock.delta * OVERHEADCAMSPEED);
	game->overheadcam_pos = vector2_add(game->overheadcam_pos, move_vector);
}

static void	draw_walls(t_list *linelist, t_sdlcontext sdl, t_vector2 offset)
{
	t_list	*node;
	t_line	line;

	node = linelist;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		line.start = point_mul(line.start, TILESIZE);
		line.start = point_add(line.start, (t_point){offset.x, offset.y});
		line.end = point_mul(line.end, TILESIZE);
		line.end = point_add(line.end, (t_point){offset.x, offset.y});
		drawline(sdl, line.start, line.end, CLR_TURQ);
		node = node->next;
	}
}

//TODO: add some kind of "camera position" abstraction, this just draws from (0,0) perspective and all the walls are shifted
//TODO: re-add player_ball draw for overhead_absolute camera mode
//TODO: player position in "overhead follow" is off by half a window size
void	render_overhead(t_game *game, t_sdlcontext sdl)
{
	t_point		player_ball; //player coordinates  in screenspace
	t_point		look_to; // line end coordinate that is towards player look direction
	t_vector3	normalized_forward;
	t_vector2	walls_offset;

	player_ball.x = sdl.window_w / 2; //DIS
	player_ball.y = sdl.window_h / 2;
	walls_offset = vector2_negative((t_vector2){game->player.position.x, game->player.position.y});
	//walls_offset = vector2_sub(walls_offset, point_to_vector2(player_ball));
	walls_offset = vector2_add(walls_offset, (t_vector2){player_ball.x, player_ball.y});
	draw_walls(game->linelist, sdl, walls_offset);
	normalized_forward = vector3_normalise(game->player.lookdir);
	look_to = (t_point) {normalized_forward.x * 20, normalized_forward.y * 20};
	look_to = point_add(look_to, player_ball);
	drawline(sdl, player_ball, look_to, CLR_PRPL);
	//	drawline(sdl->surface->pixels, player_ball, PLAYERRADIUS, CLR_PRPL);
	drawcircle(sdl, player_ball, PLAYERRADIUS, CLR_PRPL);
}