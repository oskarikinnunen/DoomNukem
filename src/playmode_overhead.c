/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_overhead.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:46:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/14 18:35:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "inputhelp.h"

//TODO: add "camera position", this just draws from (0,0) perspective

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
		//line.start = point_add(line.start, vector2_to_point(offset));
		line.start = point_mul(line.start, TILESIZE);
		//line.start = point_add(line.start, vector2_to_point(vector2_div(offset, TILESIZE)));
		line.start = point_add(line.start, (t_point){offset.x * TILESIZE, offset.y * TILESIZE});
		line.end = point_mul(line.end, TILESIZE);
		line.end = point_add(line.end, (t_point){offset.x * TILESIZE, offset.y * TILESIZE});
		//line.end = point_add(line.end, vector2_to_point(vector2_div(offset, TILESIZE)));
		// line start and end calculated incorrectly, needs to take offset into account
		drawline(sdl->surface->pixels, line.start, line.end, CLR_TURQ);
		node = node->next;
	}
}

void	render_overhead(t_game *game, t_sdlcontext *sdl)
{
	t_point		player_pos; //player coordinates (screenspace) in integer vector
	t_point		look_to; // line end coordinate that is towards player look direction
	t_vector2	walls_offset;

	look_to.x = sin(game->player.angle) * 40;
	look_to.y = cos(game->player.angle) * 40;
	if (game->cam_mode == overhead_absolute)
	{
		player_pos = *(t_point *)&game->player.position;
		//int temp[2];
		t_point	temp;
		temp = vector2_to_point(game->overheadcam_pos);
		//f2tov2(game->overheadcam_pos, temp);
		//v2add(player_pos, temp);
		player_pos = point_add(player_pos, temp);
		draw_walls(game->linelst, sdl, game->overheadcam_pos);
	}
	else if  (game->cam_mode == overhead_follow)
	{
		player_pos.x = WINDOW_W / 2;
		player_pos.y = WINDOW_H / 2;
		//f2add(walls_offset, game->player.position);
		
		/*walls_offset.x = -game->player.position.x;
		walls_offset.y = -game->player.position.y;
		walls_offset.x += player_pos.x;
		walls_offset.y += player_pos.y;*/

		walls_offset = vector2_negative(game->player.position);
		printf("offset %f %f \n", walls_offset.x, walls_offset.y);
		draw_walls(game->linelst, sdl, walls_offset);
	}
	//v2add(look_to, player_pos);
	look_to = point_add(look_to, player_pos);
	drawline(sdl->surface->pixels, player_pos, look_to, CLR_PRPL);
	if (game->cam_mode == overhead_follow)
		drawcircle(sdl->surface->pixels, player_pos, PLAYERRADIUS, CLR_PRPL);
	else
		drawcircle(sdl->surface->pixels, player_pos, PLAYERRADIUS, CLR_GRAY);
}