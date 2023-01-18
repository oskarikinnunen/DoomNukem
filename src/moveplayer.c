/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveplayer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:09:03 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/18 12:32:12 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
#include "bresenham.h"
#include "objects.h"
#include "entity.h"
#include "libft.h"
#include "editor_tools.h"
#include "movement_defs.h"
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

static t_vector3	normalized_inputvector(t_input input, t_player player)
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
	if (player.noclip)
	{
		movement.z += 1.5f * input.jump;
		movement.z -= 1.5f * input.crouch;
	}
	movement = vector3_mul(movement, 1.0f);
	movement = vector3_clamp_magnitude(movement, 1.0f);
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

t_vector3	vector3_movetowards2(t_vector3 vec, t_vector3 to, float delta)
{
	t_vector3	result;

	result.x = fmovetowards(vec.x, to.x, delta);
	result.y = fmovetowards(vec.y, to.y, delta);
	result.z = fmovetowards(vec.z, to.z, delta);
	return (result);
}



void	updateguntransform(t_input *input, t_clock *clock, t_player *player)
{
	static float	lerp;
	t_gun			*gun;
	t_vector3		neutralpos;

	if (player->gun == NULL || player->gun->disabled)
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
		/*
		spawn entity
		component_add_particle(entity);
		t_particle p = entity.component.data;
		p.starttime = clock.prevtime
		p.dir = 
		ft_bzero(&particleanim, sizeof(particleanim));
		particleanim.framerate = 8;
		particleanim.lastframe = 16;
		particleanim.mode = anim_forwards;
		start_anim(&particleanim, anim_forwards);
		*/
		start_anim(&gun->shoot_anim, anim_forwards);
		start_anim(&gun->view_anim, anim_forwards);
		//gun->entity.transform
		//TODO: add localposition and position
		//t_vector3 worldspace_position = transformed_vector3(gun->entity.transform, gun->entity.transform.position).v;

		//make_transform_matrix()
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

bool	touches_edges(t_vector2 pos, t_meshtri *floor)
{
	t_line		line1;
	t_line		line2;
	t_line		line3;
	t_collision	unused_col;

	line1.start = v3tov2(floor->v[0]);
	line1.end = v3tov2(floor->v[1]);
	line2.start = v3tov2(floor->v[1]);
	line2.end = v3tov2(floor->v[2]);
	line3.start = v3tov2(floor->v[2]);
	line3.end = v3tov2(floor->v[0]);
	return (linecircle(line1, pos, 15.0f, &unused_col)
			|| linecircle(line2, pos, 15.0f, &unused_col)
			|| linecircle(line3, pos, 15.0f, &unused_col));
}

bool	is_in_floor(t_vector3 pos, t_meshtri	*floor)
{
	t_ray r;
	r.origin = vector3_add(pos, (t_vector3){.z = 100.0f});
	r.dir = (t_vector3){.z = -1.0f};
	t_raycastinfo	info;
	info.distance = 10000.0f;
	if (raycast_entity(r, &info, floor->entity)
		|| touches_edges(v3tov2(pos), floor))
	{
		//floor->entity->obj->materials->img = get_image_by_name(*world->sdl, "ai_icon.cng");
		//return ((float)room->height);
		return (true);
	}
	return (false);
}

float	get_z_position(t_player *player, t_world *world)
{
	t_list	*list;
	t_room	*room;
	int		i;
	float	z;

	z = 0;

	list = world->roomlist;
	while(list != NULL)
	{
		room = (t_room *)list->content;
		i = 0;
		while (i < room->floorcount)
		{
			if (is_in_floor(player->transform.position, &room->floors[i]))
			{
				z = ft_maxf(z, (float)room->height);
				//return ((float)room->height);
			}
			room->floors[i].entity->obj->materials->img = get_image_by_name(*world->sdl, "concrete.cng");
			i++;
		}
		list = list->next;
	}
	return (z);
}

static void noclip_movement(t_player *player, t_vector3 move_vector, t_world *world)
{
	player->transform.position = vector3_add(player->transform.position, move_vector);
}

#define GRAVITY -0.8f

void	play_footstepsound(t_player *player, t_world *world)
{
	static t_audiosource source;

	if (source.sample.sound == NULL)
	{
		source.sample = get_sample(world->sdl, "concrete-footsteps_loop.wav");
		source._realrange = 50.0f;
		source.volume = 1.0f;
		source.play_always = true;
		_audiosource_start(world->sdl, &source, &player->transform.position);
	}
	float lerp = vector2_magnitude(v3tov2(player->velocity)) * 10.0f; //TODO: use player speed
	source.volume = lerp * 0.20f;
	if (!player->isgrounded)
	{
		source.volume = 0.0f;
		return ;
	}
	FMOD_Channel_SetPitch(source.channel, ft_clampf(lerp, 1.0f, 1.35f));
	FMOD_Channel_Set3DAttributes(source.channel,
			(FMOD_VECTOR *)&player->transform.position, &((FMOD_VECTOR){0}));
	FMOD_Channel_SetVolume(source.channel, source.volume * world->sdl->audio.sfx_volume);
}

void	play_landingsound(t_player *player, t_world *world)
{
	static t_audiosource source;
	float vlerp = player->velocity.z / (GRAVITY);
	source.channel = NULL;
	source.volume = ft_clampf(vlerp, 0.3f, 1.0f);
	source._realrange = ft_flerp(40.0f, 160.0f, vlerp);
	if (source.volume < 0.5f)
		source.sample = get_sample(world->sdl, "landing_impact_soft.wav");
	else
		source.sample = get_sample(world->sdl, "landing_impact_step.wav");
	_audiosource_start(world->sdl, &source, &player->transform.position);
	FMOD_BOOL	playing;
	FMOD_Channel_IsPlaying(source.channel, &playing);
}



static void collision_movement(t_player *player, t_vector3 move_vector, t_world *world) //TODO: this takes only 2D velocity vector
{
	t_vector3	potential_pos;
	t_vector3	new_pos;

	potential_pos = vector3_add(player->transform.position, v2tov3(v3tov2(move_vector)));
	new_pos = potential_pos;
	int i = 0;
	while (check_collision(world, player, new_pos, &new_pos) && i < 5)
		i++;
	if (!check_collision(world, player, new_pos, &new_pos))
		player->transform.position = new_pos;
	float floorz = get_z_position(player, world);
	//printf("floor z %f \n", floorz);
	if (floorz <= player->transform.position.z)
	{
		player->isgrounded = (player->transform.position.z <= floorz);
		float target_z = floorz;
		//float gravityapply = 0.5f;
		float gravityapply = 0.0012f;
		float zveltarget;
		zveltarget = GRAVITY;
		if (player->jump.active)
		{
			float lerp = 1.5f - player->jump.lerp;
			//zveltarget = GRAVITY + (lerp * 7.4f); //perfect rocket jump, lol
			zveltarget = GRAVITY + (lerp * 0.7f); //this one is a bit floaty
		}
		if (player->isgrounded)
			zveltarget = 0.0f;
		player->velocity.z = ft_fmovetowards(player->velocity.z, zveltarget, gravityapply * world->clock.delta);
		if (player->velocity.z <= 0.0f)
			target_z = floorz;
		else
			target_z = 10000.0f;
		player->transform.position.z = ft_fmovetowards(player->transform.position.z, target_z, ft_absf(player->velocity.z) * world->clock.delta);
		player->transform.position.z = ft_clampf(player->transform.position.z, floorz, 10000.0f);
		player->isgrounded = (player->transform.position.z <= floorz);
	}
	else
		player->transform.position.z = ft_fmovetowards(player->transform.position.z, floorz, world->clock.delta * -GRAVITY * 0.5f);
	
	if (player->isgrounded && player->velocity.z != 0)
	{
		play_landingsound(player, world);
		player->velocity.z = 0.0f;
	}
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
	move_vector = normalized_inputvector(*input, *player);
	t_vector2 velxy;
	velxy = vector2_add(v3tov2(player->velocity), vector2_mul(v3tov2(move_vector), PLAYER_ACCELERATION * world->clock.delta));
	player->velocity.x = velxy.x;
	player->velocity.y = velxy.y;
	//player->velocity = vector3_add(player->velocity, vector3_mul(move_vector, acc));
	float maxvel = PLAYER_WALKSPEED + (input->run * (PLAYER_RUNSPEED - PLAYER_WALKSPEED));
	t_vector2 clampxy;
	clampxy = vector2_clamp_magnitude(v3tov2(player->velocity), maxvel);
	player->velocity.x = clampxy.x;
	player->velocity.y = clampxy.y;
	if (player->noclip)
		noclip_movement(player, vector3_mul(move_vector, PLAYER_RUNSPEED * world->clock.delta), world);
	else
	{
		static bool has_crouchjumped;
		if (input->crouch)
		{
			if (!has_crouchjumped && !player->isgrounded && player->velocity.z >= 0)
			{
				player->velocity.z = 0.125f;
				has_crouchjumped = true;
			}
			player->height = ft_fmovetowards(player->height, PLAYER_CROUCHHEIGHT, PLAYER_CROUCHSPEED * world->clock.delta);
		}
		else
		{
			player->height = ft_fmovetowards(player->height, PLAYER_HEIGHT, PLAYER_CROUCHSPEED * world->clock.delta);
			has_crouchjumped = false;
		}
		if (input->jump && player->isgrounded && world->clock.prev_time > player->lastjumptime + JUMP_DELAY)
		{
			player->transform.position.z += 0.1f;
			player->velocity.z = 0.15f;
			start_anim(&player->jump, anim_forwards);
			player->lastjumptime = world->clock.prev_time;
		}
		if (player->jump.active)
			update_anim(&player->jump, world->clock.delta);
		collision_movement(player, vector3_mul(player->velocity, world->clock.delta), world);
		play_footstepsound(player, world);
	}
	t_vector2	damped;
	damped = v3tov2(vector3_mul(player->velocity, 1.0f - (world->clock.delta * PLAYER_DECELERATION)));
	player->velocity.x = damped.x;
	player->velocity.y = damped.y;
	player->headposition = vector3_add(player->transform.position, (t_vector3){.z = player->height * 0.75f});
	/*player->velocity.x = ft_fmovetowards(player->velocity.x, 0.0f, world->clock.delta * 0.001f);
	player->velocity.y = ft_fmovetowards(player->velocity.y, 0.0f, world->clock.delta * 0.001f);*/
}
