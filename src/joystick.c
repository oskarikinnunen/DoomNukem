/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joystick.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:01 by raho              #+#    #+#             */
/*   Updated: 2022/11/09 14:18:22 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int	controller_events(SDL_Event e, t_game *game)
{
	// Analog sticks
	if (e.type == SDL_JOYAXISMOTION)
	{

		
		// Left analog stick
		// left-right
		if (e.jaxis.axis == 0)
		{
			if ((e.jaxis.value < -3200) || (e.jaxis.value > 3200))
			{
				// printf("left stick - left-right: e.jaxis.axis: %d\n", e.jaxis.value);
			
			
			
			}
		}
		
		// up-down
		if (e.jaxis.axis == 1)
		{
			if ((e.jaxis.value < -3200) || (e.jaxis.value > 3200))
			{
				// printf("left stick - up-down: e.jaxis.axis: %d\n", e.jaxis.value);

				
					
			}	
		}

		
		// Right analog stick
		// left-right
		if (e.jaxis.axis == 2)
		{
			if ((e.jaxis.value < -3200) || (e.jaxis.value > 3200))
			{
				// printf("right stick - left-right: e.jaxis.axis: %d\n", e.jaxis.value);
			
			
			
			}
		}
				
		// up-down
		if (e.jaxis.axis == 5)
		{
			if ((e.jaxis.value < -3200) || (e.jaxis.value > 3200))
			{
				// printf("right stick - up-down: e.jaxis.axis: %d\n", e.jaxis.value);
			
			
			
			}
		}
		
		
	}
	// Pressing buttons
	else if (e.type == SDL_JOYBUTTONDOWN)
	{
		// printf("buttondown: %d\n", e.jbutton.button);
		
		// square
		if (e.jbutton.button == 0)
		{
			
		}
		
		// cross
		if (e.jbutton.button == 1)
		{
			
		}
		
		// circle
		if (e.jbutton.button == 2)
		{
			
		}
		
		// triangle
		if (e.jbutton.button == 3)
		{
			
		}
		
		// l1
		if (e.jbutton.button == 4)
		{
			
		}

		// r1
		if (e.jbutton.button == 5)
		{
			
		}

		// l2
		if (e.jbutton.button == 6)
		{
			
		}

		// r2
		if (e.jbutton.button == 7)
		{
			
		}

		// share
		if (e.jbutton.button == 8)
		{
			
		}

		// options
		if (e.jbutton.button == 9)
		{
			
		}
		
		// l3
		if (e.jbutton.button == 10)
		{
			
		}

		// r3
		if (e.jbutton.button == 11)
		{
			
		}

		// ps
		if (e.jbutton.button == 12)
		{
			return (game_exit);
		}

		// touchpad
		if (e.jbutton.button == 13)
		{
			return (game_switchmode);
		}
	}

	
	// Releasing buttons
	else if (e.type == SDL_JOYBUTTONUP)
	{
		// printf("buttonup: %d\n", e.jbutton.button);
		
		// square
		if (e.jbutton.button == 0)
		{
			
		}
		
		// cross
		if (e.jbutton.button == 1)
		{
			
		}
		
		// circle
		if (e.jbutton.button == 2)
		{
			
		}
		
		// triangle
		if (e.jbutton.button == 3)
		{
			
		}
		
		// l1
		if (e.jbutton.button == 4)
		{
			
		}

		// r1
		if (e.jbutton.button == 5)
		{
			
		}

		// l2
		if (e.jbutton.button == 6)
		{
			
		}

		// r2
		if (e.jbutton.button == 7)
		{
			
		}

		// share
		if (e.jbutton.button == 8)
		{
			
		}

		// options
		if (e.jbutton.button == 9)
		{
			
		}
		
		// l3
		if (e.jbutton.button == 10)
		{
			
		}

		// r3
		if (e.jbutton.button == 11)
		{
			
		}

		// ps
		if (e.jbutton.button == 12)
		{
			
		}

		// touchpad
		if (e.jbutton.button == 13)
		{
			
		}
	}

	
	// Pressing/releasing directional pad buttons
	else if (e.type == SDL_JOYHATMOTION)
	{
		// printf("joyhat value: %d\n", e.jhat.value);
		
		// up
		if (e.jbutton.button == 1)
		{
			
		}
		
		// up-right
		if (e.jbutton.button == 3)
		{
			
		}

		// right
		if (e.jbutton.button == 2)
		{
			
		}

		// down-right
		if (e.jbutton.button == 6)
		{
			
		}

		// down
		if (e.jbutton.button == 4)
		{
			
		}

		// down-left
		if (e.jbutton.button == 12)
		{
			
		}

		// left
		if (e.jbutton.button == 8)
		{
			
		}

		// up-left
		if (e.jbutton.button == 9)
		{
			
		}
	}	
	return (game_continue);
}
