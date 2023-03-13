
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

t_bound	get_bound(t_characterphysics *cp, t_world *world);

static void	crouchupdate(t_player *player, t_world *world)
{
	t_bound	bound;

	if (player->input.crouch)
	{
		if (!player->crouchjumped && !player->isgrounded && player->cp.velocity.z >= 0)
		{
			player->cp.velocity.z = 0.125f;
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
	if (world->clock.time > 1000)
	{
		bound = get_bound(&player->cp, world);
		player->height = ft_clampf(player->height, PLAYER_CROUCHHEIGHT, bound.max - player->cp.position->z);
	}
}

void	player_init_physics(t_player *player)
{
	float				lerp;
	static float		override;

	ft_bzero(&player->cp, sizeof(player->cp));
	player->cp.position = &player->transform.position;
	player->cp.radius = 15.0f;
	player->cp.gravity_override = NULL;
	//player->cp.height = player->height; Move to update
	//return (phys);
}


void	player_update_physics(t_player *player, t_world *world)
{
	float				lerp;
	static float		override;

	player->cp.position = &player->transform.position;
	player->cp.radius = 15.0f;
	player->cp.gravity_override = NULL;
	player->cp.height = player->height;
	player->cp.max_velocity = current_maxvelocity(player);
	if (player->jump.active)
	{
		lerp = 1.5f - player->jump.lerp;
		override = (GRAVITY + (lerp * 0.65f));
		player->cp.gravity_override = &override;
	}
	//player->cp.po
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
	float lerp = vector2_magnitude(v3tov2(player->cp.velocity)) * 10.0f; //TODO: use phys
	source.volume = lerp * 0.20f;
	if (!player->cp.isgrounded)
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
	
	if (player->cp.impactvelocity.z > 0.0f)
	{
		source.volume = 0.2f;
		source.sample = random_climbsound(world);
		source._realrange = ft_flerp(40.0f, 160.0f, vlerp);
		_audiosource_start(world->sdl, &source, &player->headposition);
	}
	else
	{
		float vlerp = player->cp.impactvelocity.z / (GRAVITY);
		source.volume = ft_clampf(vlerp, 0.2f, 0.4f);
		source._realrange = ft_flerp(40.0f, 160.0f, vlerp);
		if (vlerp < 0.38f)
			source.sample = get_sample(world->sdl, "landing_impact_soft.wav");
		else
			source.sample = get_sample(world->sdl, "landing_impact_hard.wav");
		_audiosource_start(world->sdl, &source, &player->transform.position);
	}
}

void	player_rotate(t_player *player)
{
	player->transform.rotation = vector3_sub(player->transform.rotation, v2tov3(player->input.turn));
	player->transform.rotation.y = ft_clampf(player->transform.rotation.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	player->lookdir = lookdirection((t_vector2){player->transform.rotation.x, player->transform.rotation.y});
}

void	player_move(t_player *player, t_world *world)
{
	t_vector2	velocity_xy;
	
	velocity_xy = vector2_mul(normalized_inputvector(player->input, *player), PLAYER_ACCELERATION * world->clock.delta);
	capsule_add_xy_velocity(velocity_xy, &player->cp, world);
}

void	player_jump(t_player *player, t_world *world)
{
	if (player->input.jump && player->cp.isgrounded && world->clock.time > player->lastjumptime + JUMP_DELAY)
	{
		player->cp.velocity.z = 0.065f;
		player->transform.position.z += 0.1f;
		start_anim(&player->jump, anim_forwards);
		player->lastjumptime = world->clock.time;
	}
	if (player->jump.active)
		update_anim(&player->jump, world->clock.delta);
}

void	player_update_animations(t_player *player, t_world *world)
{
	if (player->jump.active)
		update_anim(&player->jump, world->clock.delta);
}

void	player_ceiling_check(t_player *player)
{
	if (player->cp.ceilingtrigger)
	{
		player->jump.active = false;
		player->cp.ceilingtrigger = false;
	}
}

void	playermovement_normal(t_player *player, t_world *world)
{
	player_update_physics(player, world);
	player_rotate(player);
	player_move(player, world);
	crouchupdate(player, world);
	player_jump(player, world);
	player_update_animations(player, world);
	player_update_physics(player, world);
	capsule_applygravity_new(&player->cp, world);
	play_footstepsound(player, world);
	player_ceiling_check(player);
}
