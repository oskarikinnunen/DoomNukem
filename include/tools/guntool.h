/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guntool.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:08:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/07 10:18:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUNTOOL_H
# define GUNTOOL_H

#include "room.h"

typedef enum e_guntoolmode
{
	gtm_model,
	gtm_offset,
	gtm_recoil,
	gtm_stats,
}	t_guntoolmode;

typedef struct s_guntooldata
{
	t_autogui		maingui;
	t_autogui		offsetgui;
	t_autogui		recoilgui;
	t_gun			gun;
	bool			gun_aim;
	t_guntoolmode	gtm;
}	t_guntooldata;

# endif