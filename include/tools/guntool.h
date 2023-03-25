/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guntool.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:08:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 21:24:37 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUNTOOL_H
# define GUNTOOL_H

#include "room.h"

struct s_editor;
struct s_sdlcontext;

typedef enum e_guntoolmode
{
	gtm_model,
	gtm_offset,
	gtm_recoil,
	gtm_stats,
}	t_guntoolmode;

typedef struct s_guntooldata
{
	t_autogui		maingui;
	t_autogui		offsetgui;
	t_autogui		presetgui;
	t_autogui		recoilgui;
	t_gun			gun;
	bool			gun_aim;
	t_guntoolmode	gtm;
}	t_guntooldata;

void	gun_tool_init(struct s_editor *ed, struct s_sdlcontext *sdl);
void	gun_tool_update(struct s_editor *ed, struct s_sdlcontext *sdl);

# endif
