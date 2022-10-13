/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:53:51 by raho              #+#    #+#             */
/*   Updated: 2022/10/13 20:35:52 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

/* Haven't checked yet whether SDL will leak if SDL_Quit hasn't been called 
** before exiting. For now, SDL_Quit is only called when ESC is pressed.
*/

static void	sdl_errors(int error_code)
{
	if (error_code == EC_SDL_INIT)
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
	if (error_code == EC_SDL_CREATEWINDOW)
		fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
	if (error_code == EC_SDL_GETWINDOW_SURFACE)
		fprintf(stderr, "SDL_GetWindowSurface failed: %s\n", SDL_GetError());
	if (error_code == EC_SDL_UPDATEWINDOWSURFACE)
		fprintf(stderr, "SDL_UpdateWindowSurface failed: %s\n", SDL_GetError());
	if (error_code == EC_SDL_SETRELATIVEMOUSEMODE)
		fprintf(stderr, "SDL_SetRelativeMouseMode failed: %s\n", SDL_GetError());
	exit (1);
}

static void	standard_function_errors(error_code)
{
	if (error_code == EC_OPEN)
		fprintf(stderr, "Open failed\n");
	if (error_code == EC_CLOSE)
		fprintf(stderr, "Close failed\n");
	if (error_code == EC_WRITE)
		fprintf(stderr, "Write failed\n");
	if (error_code == EC_MALLOC)
		fprintf(stderr, "Malloc failed\n");
	exit (2);
}

void	error_log(int error_code)
{
	if (error_code <= 10)
		sdl_errors(error_code);
	if (error_code >= 11 && error_code <= 20)
		standard_function_errors(error_code);
}
