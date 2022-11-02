/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 12:09:50 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 18:56:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v3lib.h"
#include "bresenham.h"

void	step_bresenham_x(t_bresenham *b)
{
	if (b->error * 2 < b->diff.x && b->local.y != b->target.y)
	{
		b->local.y += b->add.y;
		b->error += b->diff.x;
	}
}

void	step_bresenham_y(t_bresenham *b)
{
	if (b->error * 2 > b->diff.y && b->local.x != b->target.x)
	{
		b->local.x += b->add.x;
		b->error += b->diff.y;
	}
}

#include <stdio.h>

int		step_bresenham(t_bresenham *b)
{
	step_bresenham_x(b);
	step_bresenham_y(b);
	printf("b.local %i %i b->target %i %i \n", b->local.x, b->local.y, b->target.x, b->target.y);
	return (b->local.x == b->target.x && b->local.y == b->target.y);
}

void	populate_bresenham(t_bresenham *b, t_point from, t_point to)
{
	b->local = from;
	b->diff.x = ft_abs(b->local.x - to.x);
	b->diff.y = -ft_abs(b->local.y - to.y);
	b->add.x = 1 - ((b->local.x > to.x) * 2);
	b->add.y = 1 - ((b->local.y > to.y) * 2);
	b->target = to;
	b->error = b->diff.x + b->diff.y;
}
