/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:20:17 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:01:12 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool	iskey(SDL_Event e, int keycode)
{
	return (e.key.keysym.sym == keycode);
}

bool	key_is_move_left(SDL_Event e)
{
	return (iskey(e, SDLK_LEFT) || iskey(e, SDLK_a));
}

bool	key_is_move_right(SDL_Event e)
{
	return (iskey(e, SDLK_RIGHT) || iskey(e, SDLK_d));
}

bool	key_is_move_up(SDL_Event e)
{
	return (iskey(e, SDLK_UP) || iskey(e, SDLK_w));
}

bool	key_is_move_down(SDL_Event e)
{
	return (iskey(e, SDLK_DOWN) || iskey(e, SDLK_s));
}
