/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guntool.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:08:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/25 19:17:14 by okinnune         ###   ########.fr       */
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
	t_gun			gun;
	t_guntoolmode	gtm;
}	t_guntooldata;

# endif