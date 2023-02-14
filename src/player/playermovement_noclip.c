
#include "doomnukem.h"
#include "movement_defs.h"

static t_vector3 sanitize_z(t_vector3 vec) //TODO: move to vectors
{
	return ((t_vector3){vec.x, vec.y, 0.0f});
}

static t_vector3	normalized_inputvector(t_input input, t_player player)
{
	t_vector3	movement;
	t_vector3	forward;
	float		speed;

	movement = vector3_zero();
	forward = player.lookdir;
	forward.z = 0.0f;
	forward = vector3_normalise(forward);
	movement = vector3_mul(sanitize_z(forward), -input.move.y);
	t_vector3 right = vector3_crossproduct(forward, vector3_up());
	t_vector3 strafe = vector3_mul(right, input.move.x);
	movement = vector3_add(movement, strafe);
	movement = vector3_clamp_magnitude(movement, 1.0f); //Same as normalise?
	movement.z = (float)input.jump;
	movement.z -= (float)input.crouch;
	return (movement);
}

void	playermovement_noclip(t_player *player, t_world *world)
{
	t_vector2		velocity_xy;
	t_vector2		add_velocity;
	static bool		toggler;
	static bool		run_toggle;
	static float	speed;

	player->transform.rotation = vector3_sub(player->transform.rotation, v2tov3(player->input.turn));
	player->transform.rotation.y = ft_clampf(player->transform.rotation.y, -RAD90 * 0.99f, RAD90 * 0.99f);
	player->lookdir = lookdirection((t_vector2){player->transform.rotation.x, player->transform.rotation.y});
	t_vector3 move_vec = normalized_inputvector(player->input, *player);
	if (player->input.run && !toggler)
	{
		run_toggle = !run_toggle;
		toggler = true;
		printf("run_toggle %i \n", run_toggle);
	}
	if (!player->input.run)
		toggler = false;
	speed = EDITOR_FLYSPEED + (run_toggle * (EDITOR_FLYFASTSPEED - EDITOR_FLYSPEED));
	move_vec = vector3_mul(move_vec, speed * world->clock.delta);
	player->transform.position = vector3_add(player->transform.position, move_vec);
}