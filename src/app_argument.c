/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_argument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:06:15 by raho              #+#    #+#             */
/*   Updated: 2023/03/20 19:28:33 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	check_editor_flags(int argc, char **argv,
								t_app_argument *app_argument)
{
	if (argc == 3 && ft_strcmp(argv[1], "-edit") == 0)
	{
		app_argument->app_mode = APPMODE_EDIT;
		ft_strncpy_term(app_argument->level_name, argv[2], 120);
	}
	else if ((argc == 2 && ft_strcmp(argv[1], "-edit") == 0) || argc == 1)
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

t_app_argument	get_app_argument(int argc, char **argv)
{
	t_app_argument	app_argument;

	ft_bzero(&app_argument, sizeof(t_app_argument));
	check_editor_flags(argc, argv, &app_argument);
	check_playmode_flags(argc, argv, &app_argument);
	if (argc == 2 && ft_strcmp(argv[1], "-gfxreset") == 0)
		app_argument.app_mode = APPMODE_GFX_RESET;
	return (app_argument);
}
