/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_presets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 12:51:01 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 12:09:44 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "player.h"

t_gun_stats	gun_machinegun(void)
{
	t_gun_stats	stats;

	ft_bzero(&stats, sizeof(stats));
	ft_strncpy_term(stats.object_name, "grenadelauncher.obj", 120);
	ft_strncpy_term(stats.audio_name, "gun_machinegun1.wav", 120);
	stats.ammo_mask = AM_ASSAULT;
	stats.holster_pos = (t_vector3){11.5f, 7.90f, -10.9f};
	stats.angle_recovery = 0.0006f;
	stats.recoil_jump.y = -0.7f;
	stats.view_recoil.y = 0.0004f;
	stats.recoil_angle.y = 3.7f;
	stats.aim_pos = (t_vector3){0.0f, 3.0f, -4.7f};
	stats.fire_delay = 130;
	stats.ads_speed = 0.02f;
	stats.fov_offset = 0.0f;
	stats.damage = 17;
	stats.magazine_capacity = 25;
	stats.reload_time = 45;
	stats.full_auto = true;
	return (stats);
}

t_gun_stats	gun_sniper(void)
{
	t_gun_stats	stats;

	ft_bzero(&stats, sizeof(stats));
	ft_strncpy_term(stats.object_name, "welrod.obj", 120);
	ft_strncpy_term(stats.audio_name, "gun_sniper.wav", 120);
	stats.ammo_mask = AM_SNIPER;
	stats.holster_pos = (t_vector3){5.0f, 3.7f, -5.1f};
	stats.angle_recovery = 0.0002f;
	stats.recoil_jump.y = -0.2f;
	stats.view_recoil.y = 0.001f;
	stats.recoil_angle.y = 4.0f;
	stats.aim_pos = (t_vector3){0.0f, 1.0f, -0.9f};
	stats.fov_offset = -40.0f;
	stats.fire_delay = 1150;
	stats.damage = 100;
	stats.ads_speed = 0.005f;
	stats.magazine_capacity = 5;
	stats.reload_time = 25;
	stats.full_auto = false;
	return (stats);
}
