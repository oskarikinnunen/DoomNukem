/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_functions2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 19:31:26 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/22 19:31:34 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_vector3	normal_calc_quaternion(t_quaternion p[3])
{
	t_vector3	normal;
	t_vector3	line1;
	t_vector3	line2;

	line1 = vector3_sub(p[1].v, p[0].v);
	line2 = vector3_sub(p[2].v, p[0].v);
	normal = vector3_crossproduct(line1, line2);
	normal = vector3_normalise(normal);
	return (normal);
}

t_vector3	normal_calc(t_vector3 p[3])
{
	t_vector3 normal, line1, line2;

	line1 = vector3_sub(p[1], p[0]);
	line2 = vector3_sub(p[2], p[0]);
	normal = vector3_crossproduct(line1, line2);
	normal = vector3_normalise(normal);
	return (normal);
}
