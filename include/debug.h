#ifndef DEBUG_H
# define DEBUG_H

typedef struct s_debug_option
{
	char	string[64];
	bool	toggled;
}	t_debug_option;

typedef struct s_debugconsole
{
	int				scroll;
	char			**messages;
	uint32_t		messagecount;
	char			line[10][1024];
	t_anim			show_anim;
	bool			hidden;
	t_player		*player;
	t_hid_info		*hid;
	t_debug_option	noclip;
}	t_debugconsole;

/* DEBUGCONSOLE.C */
void	debugconsole_addmessage(t_debugconsole *console, char *str);
void	update_debugconsole(t_debugconsole *console, t_sdlcontext *sdl, uint32_t delta);

/*DEBUG FILES*/
void print_tri(t_triangle tri);
void print_quat(t_quaternion v);
void print_vec(t_vector3 v);
void print_matrix(t_mat4x4 m);
void print_point(t_point p);
void print_texture(t_texture t);
void print_face(void *face);

#endif