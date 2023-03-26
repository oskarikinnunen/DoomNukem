/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:58:37 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:02:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# define MOUSE_LEFT 1
# define MOUSE_MDL 2
# define MOUSE_RIGHT 3
# define KEYS_LEFTMASK 0
# define KEYS_RIGHTMASK 1
# define KEYS_UPMASK 2
# define KEYS_DOWNMASK 3
# define KEYS_SPACEMASK 4
# define KEYS_CTRLMASK 5
# define KEYS_SHIFTMASK 6
# define KEYS_MMASK 7
# define KEYS_LALTMASK 8
# define KEYS_DELETEMASK 9
# define KEYS_1MASK 10
# define KEYS_2MASK 11
# define KEYS_3MASK 12
# define KEYS_4MASK 13
# define KEYS_VMASK 14
# define KEYS_ENTERMASK 15

# define CONTROLLER_SENS 0.002f
# define DEADZONE 0.1f
# define MAX_CONTROLLERS 2

struct	s_game;

typedef struct s_mouse
{
	t_point		pos;
	t_point		delta;
	bool		safe_delta;
	bool		relative;
	int			scroll;
	int			scroll_delta;
	bool		click_unhandled;
	int			click_button;
	int			held;
	bool		dragging_ui;
	uint32_t	heldstate;
}	t_mouse;

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
	bool		shoot;
	bool		aim;
	bool		use;
	bool		reload;
	t_vector2	turn;
	bool		nextgun;
	bool		prevgun;
}	t_input;

typedef struct s_controller
{
	SDL_GameController		*gamecontroller;
	SDL_JoystickID			instance_id;
	bool					connected;
	t_vector2				left_analog;
	t_vector2				right_analog;
	float					left_trigger;
	float					right_trigger;
	bool					a;
	bool					b;
	bool					x;
	bool					y;
	bool					back;
	bool					guide;
	bool					start;
	bool					left_stick;
	bool					right_stick;
	bool					left_shoulder;
	bool					right_shoulder;
	bool					dpad_up;
	bool					dpad_down;
	bool					dpad_left;
	bool					dpad_right;
}	t_controller;

//alphakeystate: Stores keystate from 'a' to 'z'
//alphakey_pressed: Stores keystate from 'a' to 'z'
typedef struct s_hid_info
{
	t_mouse			mouse;
	uint32_t		keystate;
	uint32_t		alphakeystate;
	uint32_t		alphakey_pressed;
	t_controller	controller[2];
	t_input			input;
}	t_hid_info;

void	update_input(t_input *input, t_hid_info info);
bool	iskey(SDL_Event e, int keycode);
bool	check_alpha_key(uint32_t alphakeystate, char c);
bool	mouse_clicked(t_mouse mouse, int button);
bool	key_is_move_left(SDL_Event e);
bool	key_is_move_right(SDL_Event e);
bool	key_is_move_up(SDL_Event e);
bool	key_is_move_down(SDL_Event e);
void	toggle_keystates(t_hid_info *hid, SDL_Event e);
void	playmode_events(struct s_game *game);

#endif