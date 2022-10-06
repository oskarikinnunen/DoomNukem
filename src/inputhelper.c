/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:20:17 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 10:49:03 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool iskey(SDL_Event e, int keycode)
{
	return (e.key.keysym.sym == keycode);
}

bool	keyismoveleft(SDL_Event e)
{
	return (iskey(e, SDLK_LEFT) || iskey(e, SDLK_a));
}

bool	keyismoveright(SDL_Event e)
{
	return (iskey(e, SDLK_RIGHT) || iskey(e, SDLK_d));
}

bool	keyismoveup(SDL_Event e)
{
	return (iskey(e, SDLK_UP) || iskey(e, SDLK_w));
}

bool	keyismovedown(SDL_Event e)
{
	return (iskey(e, SDLK_DOWN) || iskey(e, SDLK_s));
}

/*bool check_keystate(int32_t keystate, int32_t mask)
{
	return (keystate & mask);
}*/