/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 04:27:26 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/29 18:20:00 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRESENHAM_H
# define BRESENHAM_H

typedef struct s_bresenham //CHANGE TO USE Uu_int32_t32
{
	int32_t	diff[2];
	int32_t	add[2];
	int32_t	local[3];
	int32_t	target[2];
	int32_t	error;
}	t_bresenham;

void	populate_bresenham(t_bresenham *b, int32_t *from, int32_t *to);
int		step_bresenham(t_bresenham *b);
void	step_bresenham_x(t_bresenham *b);
void	step_bresenham_y(t_bresenham *b);

#endif