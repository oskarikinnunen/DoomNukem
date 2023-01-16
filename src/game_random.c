
#include "doomnukem.h"

uint32_t	game_random(t_world *world)
{
	static uint32_t r = INT_MAX / 2;

	r += INT_MAX / 4;
	r = r << (int)(world->clock.prev_time);
	return (r);
}

uint32_t	game_random_range(t_world *world, uint32_t min, uint32_t max)
{
	uint32_t	r;
	uint32_t	result;

	r = game_random(world);
	result = r % (max - min);
	result += min;
	return (result);
}