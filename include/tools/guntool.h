/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guntool.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:08:53 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 19:53:58 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUNTOOL_H
# define GUNTOOL_H

# include "room.h"

struct	s_editor;
struct	s_sdlcontext;

typedef enum e_gun_tool_mode
{
	gtm_model,
	gtm_offset,
	gtm_recoil,
	gtm_stats,
}	t_gun_tool_mode;

typedef struct s_gun_tool_data
{
	t_autogui		main_gui;
	t_autogui		offset_gui;
	t_autogui		preset_gui;
	t_autogui		recoil_gui;
	t_gun			gun;
	bool			gun_aim;
	t_gun_tool_mode	gtm;
}	t_gun_tool_data;

void	gun_tool_init(struct s_editor *ed, struct s_sdlcontext *sdl);
void	gun_tool_update(struct s_editor *ed, struct s_sdlcontext *sdl);

#endif