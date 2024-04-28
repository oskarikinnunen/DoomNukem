/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_random.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:11:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 17:17:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#define RAND_A 1664525
#define RAND_B 1013904223

uint32_t	game_random(t_world *world)
{
	static uint32_t	seed = 1234;

	seed = (RAND_A * seed) + RAND_B;
	return (seed);
}

uint32_t	game_random_range(t_world *world, uint32_t min, uint32_t max)
{
	uint32_t	r;
	uint32_t	result;

	r = game_random(world);
	if (max - min == 0)
		doomlog(LOG_FATAL, \
			"game_random_range max - min = 0 (can't modulo 0)");
	result = r % (max - min);
	result += min;
	return (result);
}
