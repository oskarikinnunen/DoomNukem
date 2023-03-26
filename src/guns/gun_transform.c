/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_transform.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 15:02:26 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 15:05:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
#include "entity.h"
#include "libft.h"
#include "editor_tools.h"
#include "raycast.h"

void	play_gun_audio(t_gun *gun, t_world *world)
{
	static t_audiosource	source;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "gun_grenadelauncher.wav");
		source._realrange = 50.0f;
		source.volume = 1.0f;
		source.play_always = false;
	}
	source.sample = get_sample(world->sdl, gun->stats.audio_name);
	_audiosource_2d_start(world->sdl, &source);
}

void	play_gun_reload_audio(t_world *world)
{
	static t_audiosource	source;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "gun_reload.wav");
		source._realrange = 50.0f;
		source.volume = 1.0f;
		source.play_always = false;
	}
	_audiosource_2d_start(world->sdl, &source);
}

void	gun_update_transform(t_player *player, t_gun *gun, t_clock c)
{
	float		lerp;
	t_vector3	neutralpos;

	gun->entity->transform.rotation.x = ft_degtorad(0.0f);
	if (player->input.aim)
		gun->aim_lerp += gun->stats.ads_speed * c.delta;
	else
		gun->aim_lerp -= (gun->stats.ads_speed * 1.25f) * c.delta;
	gun->aim_lerp = ft_clampf(gun->aim_lerp, 0.0f, 1.0f);
	lerp = gun->aim_lerp;
	player->fov = 90.0f + (gun->aim_lerp * gun->stats.fov_offset);
	neutralpos = vector3_lerp(gun->stats.holsterpos, gun->stats.aimpos, lerp);
	gun->entity->transform.position = neutralpos;
	if (gun->shoot_anim.active)
		gun->entity->transform.position = vector3_add(gun->entity->transform.position, vector3_mul(vector3_up(), gun->shoot_anim.lerp * gun->stats.recoiljump.y));
	//bobbing:
	gun->entity->transform.position.z += vector2_magnitude(player->input.move) * cosf((c.time * 0.007f)) * 0.05f;
	gun->entity->transform.position.z = ft_fmovetowards(gun->entity->transform.position.z, gun->entity->transform.position.z + player->cp.velocity.z, c.delta * 0.1f);
	gun->entity->transform.rotation.z += vector2_magnitude(player->input.move) * cosf((c.time * 0.007f)) * ft_degtorad(0.15f);
	//recoilrecovery:
	gun->entity->transform.rotation.y = ft_fmovetowards(gun->entity->transform.rotation.y, ft_degtorad(player->input.move.y * 1.15f), gun->stats.anglerecovery * c.delta);
	//recoil.y:
	if (gun->shoot_anim.active)
		gun->entity->transform.rotation.y += ft_flerp(0.0f, ft_degtorad(gun->stats.recoilangle.y), gun->shoot_anim.lerp);
	gun->entity->transform.rotation.y = ft_clampf(gun->entity->transform.rotation.y, ft_degtorad(-0.5f), ft_degtorad(15.0f));
	//gun->entity->transform.rotation.x = ft_flerp(0.0f, ft_degtorad(2.0f), gun->shoot_anim.lerp);
	//Slight angle when player moves:
	float	zturn = player->input.move.x * 2.0f;
	if (!player->input.aim)
		zturn += player->input.turn.x * 5.0f;

	zturn = ft_clampf(zturn, ft_degtorad(-2.5f), ft_degtorad(2.5f));
	if (player->locked)
		return ;
	gun->entity->transform.rotation.z = ft_fmovetowards(gun->entity->transform.rotation.z, zturn, 0.0004f * c.delta);
	if (gun->view_anim.active)
		player->transform.rotation.y += gun->view_anim.lerp * c.delta * gun->stats.viewrecoil.y;
}