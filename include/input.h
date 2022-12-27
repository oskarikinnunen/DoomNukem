#ifndef INPUT_H
# define INPUT_H

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

# define CONTROLLER_SENS 0.002f
# define DEADZONE 0.1f
# define MAX_CONTROLLERS 2

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
	t_vector2	turn;
}	t_input;

typedef struct s_controller
{
	SDL_GameController		*gamecontroller;
	SDL_JoystickID			instance_id;
	bool					connected;
	t_vector2				leftanalog;
	t_vector2				rightanalog;
	float					lefttrigger;
	float					righttrigger;
	bool					a;
	bool					b;
	bool					x;
	bool					y;
	bool					back;
	bool					guide;
	bool					start;
	bool					leftstick;
	bool					rightstick;
	bool					leftshoulder;
	bool					rightshoulder;
	bool					dpad_up;
	bool					dpad_down;
	bool					dpad_left;
	bool					dpad_right;
}	t_controller;

typedef struct s_hid_info
{
	t_mouse			mouse;
	uint32_t		keystate;
	uint32_t		alphakeystate; //Stores keystate from 'a' to 'z'
	uint32_t		alphakey_pressed; //Stores keystate from 'a' to 'z'
	t_controller	controller[2];
	t_input			input;
}	t_hid_info;

void	updateinput(t_input *input, int keystate, t_mouse m, t_controller *controller);
bool	iskey(SDL_Event e, int keycode);
bool	check_alpha_key(uint32_t alphakeystate, char c);
bool	mouse_clicked(t_mouse mouse, int button);
bool	keyismoveleft(SDL_Event e);
bool	keyismoveright(SDL_Event e);
bool	keyismoveup(SDL_Event e);
bool	keyismovedown(SDL_Event e);

#endif