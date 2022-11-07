/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walltool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:17:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/07 07:24:26 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WALLTOOL_H
# define WALLTOOL_H

#include "objects.h"

typedef	enum e_walltoolmode
{
	place_first,
	place_other,
	place_height
}	t_walltoolmode;

typedef struct s_walltooldata
{
	t_wall			wall;
	t_wall			*hover;
	t_walltoolmode	mode;
}	t_walltooldata;

#endif