/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitytool.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:28:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 22:32:39 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITYTOOL_H
# define ENTITYTOOL_H

#include "entity.h"
#include "editor_tools.h"

typedef struct s_entityeditor
{
	t_autogui		gui;
	t_autogui		component_gui;
	bool			tr_toggle;
	bool			component_toggle;
}	t_entityeditor;

typedef struct s_entitytooldata
{
	t_autogui		worldgui;
	t_entityeditor	entity_ed;
	t_autogui		new_object_gui;
	t_entity		*sel_ent;
	t_entity		*new_ent;
	bool			grabbing;
	t_raycastinfo	info;
}	t_entitytooldata;

void	entitytool_entitygui_init(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat);
void	entitytool_worldgui_init(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat);
void	entitytool_objectgui_init(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat);
void	entity_tool_raycast(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat);

#endif