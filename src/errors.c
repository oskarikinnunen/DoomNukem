/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:53:51 by raho              #+#    #+#             */
/*   Updated: 2022/10/12 15:45:39 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	errors(uint32_t error_code)
{
	if (error_code == 1)
	{
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		exit (1);
	}
	if (error_code == 2)
	{
		fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
		exit (1);
	}
	if (error_code == 3)
	{
		fprintf(stderr, "SDL_GetWindowSurface failed: %s\n", SDL_GetError());
		exit (1);
	}
	if (error_code == 4)
	{
		fprintf(stderr, "Malloc failed\n");
		exit (2);
	}
}
