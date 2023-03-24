/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_presets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 12:51:01 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 14:39:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "player.h"

t_gunstats	gun_machinegun(void)
{
	t_gunstats	stats;

	ft_bzero(&stats, sizeof(stats));
	ft_strncpy_term(stats.object_name, "grenadelauncher.obj", 120);
	ft_strncpy_term(stats.audio_name, "gun_machinegun1.wav", 120);
	stats.ammomask = AM_ASSAULT;
	stats.holsterpos = (t_vector3){11.5f, 7.90f, -10.9f};
	stats.anglerecovery = 0.0006f;
	stats.recoiljump.y = -0.7f;
	stats.viewrecoil.y = 0.0004f;
	stats.recoilangle.y = 3.7f;
	stats.aimpos = (t_vector3){0.0f, 3.0f, -4.7f};
	stats.firedelay = 150;
	stats.ads_speed = 0.02f;
	stats.fov_offset = 0.0f;
	stats.damage = 15;
	stats.magazinecapacity = 25;
	stats.reloadtime = 45;
	stats.fullauto = true;
	return (stats);
}

t_gunstats	gun_sniper(void)
{
	t_gunstats	stats;

	ft_bzero(&stats, sizeof(stats));
	ft_strncpy_term(stats.object_name, "welrod.obj", 120);
	ft_strncpy_term(stats.audio_name, "gun_sniper.wav", 120);
	stats.ammomask = AM_SNIPER;
	stats.holsterpos = (t_vector3){5.0f, 3.7f, -5.1f};
	stats.anglerecovery = 0.0002f;
	stats.recoiljump.y = -0.2f;
	stats.viewrecoil.y = 0.001f;
	stats.recoilangle.y = 4.0f;
	stats.aimpos = (t_vector3){0.0f, 1.0f, -0.9f};
	stats.fov_offset = -40.0f;
	stats.firedelay = 1150;
	stats.damage = 100;
	stats.ads_speed = 0.005f;
	stats.magazinecapacity = 5;
	stats.reloadtime = 25;
	stats.fullauto = false;
	return (stats);
}
