/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:37:38 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 11:12:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "assets.h"
#include "objects.h"
#include "file_io.h"

static void	check_editor_flags(int argc, char **argv,
								t_app_argument *app_argument)
{
	if (argc == 3 && ft_strcmp(argv[1], "-edit") == 0)
	{
		app_argument->app_mode = APPMODE_EDIT;
		ft_strncpy_term(app_argument->level_name, argv[2], 120);
	}
	else if (argc == 2 && ft_strcmp(argv[1], "-edit") == 0)
	{
		app_argument->app_mode = APPMODE_EDIT;
		ft_strncpy_term(app_argument->level_name, DEFAULTLEVEL, 120);
	}
}

static void	check_playmode_flags(int argc, char **argv,
								t_app_argument *app_argument)
{
	if (argc == 3 && ft_strcmp(argv[1], "-play") == 0)
	{
		app_argument->app_mode = APPMODE_PLAY;
		ft_strncpy_term(app_argument->level_name, argv[2], 120);
	}
	else if ((argc == 2 && ft_strcmp(argv[1], "-play") == 0) || argc == 1)
	{
		app_argument->app_mode = APPMODE_PLAY;
		ft_strncpy_term(app_argument->level_name, DEFAULTLEVEL, 120);
	}
}

static t_app_argument	get_app_argument(int argc, char **argv)
{
	t_app_argument	app_argument;

	ft_bzero(&app_argument, sizeof(t_app_argument));
	check_editor_flags(argc, argv, &app_argument);
	check_playmode_flags(argc, argv, &app_argument);
	return (app_argument);
}

int	main(int argc, char **argv)
{
	t_sdlcontext	sdl;
	t_app_argument	app_argument;

	app_argument = get_app_argument(argc, argv);
	if (app_argument.app_mode == APPMODE_EDIT)
	{
		create_sdlcontext(app_argument, &sdl);
		editor(app_argument, sdl);
	}
	else if (app_argument.app_mode == APPMODE_PLAY)
	{
		create_sdlcontext(app_argument, &sdl);
		playmode(app_argument, sdl);
	}
	else if (app_argument.app_mode == APPMODE_INVALID)
	{
		ft_putendl_fd(\
			"usage: ./DoomNukem [-edit | -play] level_file", 2);
		return (1);
	}
	return (0);
}
