/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_window.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:59:55 by raho              #+#    #+#             */
/*   Updated: 2023/03/08 21:01:53 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	save_messages(t_parent *p, char *str, int i)
{
	p->messages[0] = \
			ft_strdup("GAME RAN INTO AN ERROR! DISPLAYING LOG ENTRIES:");
	if (p->messages[0] == NULL)
	{
		ft_putstr_fd("malloc for error message failed - ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit (1);
	}
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
		ft_putstr_fd("can't read doomlog for error message -", 2);
		ft_putendl_fd(strerror(errno), 2);
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
		ft_putstr_fd("malloc for error message failed - ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit (1);
	}
	p->fd = open("doomlog.txt", O_RDONLY);
	if (p->fd == -1)
	{
		ft_putstr_fd("can't open doomlog for error message - ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit (1);
	}
	save_messages(p, str, 0);
	if (close(p->fd) == -1)
	{
		ft_putstr_fd("couldn't close doomlog - ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit (1);
	}
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
	if (p->message_count > 0)
		p->message_count += 2;
	if (p->gnl == -1)
	{
		ft_putstr_fd("can't read log for error message - ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit(1);
	}
}

static void	parse_log(t_parent *p)
{
	p->fd = open("doomlog.txt", O_RDONLY);
	if (p->fd == -1)
	{
		ft_putstr_fd("can't open doomlog for error message - ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit (1);
	}
	count_log_messages(p);
	if (close(p->fd) == -1)
	{
		ft_putstr_fd("couldn't close doomlog - ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit (1);
	}
}

void	error_window(char *str)
{
	t_parent	p;

	ft_bzero(&p, sizeof(t_parent));
	ft_putendl_fd(str, 2);
	init_sdl_error_window(&p.sdl);
	editor_load_fonts(&p.sdl, "assets/.font_list.txt");
	p.sdl.font_default->color = color32_to_sdlcolor(CLR_GREEN);
	parse_log(&p);
	cache_log(&p, str);
	while (1)
	{
		p.ew.hid.mouse.scroll_delta = 0; // TODO: Still needs to be reset?
		p.ew.hid.alphakey_pressed = 0; // TODO: Still needs to be reset?
		SDL_GetRelativeMouseState(&p.ew.hid.mouse.delta.x, \
									&p.ew.hid.mouse.delta.y);
		error_window_events(&p);
	}
}
