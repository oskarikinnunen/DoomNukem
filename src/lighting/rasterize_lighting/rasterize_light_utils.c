/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_light_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 17:18:20 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/22 14:13:59 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_v2rectangle	get_bounds(t_point_triangle *triangle)
{
	t_v2rectangle	bounds;

	sort_point_uv_tri(triangle->p, triangle->t);
	if (clockwise(triangle->p) >= -1.0f)
	{
		ft_swap(&triangle->p[0], &triangle->p[2], sizeof(t_vector2));
		ft_swap(&triangle->t[0], &triangle->t[2], sizeof(t_vector3));
	}
	bounds.min = get_min_from_vector2_triangle(triangle->p);
	bounds.max = get_max_from_vector2_triangle(triangle->p);
	bounds.min = vector2_floorf(bounds.min);
	bounds.max = vector2_ceilf(bounds.max);
	return (bounds);
}

t_vector2	get_barycentric_offset(t_vector2 p[3])
{
	t_vector2	offset;
	t_vector2	start_bary;
	t_vector2	end_bary;

	start_bary = barycentric_coordinates(p, (t_vector2){0, 0});
	end_bary = barycentric_coordinates(p, (t_vector2){1, 1});
	offset = vector2_sub(start_bary, end_bary);
	offset = vector2_abs(offset);
	offset = vector2_negative(offset);
	return (offset);
}
