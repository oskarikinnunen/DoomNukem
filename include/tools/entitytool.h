/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitytool.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:28:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/13 08:14:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITYTOOL_H
# define ENTITYTOOL_H

#include "entity.h"
#include "editor_tools.h"

typedef struct s_prefab_editor
{
	t_entity		preview_entity;
	t_prefab		*prefab;
	t_autogui		object_select;
	t_autogui		prefab_select;
	t_autogui		prefab_modify;
	t_autogui		subtype_gui;
	bool			view_lock;
}	t_prefab_editor;

typedef struct s_entityeditor
{
	t_autogui		gui;
	t_autogui		component_gui;
	bool			transform_toggle;
	bool			component_toggle;
}	t_entityeditor;

typedef struct s_entitytooldata
{
	t_autogui		worldgui;
	t_entityeditor	entityeditor;
	//t_autogui		entitygui;
	t_objectgui		objectgui;
	t_prefab_editor	prefab_editor;
	t_entity		*ent;
	t_entity		*sel_ent;
	bool			grabbing;
	t_raycastinfo	info;
}	t_entitytooldata;

void	entitytool_entitygui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat);
void	entitytool_worldgui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat);
void	entitytool_prefabgui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat);
void	entitytool_objectgui_init(t_editor	*ed, t_sdlcontext *sdl, t_entitytooldata *dat);

#endif