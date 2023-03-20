/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/17 15:03:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	save_controller_info(t_hid_info *hid,
							SDL_Joystick *joystick_id, int joysticks, int i)
{
	while (i < joysticks && i < MAX_CONTROLLERS)
	{
		if (SDL_IsGameController(i))
		{
			hid->controller[i].gamecontroller = \
					SDL_GameControllerOpen(i);
			if (!hid->controller[i].gamecontroller)
				doomlog(LOG_EC_SDL_GAMECONTROLLEROPEN, NULL);
			joystick_id = \
					SDL_GameControllerGetJoystick(\
							hid->controller[i].gamecontroller);
			hid->controller[i].instance_id = \
					SDL_JoystickInstanceID(joystick_id);
			if (hid->controller[i].instance_id < 0)
				doomlog(LOG_EC_SDL_JOYSTICKINSTANCEID, NULL);
			else
				hid->controller[i].connected = true;
			doomlog_mul(LOG_NORMAL, (char *[4]){
				"controller", s_itoa(i + 1), "connected", NULL});
		}
		else
			doomlog(LOG_WARNING, "controller not supported");
		i++;
	}
}

void	initialize_controllers(t_hid_info *hid)
{
	SDL_Joystick	*joystick_id;
	int				joysticks;
	int				i;

	joysticks = SDL_NumJoysticks();
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			"number of joysticks/controllers plugged in:", \
			s_itoa(joysticks), NULL});
	i = 0;
	if (joysticks > 0)
		save_controller_info(hid, joystick_id, joysticks, i);
	if (SDL_GameControllerEventState(SDL_QUERY) == 0)
		SDL_GameControllerEventState(SDL_ENABLE);
}
