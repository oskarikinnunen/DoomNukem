
#include "doomnukem.h"

#define RAND_A 1664525
#define RAND_B 1013904223

uint32_t	game_random(t_world *world)
{
	static uint32_t seed = 1234;

	seed = (RAND_A * seed) + RAND_B;
	return (seed);
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

bool		game_random_coinflip(t_world *world)
{
	uint32_t	r;
	r = game_random_range(world, 0, 10000);
	return (r % 2 == 0);
}