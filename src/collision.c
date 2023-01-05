/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:48:43 by raho              #+#    #+#             */
/*   Updated: 2023/01/05 18:10:13 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static bool	linepoint(t_vector2 start, t_vector2 end, t_vector2 point)
{
	float	distance1;
	float	distance2;
	float	line_len;
	float	buffer;

	distance1 = vector2_dist(point, start);
	distance2 = vector2_dist(point, end);
	line_len = vector2_dist(start, end);
	buffer = 0.1;
	if ((distance1 + distance2) >= (line_len - buffer) && (distance1 + distance2) <= (line_len + buffer))
		return (true);
	return (false);
}

static bool	pointcircle(t_vector2 point, t_vector2 circle, float radius)
{
	if (vector2_dist(point, circle) <= radius)
		return (true);
	return (false);
}

bool	linecircle(t_vector2 start, t_vector2 end, t_vector2 circle, float radius)
{
	
}

bool	check_collision(t_world *world, t_player *player, t_vector3 potential_pos, t_vector3 *newpos)
{
	t_list		*l;
	t_room		*room;
	t_edgeline	*wall_line;
	int			index;
	float		circle_radius;
	float		wall_len;
	float		dot;
	t_vector2	closest;
	bool		on_segment;
	t_vector2	dist;
	float		distance;
	static int	printer = 0;

	circle_radius = 10;
	l = world->roomlist;
	while (l != NULL)
	{
		room = l->content;
		if (room != NULL)
		{
			index = 0;
			while (index < room->wallcount)
			{
				wall_line = &room->walls[index].edgeline;
				printf("potential_pos.z: %f\n", potential_pos.z);
				if (potential_pos.z < room->walls[index].height * 1.3f)
				{
					if (pointcircle(*wall_line->start, (t_vector2){potential_pos.x, potential_pos.y}, circle_radius) || \
						pointcircle(*wall_line->end, (t_vector2){potential_pos.x, potential_pos.y}, circle_radius))
						return (true);
					wall_len = vector2_dist(*wall_line->start, *wall_line->end);
					/* dot = vector2_dot((t_vector2){(potential_pos.x - wall_line->start->x), (wall_line->end->x - wall_line->start->x)}, \
									(t_vector2){(potential_pos.y - wall_line->start->y), (wall_line->end->y - wall_line->start->y)}) / \
									(wall_len * wall_len); */ //didnt work so had to write it open below
					dot = (((potential_pos.x - wall_line->start->x) * (wall_line->end->x - wall_line->start->x)) + \
							((potential_pos.y - wall_line->start->y) * (wall_line->end->y - wall_line->start->y))) / (wall_len * wall_len);
					closest.x = wall_line->start->x + (dot * (wall_line->end->x - wall_line->start->x));
					closest.y = wall_line->start->y + (dot * (wall_line->end->y - wall_line->start->y));
					on_segment = linepoint(*wall_line->start, *wall_line->end, closest);
					if (on_segment)
					{
						distance = vector2_dist(closest, (t_vector2){potential_pos.x, potential_pos.y});
						if (distance <= circle_radius)
							return (true);
					}
				}
				index++;
			}
		}
		l = l->next;
	}
	return (false);
}
