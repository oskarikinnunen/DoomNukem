/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roomtool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:42:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/06 15:52:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOMTOOL_H
# define ROOMTOOL_H

#include "walltool.h"
#include "editor_tools.h"

//defines the current state of roomtool
typedef enum e_roomtoolmode
{
	/* rtm_select ?*/
	rtm_create,
	rtm_modify
} t_roomtoolmode;

t_vector2	flipped_uv(t_vector2 og);

typedef struct s_roomtooldata
{
	t_roomtoolmode	rtm;
	t_autogui		maingui;
	t_autogui		modroom_gui;
	t_autogui		newroom_gui;
	t_room			*room;
	t_wall			wall;
	t_wall			*ed_wall;
	t_wall			doorwalls[3];
	t_floorcalc		fc;
}	t_roomtooldata;

# endif