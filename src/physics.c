/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 13:52:50 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/05 14:32:48 by okinnune         ###   ########.fr       */
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


bool	pointrectanglecollision(t_point p, t_rectangle rect) //dunno if this should be in physics
{
	return (p.x > rect.position.x && p.x < rect.position.x + rect.size.x
		&& p.y > rect.position.y && p.y < rect.position.y + rect.size.y);
}

bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r)
{
	return (vector2_dist(p, cp) <= r);
}

float sign (t_point p1, t_vector3 p2, t_vector3 p3)
{
    return ((float)p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * ((float)p1.y - p3.y);
}

float sign2 (t_point p1, t_point p2, t_point p3)
{
    return ((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y));
}

bool pointtrianglecollision (t_point point, t_triangle tri)
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
	if (fabsf(res) > 0.01f)
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
			printf("\nLINE COLLIDES\n");
			return (true);
		}
	}
	return (false);
}

bool pointtrianglecollisionp (t_point point, t_point	t1, t_point	t2, t_point	t3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign2(point, t1, t2);
    d2 = sign2(point, t2, t3);
    d3 = sign2(point, t3, t1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

//TODO: no worky, look at https://www.jeffreythompson.org/collision-detection/line-circle.php and fix
bool	linecirclecollision(t_line line, t_vector2 cp, float r) //CONTINUE FIXING HERE
{
	return	false;
	/*float	start[2];
	float	end[2];
	float	dist;
	float	dot;
	
	f2tov2(start, line.start);
	f2tov2(end, line.end);
	f2mul(start, TILESIZE); //Lines are in their grid positions so multiply by tilesize to get world coordinates
	f2mul(end, TILESIZE);
	dist = f2dist(start, end);
	dot = (((cp[X]-start[X])*(end[X]-start[X])) + ((cp[Y]-start[Y])*(end[Y]-start[Y])) ) / pow(dist,2);
	float x_dist = (start[X] + (dot * (end[X]-start[X]))) - cp[X];
	float y_dist = (start[Y] + (dot * (end[Y]-start[Y]))) - cp[Y];
	dist = sqrt((x_dist * x_dist) + (y_dist * y_dist));
	return (dist <= r);*/
}