/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 22:31:17 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 23:08:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

#define HUD1_X 20
#define HUD1_Y 40

static void	draw_ammo_hud_box(t_point pos_ammo, t_sdlcontext *sdl)
{
	t_rectangle	rect_ammo;

	rect_ammo.position = point_sub(pos_ammo, (t_point){3, 2});
	rect_ammo.size = (t_point){70, 28};
	draw_rectangle_filled(*sdl, rect_ammo, 0);
	draw_rectangle(*sdl, rect_ammo, AMBER_1);
}

static t_point	draw_ammo_hud(t_world *world)
{
	t_point		pos_ammo;
	t_point		origin;
	uint8_t		ammocount;

	origin = (t_point){HUD1_X, world->sdl->screensize.y - HUD1_Y};
	pos_ammo = origin;
	world->sdl->font_default->color = color32_to_sdlcolor(AMBER_2);
	print_text(world->sdl, "Ammo:", pos_ammo);
	pos_ammo.y += 14;
	if (world->player->gun->reload_anim.active)
		print_text(world->sdl, "R", pos_ammo);
	else
		print_text(world->sdl, s_itoa(world->player->gun->bullets), pos_ammo);
	pos_ammo.x += 20;
	print_text(world->sdl, "/", pos_ammo);
	pos_ammo.x += 10;
	ammocount = world->player->ammo_arr[world->player->gun->stats.ammo_mask];
	print_text(world->sdl, s_itoa(ammocount), pos_ammo);
	return (origin);
}

static void	draw_reload_hud(t_world *world)
{
	t_point		pos_reload;
	t_rectangle	rect_reload;

	pos_reload = screenlerp(0.75f, 0.75f, world->sdl);
	if (world->player->input.aim)
		pos_reload = screenlerp(0.55f, 0.5f, world->sdl);
	if (world->player->gun->bullets == 0)
	{
		if (world->player->input.shoot && sin(world->clock.time * 0.0185f)
			< 0.0f)
			world->sdl->font_default->color = color32_to_sdlcolor(CLR_RED);
		print_text_boxed(world->sdl, "![R]eload", pos_reload);
		world->sdl->font_default->color = color32_to_sdlcolor(AMBER_2);
	}
}

static void	draw_health_hud(t_world *world)
{
	t_rectangle	rect_health;
	t_rectangle	rect_hurt;

	rect_health.size = (t_point){100, 15};
	rect_health.position = point_add((t_point){HUD1_X,
			world->sdl->screensize.y - HUD1_Y}, (t_point){60, 20});
	draw_rectangle_filled(*world->sdl, rect_health, 0);
	draw_rectangle(*world->sdl, rect_health, AMBER_1);
	rect_health.size.x = ft_max(0, ((float)world->player->health
				/ (float)MAXHEALTH) * 100);
	draw_rectangle_filled(*world->sdl, rect_health, CLR_RED);
	if (world->clock.time < world->player->last_hurt_time + 1000)
	{
		rect_hurt.position.x = 20;
		rect_hurt.position.y = (int)(world->sdl->screensize.y * 0.75f);
		rect_hurt.size.x = world->sdl->screensize.x - 40;
		rect_hurt.size.y = (int)(world->sdl->screensize.y * 0.25f);
		draw_rectangle_raster_few(*world->sdl, rect_hurt, CLR_RED);
	}
}

void	draw_player_hud(t_world *world)
{
	t_point	origin;

	if (world->player->gun->player_owned)
	{
		origin = draw_ammo_hud(world);
		draw_ammo_hud_box(origin, world->sdl);
		origin = draw_ammo_hud(world);
		draw_reload_hud(world);
	}
	draw_health_hud(world);
	if (world->clock.time < world->player->gui_notif.starttime + 5000)
	{
		if (ft_strlen(world->player->gui_notif.str.str) > 0)
			print_text_boxed(world->sdl, world->player->gui_notif.str.str,
				point_sub(world->sdl->screensize, (t_point){200, 20}));
	}
}
