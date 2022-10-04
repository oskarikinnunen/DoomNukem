/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 12:09:50 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/29 18:21:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v3lib.h"
#include "bresenham.h"

void	step_bresenham_x(t_bresenham *b)
{
	if (b->error * 2 < b->diff[X] && b->local[Y] != b->target[Y])
	{
		b->local[Y] += b->add[Y];
		b->error += b->diff[X];
	}
}

void	step_bresenham_y(t_bresenham *b)
{
	if (b->error * 2 > b->diff[Y] && b->local[X] != b->target[X])
	{
		b->local[X] += b->add[X];
		b->error += b->diff[Y];
	}
}

int		step_bresenham(t_bresenham *b)
{
	step_bresenham_x(b);
	step_bresenham_y(b);
	return (b->local[X] == b->target[X] && b->local[Y] == b->target[Y]);
}

void	populate_bresenham(t_bresenham *b, int32_t *from, int32_t *to)
{
	ft_memcpy(b->local, from, sizeof(int32_t [3]));
	b->diff[X] = ft_abs(b->local[X] - to[X]);
	b->diff[Y] = -ft_abs(b->local[Y] - to[Y]);
	b->add[X] = 1 - ((b->local[X] > to[X]) * 2);
	b->add[Y] = 1 - ((b->local[Y] > to[Y]) * 2);
	b->target[X] = to[X];
	b->target[Y] = to[Y];
	b->error = b->diff[X] + b->diff[Y];
}
