/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitytool.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:28:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/21 15:59:40 by okinnune         ###   ########.fr       */
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

void	entitytool_entitygui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat);
void	entitytool_worldgui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat);
void	entitytool_objectgui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat);

#endif