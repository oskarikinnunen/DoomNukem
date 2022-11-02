/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 13:52:50 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 14:27:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "shapes.h"

t_vector3	raycast_result(t_vector3 origin, t_vector3 direction, t_list *entitylist)
{
	
}

bool	pointrectanglecollision(t_point p, t_rectangle rect) //dunno if this should be in physics
{
	return (p.x > rect.position.x && p.x < rect.position.x + rect.size.x
		&& p.y > rect.position.y && p.y < rect.position.y + rect.size.y);
}

bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r)
{
	return (vector2_dist(p, cp) <= r);
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