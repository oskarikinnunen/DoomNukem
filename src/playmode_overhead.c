/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_overhead.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:46:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 21:50:21 by okinnune         ###   ########.fr       */
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

static void	draw_walls(t_list *linelist, t_img	img, t_vector2 offset)
{
	t_list	*node;
	t_line	line;

	node = linelist;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		line.start = point_mul(line.start, TILESIZE);
		line.start = point_add(line.start, (t_point){offset.x, offset.y});
		line.
		line.end = point_mul(line.end, TILESIZE);
		line.end = point_add(line.end, (t_point){offset.x, offset.y});
		drawline(img.data, line.start, line.end, CLR_TURQ);
		node = node->next;
	}
}

//TODO: add some kind of "camera position" abstraction, this just draws from (0,0) perspective and all the walls are shifted
//TODO: re-add player_ball draw for overhead_absolute camera mode
//TODO: player position in "overhead follow" is off by half a window size
void	render_overhead(t_game *game, t_sdlcontext *sdl)
{
	t_point			player_ball; //player coordinates  in screenspace
	t_point			look_to; // line end coordinate that is towards player look direction
	t_vector3		normalized_forward;
	t_vector2		walls_offset;
	static t_img	*image;
	static t_rectangle	map_rect =
	{
		//TODO: dis
	}

	if (image == NULL)
	{
		image = ft_memalloc(sizeof(t_img));
		alloc_image(image, WINDOW_W, WINDOW_H);
	}
	ft_bzero(image->data, sizeof(uint32_t) * image->length);
	player_ball.x = WINDOW_W / 2;
	player_ball.y = WINDOW_H / 2;
	walls_offset = vector2_negative((t_vector2){game->player.position.x, game->player.position.y});
	//walls_offset = vector2_sub(walls_offset, point_to_vector2(player_ball));
	walls_offset = vector2_add(walls_offset, (t_vector2){player_ball.x, player_ball.y});
	draw_walls(game->linelst, *image, walls_offset);
	normalized_forward = vector3_normalise(game->player.lookdir);
	look_to = (t_point) {normalized_forward.x * 20, normalized_forward.y * 20};
	look_to = point_add(look_to, player_ball);
	//drawline(sdl->surface->pixels, player_ball, look_to, CLR_PRPL);
	//	drawline(sdl->surface->pixels, player_ball, PLAYERRADIUS, CLR_PRPL);
	drawcircle(image->data, player_ball, 100, CLR_PRPL);
	draw_ingame_image(sdl->surface->pixels, point_zero(), *image, point_div(image->size, 8));
	drawrectangle(sdl->surface->pixels, (t_rectangle){point_zero(), point_div(image->size, 8)}, CLR_GRAY);
}