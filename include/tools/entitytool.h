/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitytool.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:28:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 19:55:39 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITYTOOL_H
# define ENTITYTOOL_H

# include "entity.h"
# include "editor_tools.h"

typedef struct s_entity_editor
{
	t_autogui		gui;
	t_autogui		component_gui;
	bool			tr_toggle;
	bool			component_toggle;
}	t_entity_editor;

typedef struct s_entitytooldata
{
	t_autogui		world_gui;
	t_entity_editor	entity_ed;
	t_autogui		new_object_gui;
	t_entity		*sel_ent;
	t_entity		*new_ent;
	bool			grabbing;
	t_raycast_info	info;
}	t_entity_tool_data;

void	entitytool_entitygui_init(t_editor *ed, t_sdlcontext *sdl, \
									t_entity_tool_data *dat);
void	entitytool_worldgui_init(t_editor *ed, t_sdlcontext *sdl, \
									t_entity_tool_data *dat);
void	entitytool_objectgui_init(t_editor *ed, t_sdlcontext *sdl, \
									t_entity_tool_data *dat);
void	entity_tool_raycast(t_editor *ed, t_sdlcontext *sdl, \
									t_entity_tool_data *dat);
void	entity_tool_modify(t_editor *ed, t_sdlcontext *sdl, \
									t_entity_tool_data *dat);
void	entity_tool_place(t_editor *ed, t_sdlcontext *sdl, \
									t_entity_tool_data *dat);
void	gui_entity_mode(t_entity *entity, t_autogui *gui, t_world *world);
void	gui_component(t_entity *entity, t_autogui *gui, t_world *world);
void	find_bounds(t_entity *ent);

#endif