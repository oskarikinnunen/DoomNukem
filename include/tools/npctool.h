/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npctool.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 19:48:29 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 19:52:37 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NPCTOOL_H
# define NPCTOOL_H

# include "room.h"
# include "colliders.h"
# include "npctool.h"

typedef enum e_npc_tool_mode
{
	ntm_select,
	ntm_addbox,
	ntm_addvbox
}	t_npc_tool_mode;

typedef struct s_npc_tool_data
{
	t_entity			*ent;
	int32_t				tri_index;
	t_vector3			*vertex;
	t_object_meta_data	metadata;
	int					i_col;
	bool				vertex_set;
	t_limbcollider		col;
	t_npc_tool_mode		ntm;
	t_autogui			main_gui;
	t_autogui			object_gui;
	t_autogui			collider_gui;
}	t_npc_tool_data;

#endif