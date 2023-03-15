/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_presets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 12:51:01 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/15 16:24:35 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "player.h"

t_gunstats	gun_machinegun(void)
{
	t_gunstats	stats;

	ft_bzero(&stats, sizeof(stats));
	ft_strcpy(stats.object_name, "grenadelauncher.obj");
	ft_strcpy(stats.audio_name, "gun_machinegun1.wav");
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
	ft_strcpy(stats.object_name, "welrod.obj");
	ft_strcpy(stats.audio_name, "gun_sniper.wav");
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

void	change_gun_preset(t_gun *gun, t_sdlcontext *sdl, int index)
{
	static t_gunstats	(*preset_funcs[20])(void)
		= {
		gun_machinegun,
		gun_sniper,
		NULL
	};

	index = ft_clamp(index, 0, 1);
	gun->stats = (preset_funcs[index])();
	gun->entity->obj = get_object_by_name(*sdl, gun->stats.object_name);
}
