/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelp.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:30:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 10:50:16 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTHELP_H

# include "../SDL_built/include/SDL2/SDL.h"

# define KEYS_LEFTMASK 0
# define KEYS_RGHTMASK 2
# define KEYS_UPMASK 4
# define KEYS_DOWNMASK 8


bool	keyismoveleft(SDL_Event e);
bool	keyismoveright(SDL_Event e);
bool	keyismoveup(SDL_Event e);
bool	keyismovedown(SDL_Event e);

#endif