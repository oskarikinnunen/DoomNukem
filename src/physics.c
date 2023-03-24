/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 13:52:50 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 14:56:33 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "shapes.h"
#include "objects.h"

/*t_entity	*raycast_result(t_vector3 origin, t_vector3 direction, t_list *entitylist)
{
	
}*/

/*static	t_object	*object_cube()
{
	static t_object	cube;

	if (cube.vertice_count != 0)
		return &cube;
	cube.vertice_count = 8;
	cube.vertices = ft_memalloc(8 * sizeof(t_vector3));
	if (cube.vertices == NULL)
		error_log(EC_MALLOC);
	cube.vertices[0] = vector3_zero();
	cube.vertices[1] = vector3_zero();
}*/


bool	collision_point_rectangle(t_point p, t_rectangle rect) //dunno if this should be in physics
{
	return (p.x > rect.position.x && p.x < rect.position.x + rect.size.x
		&& p.y > rect.position.y && p.y < rect.position.y + rect.size.y);
}

float sign (t_point p1, t_vector3 p2, t_vector3 p3)
{
    return ((float)p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * ((float)p1.y - p3.y);
}

float sign2 (t_point p1, t_point p2, t_point p3)
{
    return ((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y));
}

bool pointtrianglecollision (t_point point, t_world_triangle tri)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(point, tri.p[0].v, tri.p[1].v);
    d2 = sign(point, tri.p[1].v, tri.p[2].v);
    d3 = sign(point, tri.p[2].v, tri.p[0].v);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

bool linelineintersect(t_line line1, t_line line2)
{
	float	xdiff1;
	float	xdiff2;
	float	ydiff1;
	float	ydiff2;
	float	res;

	xdiff1 = line1.start.x - line1.end.x;
	xdiff2 = line2.start.x - line2.end.x;
	ydiff1 = line1.start.y - line1.end.y;
	ydiff2 = line2.start.y - line2.end.y;
	res = xdiff1 * ydiff2 - ydiff1 * xdiff2;
	if (fabsf(res) > 0.001f)
	{
		float fa = line1.start.x * line1.end.y - line1.start.y * line1.end.x;
		float fb = line2.start.x * line2.end.y - line2.start.y * line2.end.x;

		float x = (fa * xdiff2 - fb * xdiff1) / res;
		float y = (fa * ydiff2 - fb * ydiff1) / res;
		if ((ft_minf(line1.start.x, line1.end.x) < x
			&& ft_maxf(line1.start.x, line1.end.x) > x
			&& ft_minf(line2.start.x, line2.end.x) < x
			&& ft_maxf(line2.start.x, line2.end.x) > x)
			|| (ft_minf(line1.start.y, line1.end.y) < y
			&& ft_maxf(line1.start.y, line1.end.y) > y
			&& ft_minf(line2.start.y, line2.end.y) < y
			&& ft_maxf(line2.start.y, line2.end.y) > y))
		{
			return (true);
		}
	}
	return (false);
}
