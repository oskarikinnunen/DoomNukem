/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelp.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:30:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/17 14:48:27 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTHELP_H

# include "../SDL_built/include/SDL2/SDL.h"

//wasd
# define KEYS_LEFTMASK 0
# define KEYS_RIGHTMASK 1
# define KEYS_UPMASK 2
# define KEYS_DOWNMASK 3
// For debugging arrowkeys
# define KEYS_ARROWDOWN 4
# define KEYS_ARROWUP 5
# define KEYS_ARROWLEFT 6
# define KEYS_ARROWRIGHT 7

bool	keyismoveleft(SDL_Event e);
bool	keyismoveright(SDL_Event e);
bool	keyismoveup(SDL_Event e);
bool	keyismovedown(SDL_Event e);

//debug
bool	keyarrowdown(SDL_Event e);
bool	keyarrowup(SDL_Event e);
bool	keyarrowright(SDL_Event e);
bool	keyarrowleft(SDL_Event e);
#endif