/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_transform.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 15:02:26 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:23:41 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
#include "entity.h"
#include "libft.h"
#include "editor_tools.h"
#include "raycast.h"

static void	gun_recoil_transform(t_player *player, t_gun *gun, t_clock c)
{
	gun->entity->transform.rotation.y
		= ft_fmovetowards(gun->entity->transform.rotation.y,
			ft_degtorad(player->input.move.y * 1.15f),
			gun->stats.angle_recovery * c.delta);
	if (gun->shoot_anim.active)
		gun->entity->transform.rotation.y
			+= ft_flerp(0.0f, ft_degtorad(gun->stats.recoil_angle.y),
				gun->shoot_anim.lerp);
	gun->entity->transform.rotation.y
		= ft_clampf(gun->entity->transform.rotation.y,
			ft_degtorad(-0.5f), ft_degtorad(15.0f));
	if (gun->view_anim.active)
		player->transform.rotation.y += gun->view_anim.lerp
			* c.delta * gun->stats.view_recoil.y;
}

static void	gun_calc_aim_lerp(t_player *player, t_gun *gun, t_clock c)
{
	if (player->input.aim)
		gun->aim_lerp += gun->stats.ads_speed * c.delta;
	else
		gun->aim_lerp -= (gun->stats.ads_speed * 1.25f) * c.delta;
	gun->aim_lerp = ft_clampf(gun->aim_lerp, 0.0f, 1.0f);
}

static void	gun_update_bobbing(t_player *player, t_gun *gun, t_clock c)
{
	if (gun->shoot_anim.active)
		gun->entity->transform.position
			= vector3_add(gun->entity->transform.position,
				vector3_mul(vector3_up(), gun->shoot_anim.lerp
					* gun->stats.recoil_jump.y));
	gun->entity->transform.position.z
		+= vector2_magnitude(player->input.move)
		* cosf((c.time * 0.007f)) * 0.05f;
	gun->entity->transform.position.z
		= ft_fmovetowards(gun->entity->transform.position.z,
			gun->entity->transform.position.z + player->cp.velocity.z,
			c.delta * 0.1f);
	gun->entity->transform.rotation.z
		+= vector2_magnitude(player->input.move)
		* cosf((c.time * 0.007f)) * ft_degtorad(0.15f);
}

static void	gun_update_turn(t_player *player, t_gun *gun, t_clock c)
{
	float	zturn;

	zturn = player->input.move.x * 2.0f;
	if (!player->input.aim)
		zturn += player->input.turn.x * 5.0f;
	zturn = ft_clampf(zturn, ft_degtorad(-2.5f), ft_degtorad(2.5f));
	gun->entity->transform.rotation.z
		= ft_fmovetowards(gun->entity->transform.rotation.z, zturn,
			0.0004f * c.delta);
}

void	gun_update_transform(t_player *player, t_gun *gun, t_clock c)
{
	t_vector3	neutralpos;

	gun->entity->transform.rotation.x = ft_degtorad(0.0f);
	gun_calc_aim_lerp(player, gun, c);
	player->fov = 90.0f + (gun->aim_lerp * gun->stats.fov_offset);
	neutralpos = vector3_lerp(gun->stats.holster_pos,
			gun->stats.aim_pos, gun->aim_lerp);
	gun->entity->transform.position = neutralpos;
	gun_recoil_transform(player, gun, c);
	gun_update_bobbing(player, gun, c);
	gun_update_turn(player, gun, c);
}
