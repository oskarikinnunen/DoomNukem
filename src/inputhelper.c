/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:20:17 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 10:49:27 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool iskey(SDL_Event e, int keycode)
{
	return (e.key.keysym.sym == keycode);
}

bool	keyismoveleft(SDL_Event e)
{
	return (iskey(e, SDLK_a));
}

bool	keyismoveright(SDL_Event e)
{
	return (iskey(e, SDLK_d));
}

bool	keyismoveup(SDL_Event e)
{
	return (iskey(e, SDLK_w));
}

bool	keyismovedown(SDL_Event e)
{
	return (iskey(e, SDLK_s));
}

//debug
bool	keyarrowdown(SDL_Event e)
{
	return (iskey(e, SDLK_DOWN));
}

bool	keyarrowup(SDL_Event e)
{
	return (iskey(e, SDLK_UP));
}

bool	keyarrowright(SDL_Event e)
{
	return (iskey(e, SDLK_RIGHT));
}

bool	keyarrowleft(SDL_Event e)
{
	return (iskey(e, SDLK_LEFT));
}

/*bool check_keystate(int32_t keystate, int32_t mask)
{
	return (keystate & mask);
}*/