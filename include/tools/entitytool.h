/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entitytool.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:28:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/02 15:55:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITYTOOL_H
# define ENTITYTOOL_H

#include "entity.h"
#include "editor_tools.h"

typedef struct s_entitytooldata
{
	t_autogui	maingui;
	t_objectgui	objectgui;
	t_entity	*ent;
}	t_entitytooldata;

#endif