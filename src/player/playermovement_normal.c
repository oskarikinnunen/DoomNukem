
#include "doomnukem.h"
#include "movement_defs.h"

static t_vector2	normalized_inputvector(t_input input, t_player player)
{
	t_vector2	movement;
	t_vector3	forward;
	float		speed;

	movement = vector2_zero();
	forward = player.lookdir;
	forward.z = 0.0f;
	forward = vector3_normalise(forward);
	movement = vector2_mul(v3tov2(forward), -input.move.y);
	t_vector3 right = vector3_crossproduct(forward, vector3_up());
	t_vector3 strafe = vector3_mul(right, input.move.x);
	movement = vector2_add(movement, v3tov2(strafe));
	//movement = vector3_mul(movement, 1.0f);
	movement = vector2_clamp_magnitude(movement, 1.0f); //Same as normalise?
	return (movement);
}

static float	current_maxvelocity(t_player *player)
{
	if (!player->isgrounded)
		return (PLAYER_WALKSPEED * 1.35f + (player->input.run * (PLAYER_RUNSPEED - PLAYER_WALKSPEED)));
	return (PLAYER_WALKSPEED + (player->input.run * (PLAYER_RUNSPEED - PLAYER_WALKSPEED)));
}

static void	crouchupdate(t_player *player, t_world *world)
{
	if (player->input.crouch)
	{
		if (!player->crouchjumped && !player->isgrounded && player->velocity.z >= 0)
		{
			player->velocity.z = 0.125f;
			player->crouchjumped = true;
		}
		player->height = ft_fmovetowards(player->height, PLAYER_CROUCHHEIGHT, PLAYER_CROUCHSPEED * world->clock.delta);
	}
	else
	{
		player->height = ft_fmovetowards(player->height, PLAYER_HEIGHT, PLAYER_CROUCHSPEED * world->clock.delta);
		if (player->isgrounded)
			player->crouchjumped = false;
	}
}

t_characterphysics	player_physics(t_player *player)
{
	t_characterphysics	phys;
	float				lerp;
	static float		override;

	ft_bzero(&phys, sizeof(phys));
	phys.position = &player->transform.position;
	phys.velocity = &player->velocity;
	phys.impactvelocity = &player->impactvelocity;
	phys.radius = 15.0f;
	phys.isgrounded = &player->isgrounded;
	phys.gravity_override = NULL;
	phys.height = player->height;
	phys.landingtrigger = &player->landingtrigger;
	if (player->jump.active)
	{
		float lerp = 1.5f - player->jump.lerp;
		override = (GRAVITY + (lerp * 0.66f));
		phys.gravity_override = &override;
	}
	return (phys);
}

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

//between 0-3000 because game_random_range works less randomly with 0-3
t_audiosample random_climbsound(t_world *world)
{
	uint32_t r;
	
	r = game_random_range(world, 0, 3000);
	if (r < 1000)
		return (get_sample(world->sdl, "climb1.wav"));
	if (r > 1000 && r < 2000)
	{
		return (get_sample(world->sdl, "climb2.wav"));
	}
	return (get_sample(world->sdl, "climb3.wav"));
}

void	play_landingsound(t_player *player, t_world *world)
{
	static	t_audiosource source;
	float	vlerp;
	
	if (player->impactvelocity.z > 0.0f)
	{
		source.volume = 0.2f;
		source.sample = random_climbsound(world);
		source._realrange = ft_flerp(40.0f, 160.0f, vlerp);
		_audiosource_start(world->sdl, &source, &player->headposition);
	}
	else
	{
		float vlerp = player->impactvelocity.z / (GRAVITY);
		source.volume = ft_clampf(vlerp, 0.2f, 0.4f);
		source._realrange = ft_flerp(40.0f, 160.0f, vlerp);
		if (vlerp < 0.38f)
			source.sample = get_sample(world->sdl, "landing_impact_soft.wav");
		else
			source.sample = get_sample(world->sdl, "landing_impact_hard.wav");
		_audiosource_start(world->sdl, &source, &player->transform.position);
	}
}

void	playermovement_normal(t_player *player, t_world *world)
{
	t_vector2	velocity_xy;
	t_vector2	add_velocity;

	player->transform.rotation = vector3_sub(player->transform.rotation, v2tov3(player->input.turn));
	player->transform.rotation.y = ft_clampf(player->transform.rotation.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	player->lookdir = lookdirection((t_vector2){player->transform.rotation.x, player->transform.rotation.y});
	//TODO: split to addvelocity
	velocity_xy = v3tov2(player->velocity);
	float acc = PLAYER_ACCELERATION;
	if (!player->isgrounded)
		acc *= 0.25f;
	add_velocity = vector2_mul(normalized_inputvector(player->input, *player), PLAYER_ACCELERATION * world->clock.delta);
	velocity_xy = vector2_add(velocity_xy, add_velocity);
	player->velocity.x = velocity_xy.x;
	player->velocity.y = velocity_xy.y;
	velocity_xy = vector2_clamp_magnitude(v3tov2(player->velocity), current_maxvelocity(player));
	player->velocity.x = velocity_xy.x;
	player->velocity.y = velocity_xy.y;
	crouchupdate(player, world);
	if (player->input.jump && player->isgrounded && world->clock.prev_time > player->lastjumptime + JUMP_DELAY)
	{
		if (vector2_magnitude(velocity_xy) > PLAYER_WALKSPEED)
			player->velocity.z = 0.12f;
		else
		{
			add_velocity = vector2_mul(normalized_inputvector(player->input, *player), 0.005f * world->clock.delta);
			player->velocity.x += add_velocity.x;
			player->velocity.y += add_velocity.y;
			player->velocity.z = 0.065f;
		}
			
		player->transform.position.z += 0.1f;
		start_anim(&player->jump, anim_forwards);
		player->lastjumptime = world->clock.prev_time;
	}
	if (player->jump.active)
		update_anim(&player->jump, world->clock.delta);
	//collision_movement(player, vector3_mul(player->velocity, world->clock.delta), world);
	apply_capsulephysics(player_physics(player), world);
	player->speed = vector3_magnitude(player->velocity);
	if (player->landingtrigger && (player->landingtrigger = false, 1))
		play_landingsound(player, world);
	play_footstepsound(player, world);
	//t_vector2	damped;
	//TODO: split to damp_player
	velocity_xy = v3tov2(player->velocity);
	velocity_xy = vector2_mul(velocity_xy, 1.0f - (world->clock.delta * PLAYER_DECELERATION));
	//damped = v3tov2(vector3_mul(player->velocity, 1.0f - (world->clock.delta * PLAYER_DECELERATION)));
	if (player->isgrounded)
	{
		player->velocity.x = velocity_xy.x;
		player->velocity.y = velocity_xy.y;
	}
	
}