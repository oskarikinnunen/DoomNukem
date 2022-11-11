/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelp.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:30:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/11 16:12:34 by raho             ###   ########.fr       */
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
# define KEYS_SHIFTMASK 6
# define KEYS_MMASK 7
# define CONTROLLER_SENS 0.002f
# define DEADZONE 0.1f

typedef enum s_inputmode
{
	keyboard,
	controller
}	t_inputmode;

typedef struct s_input
{
	t_inputmode	mode;
	t_vector2	move;
	bool		crouch;
	bool		jump;
	bool		run;
	t_vector2	turn;
}	t_input;

typedef struct s_controller
{
	t_vector2	leftanalog;
	t_vector2	rightanalog;
	int			dpad;
	bool		square;
	bool		cross;
	bool		circle;
	bool		triangle;
	bool		l1;
	bool		r1;
	bool		l2;
	bool		r2;
	bool		share;
	bool		options;
	bool		l3;
	bool		r3;
	bool		home;
	bool		touchpad;
}	t_controller;

struct s_mouse;

bool	iskey(SDL_Event e, int keycode);
bool	mouse_clicked(struct s_mouse mouse, int button);
bool	keyismoveleft(SDL_Event e);
bool	keyismoveright(SDL_Event e);
bool	keyismoveup(SDL_Event e);
bool	keyismovedown(SDL_Event e);

#endif