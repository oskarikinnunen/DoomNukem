/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:48:43 by raho              #+#    #+#             */
/*   Updated: 2023/03/22 18:31:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
#include "movement_defs.h"

t_collision	calculate_new_pos(t_vector2 delta, float radius)
{
	t_collision	result;
	float		dist;

	dist = vector2_magnitude(delta);
	result.normal = vector2_mul(delta, (1.0f / dist));
	result.depth = radius - dist;
	return (result);
}

bool	linepoint(t_vector2 start, t_vector2 end, t_vector2 point)
{
	float	distance1;
	float	distance2;
	float	line_len;
	float	buffer;

	distance1 = vector2_dist(point, start);
	distance2 = vector2_dist(point, end);
	line_len = vector2_dist(start, end);
	buffer = 0.1f;
	if ((distance1 + distance2) >= (line_len - buffer) && \
			(distance1 + distance2) <= (line_len + buffer))
		return (true);
	return (false);
}

bool	pointcircle(t_vector2 point, t_vector2 circle, float radius)
{
	t_vector2	delta;

	delta = vector2_sub(circle, point);
	if (vector2_dot(delta, delta) <= radius * radius)
		return (true);
	return (false);
}

bool	linecirclecollision(t_line line, t_vector2 circle, float radius)
{
	float		wall_len;
	float		dot;
	t_vector2	closest;
	t_vector2	delta;
	float		dist;

	if (pointcircle(line.start, circle, radius))
		return (true);
	if (pointcircle(line.end, circle, radius))
		return (true);
	wall_len = vector2_dist(line.start, line.end);
	dot = (((circle.x - line.start.x) * (line.end.x - line.start.x)) + \
			((circle.y - line.start.y) * (line.end.y - line.start.y))) / \
			(wall_len * wall_len);
	closest.x = line.start.x + (dot * (line.end.x - line.start.x));
	closest.y = line.start.y + (dot * (line.end.y - line.start.y));
	if (linepoint(line.start, line.end, closest))
	{
		if (pointcircle(closest, circle, radius))
			return (true);
	}
	return (false);
}

bool	col_linecircle(t_line line, t_vector2 circle,
						float radius, t_collision *collision)
{
	float		wall_len;
	float		dot;
	t_vector2	closest;
	t_vector2	delta;
	float		dist;

	if (pointcircle(line.start, circle, radius))
	{
		*collision = calculate_new_pos(vector2_sub(circle, line.start), radius);
		return (true);
	}
	if (pointcircle(line.end, circle, radius))
	{
		*collision = calculate_new_pos(vector2_sub(circle, line.end), radius);
		return (true);
	}
	wall_len = vector2_dist(line.start, line.end);
	dot = (((circle.x - line.start.x) * (line.end.x - line.start.x)) + \
			((circle.y - line.start.y) * (line.end.y - line.start.y))) / \
			(wall_len * wall_len);
	closest.x = line.start.x + (dot * (line.end.x - line.start.x));
	closest.y = line.start.y + (dot * (line.end.y - line.start.y));
	if (linepoint(line.start, line.end, closest))
	{
		if (pointcircle(closest, circle, radius))
		{
			*collision = calculate_new_pos(vector2_sub(circle, closest), radius);
			return (true);
		}
	}
	return (false);
}

/*
bool	rooms_share_zspace(t_room *room1, t_room *room2)
{
	bool	share = false;
	if (room1->height < (room2->height + room2->ceiling_height)
		&& room1->height >= room2->height)
		share = true;
	if (room2->height < (room1->height + room1->ceiling_height)
		&& room2->height >= room1->height)
		share = true;
	return (share);
}
*/

//Collider height which can be ignored aka stairs etc

bool	playerwall_stepthreshold(t_wall	*wall, t_player *player)
{
	float	topdist;

	topdist = player->transform.position.z - wall->entity->obj->vertices[2].z;
	//printf("topdist was %i \n", )
	return (topdist < 0.0f && topdist > -COL_STEP);
	//return (topdist < 0.0f && topdist > -50.0f);
}

bool	playerwall_share_z(t_wall	*wall, t_player *player)
{
	bool	share;

	share = false;
	if (wall->entity->obj->vertices[0].z < player->transform.position.z + player->height
		&& wall->entity->obj->vertices[0].z >= player->transform.position.z)
		share = true;
	if (player->transform.position.z < wall->entity->obj->vertices[2].z
		&& player->transform.position.z >= wall->entity->obj->vertices[0].z)
		share = true;
	return (share);
}

bool	wall_capsule_stepthreshold(t_wall	*wall, t_characterphysics cp)
{
	float	topdist;

	topdist = cp.position->z - wall->entity->obj->vertices[2].z;
	return (topdist < 0.0f && topdist > -COL_STEP);
}

bool	wall_capsule_share_z(t_wall	*wall, t_characterphysics cp)
{
	bool	share;

	share = false;
	if (wall->entity->obj->vertices[0].z < cp.position->z + cp.height
		&& wall->entity->obj->vertices[0].z >= cp.position->z)
		share = true;
	if (cp.position->z < wall->entity->obj->vertices[2].z
		&& cp.position->z >= wall->entity->obj->vertices[0].z)
		share = true;
	return (share);
}

bool	check_collision_character(t_world *world, t_characterphysics cp, t_vector3 potential_pos, t_vector3 *new_pos)
{
	t_list		*l;
	t_area		*room;
	int			index;
	t_collision	collision;

	l = world->arealist;
	while (l != NULL)
	{
		room = l->content;
		if (room != NULL)
		{
			index = 0;
			while (index < room->wallcount)
			{
				if (!room->walls[index].entity->hidden && \
					(wall_capsule_share_z(&room->walls[index], cp)
					&& !wall_capsule_stepthreshold(&room->walls[index], cp)))
				{
					if (col_linecircle((t_line){*room->walls[index].edgeline.start, *room->walls[index].edgeline.end}, \
							(t_vector2){potential_pos.x, potential_pos.y}, cp.radius, &collision))
					{
						*new_pos = vector3_add(potential_pos, v2tov3(vector2_mul(collision.normal, collision.depth + 0.00001f * world->clock.delta)));
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
