#ifndef INPUT_H
# define INPUT_H

typedef struct s_mouse
{
	t_point		pos;
	t_point		delta;
	bool		relative;
	int			scroll;
	int			scroll_delta;
	bool		click_unhandled;
	int			click_button;
	int			held;
	uint32_t	heldstate;
}	t_mouse;

#endif