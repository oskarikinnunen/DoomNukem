/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roomtool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:42:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/24 15:44:12 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOMTOOL_H
# define ROOMTOOL_H

#include "walltool.h"

//defines the current state of roomtool
typedef enum e_roomtoolmode
{
	/* rtm_select ?*/
	rtm_create,
	rtm_modify
} t_roomtoolmode;

typedef struct s_roomtooldata
{
	t_roomtoolmode	rtm;
	t_room			*room;
	t_wall			wall;
	t_wall			*ed_wall;
	t_wall			doorwalls[3];
	t_floorcalc		fc;
}	t_roomtooldata;

# endif