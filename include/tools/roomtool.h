/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roomtool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:42:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/21 17:00:46 by vlaine           ###   ########.fr       */
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
	rtm_modify,
	rtm_split,
	rtm_paint
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
	t_wall			*splitwalls;
	t_floor_area	*area;
	uint32_t		splitwallcount;
	int				floor_debugvalue;
	t_floorcalc		fc;
}	t_roomtooldata;

# endif