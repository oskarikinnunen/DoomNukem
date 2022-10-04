/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 06:23:41 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 10:42:16 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool	v2cmp(int v[2], int ov[2])
{
	return (v[X] == ov[X] && v[Y] == ov[Y]);
}

void	v2mul(int v[2], int mul)
{
	v[X] *= mul;
	v[Y] *= mul;
}

void	v2add(int v[2], int ov[2])
{
	v[X] += ov[X];
	v[Y] += ov[Y];
}