/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitytool.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:28:31 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/03 15:16:00 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITYTOOL_H
# define ENTITYTOOL_H

#include "entity.h"
#include "editor_tools.h"

typedef struct s_entitytooldata
{
	t_autogui		worldgui;
	t_autogui		entitygui;
	t_objectgui		objectgui;
	t_entity		*ent;
	t_entity		*sel_ent;
	bool			grabbing;
	t_raycastinfo	info;
}	t_entitytooldata;

#endif