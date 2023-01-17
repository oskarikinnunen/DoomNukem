/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/17 14:20:16 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
 
#include "bresenham.h"
#include "objects.h"
#include "entity.h"
#include "libft.h"

#include <xmmintrin.h> // SSE

//#include <glm/gtx/simd_vec4.hpp>
//#include <pmmintrin.h> // SSE3
//#include <nmmintrin.h> // SSE4.2
//#include <immintrin.h> // AVX

/* Previous working version before implementing updateinput */
/*
static t_vector3	player_movementvector(int32_t keystate, t_vector3 lookdir)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(player->lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	if ((keystate >> KEYS_UPMASK) & 1) 
		movement = vector3_add(movement, forward);
	if ((keystate >> KEYS_DOWNMASK) & 1)
		movement = vector3_sub(movement, forward);
	if ((keystate >> KEYS_LEFTMASK) & 1)
		movement = vector3_sub(movement,
			vector3_crossproduct(forward, vector3_up()));
	if ((keystate >> KEYS_RIGHTMASK) & 1)
		movement = vector3_add(movement,
			vector3_crossproduct(forward, vector3_up()));
	speed = 1.0f + (float)((keystate >> KEYS_SHIFTMASK) & 1);
	if ((keystate >> KEYS_CTRLMASK) & 1)
	{
		speed *= 0.5f;
		player->height = 30.0f;
	}
	else
		player->height = 60.0f;
		
	if (player->jump.active)
		movement.z += cos(player->jump.lerp * 4.5f) * 1.8f;
	else if (player->position.z > player->height && movement.z >= 0.0f)
		movement.z -= 4.0f;
	else if ((keystate >> KEYS_SPACEMASK) & 1 && !player->jump.active)
		start_anim(&player->jump, anim_forwards);
	movement = vector3_mul_vector3(movement, (t_vector3){speed, speed, 1.0f});
	movement = vector3_clamp_magnitude(movement, speed);
	return (movement);

*/

static t_vector3	player_movementvector(t_input input, t_player player)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	//forward.z = 0;
	forward = vector3_mul_vector3(player.lookdir, (t_vector3){1.0f, 1.0f, 0.0f});
	forward = vector3_normalise(forward);
	movement = vector3_mul(forward, -input.move.y);
	t_vector3 right = vector3_crossproduct(forward, vector3_up());
	t_vector3 strafe = vector3_mul(right, input.move.x);
	movement = vector3_add(movement, strafe);
	speed = 1.0f + input.run;
	movement.z += 1.5f * input.jump;
	movement.z -= 1.5f * input.crouch;
	movement = vector3_mul(movement, speed);
	movement = vector3_clamp_magnitude(movement, speed);
	return (movement);
}

float	fmovetowards(float f1, float f2, float delta)
{
	float	result;

	result = f1;
	if (f1 < f2)
	{
		result += delta;
		result = ft_clampf(result, f1, f2);
	}
	else
	{
		result -= delta;
		result = ft_clampf(result, f2, f1);
	}
	return (result);
}

void	updateguntransform(t_input *input, t_clock *clock, t_player *player)
{
	static float	lerp;
	t_gun			*gun;
	t_vector3		neutralpos;

	if (player->gun->disabled)
		return ;
	gun = player->gun;
	if (input->aim)
		lerp += 0.02f * clock->delta;
	else
		lerp -= 0.02f * clock->delta;
	lerp = ft_clampf(lerp, 0.0f, 1.0f);
	neutralpos = vector3_lerp(gun->holsterpos, gun->aimpos, lerp);
	gun->entity.transform.position = neutralpos;
	//neutralpos = gun->entity.transform.location;
	if (input->shoot && gun->readytoshoot)
	{
		/*
		t_particle shootparticle;
		particle.sprite = fire.png;
		particle.lifetime = 30;
		spawn_particle(t_world *world, particle)
			spawn_entity
			assigns object
			world.particlecount++;
			world.particles[particlecount] = new particle
			
		
		
		*/
		//particle_start(gunpos, player.)
		start_anim(&gun->shoot_anim, anim_forwards);
		start_anim(&gun->view_anim, anim_forwards);
		
		gun->readytoshoot = false;
		gun->lastshottime = clock->prev_time;
	}
	else if (!input->shoot && !gun->shoot_anim.active && clock->prev_time > gun->lastshottime + gun->firedelay)
		gun->readytoshoot = true;
	if (gun->fullauto && clock->prev_time > gun->lastshottime + gun->firedelay)
		gun->readytoshoot = true;
	update_anim(&gun->shoot_anim, clock->delta);
	update_anim(&gun->view_anim, clock->delta);
	//recoil.y
	gun->entity.transform.position = vector3_add(gun->entity.transform.position, vector3_mul(vector3_up(), gun->shoot_anim.lerp * gun->recoiljump.y));
	//gun->entity.transform.location = vector3_movetowards
	//gun->entity.transform.location.z = fmovetowards(neutralpos.z)
	//gun->entity.transform.location = vector3_add(gun->entity.transform.location, vector3_mul((t_vector3){.x = 1.0f}, gun->shoot_anim.lerp * -0.25f));
	//bobbing:
	gun->entity.transform.position.z += vector2_magnitude(input->move) * cosf((clock->prev_time * 0.007f)) * 0.2f;
	gun->entity.transform.rotation.z += vector2_magnitude(input->move) * cosf((clock->prev_time * 0.007f)) * ft_degtorad(0.15f);
	//recoilrecovery
	gun->entity.transform.rotation.y = fmovetowards(gun->entity.transform.rotation.y, ft_degtorad(input->move.y * 1.15f), gun->anglerecovery * clock->delta);
	//recoil.y
	gun->entity.transform.rotation.y += ft_flerp(0.0f, ft_degtorad(gun->recoilangle.y), gun->shoot_anim.lerp);
	gun->entity.transform.rotation.y = ft_clampf(gun->entity.transform.rotation.y, ft_degtorad(-0.5f), ft_degtorad(15.0f));
	gun->entity.transform.rotation.x = ft_flerp(0.0f, ft_degtorad(2.0f), gun->shoot_anim.lerp);
	float	zturn = input->move.x * 2.0f;
	zturn += input->turn.x * 5.0f;
	zturn = ft_clampf(zturn, ft_degtorad(-2.5f), ft_degtorad(2.5f));
	if (player->locked)
		return ;
	gun->entity.transform.rotation.z = fmovetowards(gun->entity.transform.rotation.z, zturn, 0.0004f * clock->delta);
	player->transform.rotation.y += gun->view_anim.lerp * clock->delta * gun->viewrecoil.y; //Separate view jump animation that is longer than gun jump animation?
}

/*
	boxcollider
		objects, this is line and z collider combined
	linecollider
		walls
	zcollider
		floors/ceiling
*/

/*
	collision pseudocode

	vector3 curpos;
	newplayerpos = curpos + movevector;
	newplayerpos = get_new_pos_from_collision(player, newplayerpos, world); //Checks line collision and clamps position
																			//to the interpolated position between
																			//curpos and newplayerpos (aka it does the glide thingy)
	if (!collision(player, newplayerpos, world) //check collision again,
	{											// exactly like get_new_pos_from_collision but just return true/false if we hit a line
												//, so if the glide position still collides, we don't move the player at all
		curpos = newplayerpos;
	}

*/

//get_z_position(t_world *world, t_vector3 pos, float radius, float height)
//{
	/*
	z = 0.0f;

	while roomfloors
		if pointintriangle || collides with floor edgelines
		 && share_z_space(playercollider, floor)
			min_z = ft_maxf(z, room.height)
	while roomramps
		if (pointintriangle || collides with ramp edgelines)
		{
			get point in ramp (basicly uv step, maybe just raycast?)
			if (share_z_space(ramp, point.z))
				min_z = ft_maxf(z, point.z)
		}
	while boxcolliders
		while col.triangles
		if pointintriangle || collides with floor edgelines
		 && share_z_space(playercollider, floor)
			min_z = ft_maxf(z, room.height)
	
	*/
//}

static void noclip_movement(t_player *player, t_vector3 move_vector, t_world *world)
{
	player->transform.position = vector3_add(player->transform.position, move_vector);
}

static void collision_movement(t_player *player, t_vector3 move_vector, t_world *world)
{
	t_vector3	potential_pos;
	t_vector3	new_pos;

	potential_pos = vector3_add(player->transform.position, move_vector);
	potential_pos.z = ft_clampf(potential_pos.z, player->height, 1000.0f);
	new_pos = potential_pos;
	if (!check_collision(world, player, new_pos, &new_pos))
		player->transform.position = new_pos;
}

void	moveplayer(t_player *player, t_input *input, t_world *world)
{
	t_vector3	move_vector;
	float		angle;

	updateguntransform(input, &world->clock, player);
	if (player->locked)
		return ;
	move_vector = vector3_zero();
	//t_vector2 delta_angle = vector2_mul(input->turn, world->clock.delta);
	t_vector2 delta_angle = input->turn; //TODO: take world->clock delta into account only for joystick
	player->transform.rotation = vector3_sub(player->transform.rotation, (t_vector3){delta_angle.x, delta_angle.y, 0.0f});
	player->transform.rotation.y = ft_clampf(player->transform.rotation.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	player->lookdir = lookdirection((t_vector2){player->transform.rotation.x, player->transform.rotation.y});
	move_vector = player_movementvector(*input, *player);
	move_vector = vector3_mul(move_vector, world->clock.delta * MOVESPEED);
	player->speed = move_vector;
	if (player->noclip)
		noclip_movement(player, move_vector, world);
	else
		collision_movement(player, move_vector, world);
}
