/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelp.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:30:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/18 21:01:26 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTHELP_H
# define INPUTHELP_H

# include "../SDL_built/include/SDL2/SDL.h"

# define KEYS_LEFTMASK 0
# define KEYS_RIGHTMASK 1
# define KEYS_UPMASK 2
# define KEYS_DOWNMASK 3
# define KEYS_SPACEMASK 4
# define KEYS_CTRLMASK 5


bool	keyismoveleft(SDL_Event e);
bool	keyismoveright(SDL_Event e);
bool	keyismoveup(SDL_Event e);
bool	keyismovedown(SDL_Event e);

#endif