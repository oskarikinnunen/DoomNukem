/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:56:54 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 10:50:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

bool	check_alpha_key(uint32_t alphakeystate, char c)
{
	c = ft_tolower(c);
	if (!ft_isalpha(c))
		return (false);
	return ((alphakeystate >> (c - 'a')) & 1);
}

static void	toggle_keydown(t_hid_info *hid, SDL_Event e)
{
	char	c;

	hid->keystate |= key_is_move_left(e) << KEYS_LEFTMASK;
	hid->keystate |= key_is_move_right(e) << KEYS_RIGHTMASK;
	hid->keystate |= key_is_move_up(e) << KEYS_UPMASK;
	hid->keystate |= key_is_move_down(e) << KEYS_DOWNMASK;
	hid->keystate |= iskey(e, SDLK_v) << KEYS_CTRLMASK;
	hid->keystate |= iskey(e, SDLK_SPACE) << KEYS_SPACEMASK;
	hid->keystate |= iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK;
	hid->keystate |= iskey(e, SDLK_LALT) << KEYS_LALTMASK;
	hid->keystate |= iskey(e, SDLK_DELETE) << KEYS_DELETEMASK;
	hid->keystate |= iskey(e, SDLK_1) << KEYS_1MASK;
	hid->keystate |= iskey(e, SDLK_2) << KEYS_2MASK;
	hid->keystate |= iskey(e, SDLK_3) << KEYS_3MASK;
	hid->keystate |= iskey(e, SDLK_4) << KEYS_4MASK;
	hid->keystate |= iskey(e, SDLK_RETURN) << KEYS_ENTERMASK;
	c = 'a';
	while (c <= 'z')
	{
		hid->alphakeystate |= iskey(e, c) << (c - 'a');
		if (((hid->alphakeystate >> (c - 'a')) & 1) == 1)
			hid->alphakey_pressed |= iskey(e, c) << (c - 'a');
		c++;
	}
}

static void	toggle_keyup(t_hid_info *hid, SDL_Event e)
{
	char	c;

	hid->keystate &= ~(key_is_move_left(e)) << KEYS_LEFTMASK;
	hid->keystate &= ~(key_is_move_right(e) << KEYS_RIGHTMASK);
	hid->keystate &= ~(key_is_move_up(e) << KEYS_UPMASK);
	hid->keystate &= ~(key_is_move_down(e) << KEYS_DOWNMASK);
	hid->keystate &= ~(iskey(e, SDLK_v) << KEYS_CTRLMASK);
	hid->keystate &= ~(iskey(e, SDLK_SPACE) << KEYS_SPACEMASK);
	hid->keystate &= ~(iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK);
	hid->keystate &= ~(iskey(e, SDLK_LALT) << KEYS_LALTMASK);
	hid->keystate &= ~(iskey(e, SDLK_DELETE) << KEYS_DELETEMASK);
	hid->keystate &= ~(iskey(e, SDLK_1) << KEYS_1MASK);
	hid->keystate &= ~(iskey(e, SDLK_2) << KEYS_2MASK);
	hid->keystate &= ~(iskey(e, SDLK_3) << KEYS_3MASK);
	hid->keystate &= ~(iskey(e, SDLK_4) << KEYS_4MASK);
	hid->keystate &= ~(iskey(e, SDLK_RETURN) << KEYS_ENTERMASK);
	c = 'a';
	while (c <= 'z')
	{
		hid->alphakeystate &= ~(iskey(e, c) << (c - 'a'));
		c++;
	}
}

void	toggle_keystates(t_hid_info *hid, SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
		toggle_keydown(hid, e);
	else if (e.type == SDL_KEYUP)
		toggle_keyup(hid, e);
}
