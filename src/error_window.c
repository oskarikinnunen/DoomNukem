#include "doomnukem.h"

static void	save_messages(t_parent *p, char *str)
{
	int	i;

	p->messages[0] = \
			ft_strdup("GAME RAN INTO AN ERROR! DISPLAYING LOG ENTRIES:");
	i = 1;
	p->gnl = get_next_line(p->fd, &p->line);
	while (p->gnl)
	{
		p->messages[i] = p->line;
		i++;
		p->gnl = get_next_line(p->fd, &p->line);
	}
	if (p->gnl == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't read the log for the error message", 2);
		exit(1);
	}
	p->messages[i++] = str;
	p->messages[i] = NULL;
}

static void	cache_log(t_parent *p, char *str)
{
	p->messages = (char **)malloc(sizeof(char *) * (p->message_count + 1));
	if (p->messages == NULL)
	{
		ft_putendl_fd("game ran into an error and the parent process's malloc for the error message failed", 2);
		exit (1);
	}
	p->fd = open("doomlog.txt", O_RDONLY);
	if (p->fd == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't open the log for the error message", 2);
		exit (1);
	}
	save_messages(p, str);
	if (close(p->fd) == -1)
		ft_putendl_fd("couldn't close the log from the parent process", 2);
}

static void	count_log_messages(t_parent *p)
{
	p->gnl = get_next_line(p->fd, &p->line);
	while (p->gnl)
	{
		if (p->line)
		{
			free(p->line);
			p->line = NULL;
		}
		p->message_count++;
		p->gnl = get_next_line(p->fd, &p->line);
	}
	p->message_count += 2; // 2 extra messages for the title and the exit status
	if (p->gnl == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't read the log for the error message", 2);
		exit(1);
	}
}

static void	parse_log(t_parent *p)
{
	p->fd = open("doomlog.txt", O_RDONLY);
	if (p->fd == -1)
	{
		ft_putendl_fd("game ran into an error and the parent process couldn't open the doomlog for the error message", 2);
		exit (1);
	}
	count_log_messages(p);
	if (close(p->fd) == -1)
		ft_putendl_fd("couldn't close the log from the parent process", 2);
}

void	error_window(char *str)
{
	t_parent	p;

	ft_bzero(&p, sizeof(t_parent));
	ft_putendl_fd(str, 2);
	init_sdl_error_window(&p.sdl);
	load_fonts(&p.sdl.font);
	p.sdl.font.color = color32_to_sdlcolor(CLR_GREEN);
	parse_log(&p);
	cache_log(&p, str);
	while (1)
	{
		p.ew.hid.mouse.scroll_delta = 0; //Needs to be reset
		p.ew.hid.alphakey_pressed = 0; //Needs to be reset
		SDL_GetRelativeMouseState(&p.ew.hid.mouse.delta.x, \
									&p.ew.hid.mouse.delta.y);
		window_events(&p);
	}
}
