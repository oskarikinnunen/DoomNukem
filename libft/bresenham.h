/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 04:27:26 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/17 18:20:22 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRESENHAM_H
# define BRESENHAM_H

# include "../include/vectors.h"

typedef struct s_bresenham //CHANGE TO USE Uu_int32_t32
{
	t_point	diff;
	t_point	add;
	t_point	local;
	t_point	target;
	int32_t	error;
}	t_bresenham;

void	populate_bresenham(t_bresenham *b, t_point from, t_point to);
int		step_bresenham(t_bresenham *b);
void	step_bresenham_x(t_bresenham *b);
void	step_bresenham_y(t_bresenham *b);

#endif