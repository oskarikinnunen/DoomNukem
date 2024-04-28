/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_death.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:32:29 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 13:19:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "player.h"
#include "movement_defs.h"

void	playermovement_death(t_player *player,
							t_world *world, uint32_t deathtime)
{
	t_vector2		velocity_xy;
	t_vector2		add_velocity;

	if (world->clock.time < deathtime + DEATH_RETRY_DELAY)
	{
		player->input.turn.x = 0.0008f * world->clock.delta;
		player->input.turn.y = 0.0016f * world->clock.delta;
		player->transform.position = vector3_add(player->transform.position,
				(t_vector3){.z = 0.03f * world->clock.delta});
	}
	else
		player->input.turn.x = 0.0003f * world->clock.delta;
	player->transform.rotation = vector3_sub(player->transform.rotation,
			v2tov3(player->input.turn));
	player->transform.rotation.y = ft_clampf(player->transform.rotation.y,
			-RAD90 * 0.99f, RAD90 * 0.99f);
	player->look_dir = look_direction(v3tov2(player->transform.rotation));
	player->head_position = vector3_add(player->transform.position,
			(t_vector3){.z = player->height * 0.75f});
	player->head_transform.position = player->head_position;
	player->head_transform.rotation = player->transform.rotation;
	player->head_transform.scale = player->transform.scale;
}

void	death_hud(t_world *world, uint32_t deathtime)
{
	t_point		pos;
	t_rectangle	rect;

	rect.position = point_zero();
	rect.size = world->sdl->screensize;
	pos = point_div(world->sdl->screensize, 2);
	if (world->clock.time > deathtime + DEATH_RETRY_DELAY)
	{
		draw_rectangle_raster_few(*world->sdl, rect, AMBER_2);
		print_text_boxed(world->sdl, "[R]etry?",
			(t_point){pos.x - 30, pos.y + 40});
	}
	else
		draw_rectangle_raster_few(*world->sdl, rect, CLR_RED);
	print_text_boxed(world->sdl, "YOU DIED", (t_point){pos.x - 30, pos.y});
}

static t_entity	*spawn_playerdeathmodel(t_world *world)
{
	t_entity	*pl_ent;

	pl_ent = spawn_entity(world, get_object_by_name(*world->sdl, "Human.obj"));
	pl_ent->transform = world->player->transform;
	pl_ent->transform.rotation.x -= RAD90;
	pl_ent->transform.rotation.y = 0.0f;
	pl_ent->transform.rotation.z = 0.0f;
	start_human_anim(pl_ent, "Death3", world);
	pl_ent->animation.frame_rate /= 5;
	world->player->gun->entity->hidden = true;
	protagonist_play_audio(world->player, world, "protag_death.wav");
	return (pl_ent);
}

static inline void	show_surface(t_sdlcontext *sdl)
{
	ft_memcpy(sdl->window_surface->pixels, sdl->surface->pixels,
		sizeof(uint32_t) * sdl->window_w * sdl->window_h);
	if (SDL_UpdateWindowSurface(sdl->window) < 0)
		doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
}

void	playmode_death(t_game *game)
{
	static uint32_t	deathtime;
	t_entity		*deathmodel;
	bool			cont;

	deathmodel = spawn_playerdeathmodel(&game->world);
	deathtime = game->world.clock.time;
	cont = false;
	while (!cont)
	{
		update_clock(&game->world.clock);
		update_anim(&deathmodel->animation, game->world.clock.delta);
		playmode_events(game);
		playermovement_death(&game->player, &game->world, deathtime);
		update_world3d(&game->world, &game->world.sdl->render);
		death_hud(&game->world, deathtime);
		if (game->world.clock.time > deathtime + DEATH_RETRY_DELAY
			&& game->hid.input.reload)
		{
			cont = true;
			destroy_entity(&game->world, deathmodel);
			respawn_player(game);
		}
		show_surface(game->world.sdl);
		update_audio(&game->world);
	}
}
