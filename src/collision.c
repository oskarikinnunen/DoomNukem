/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:48:43 by raho              #+#    #+#             */
/*   Updated: 2023/01/10 15:39:18 by raho             ###   ########.fr       */
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
	float		wall_len;
	float		dot;
	t_vector2	closest;
	//t_vector2	sub;

	if (pointcircle(start, circle, radius) || pointcircle(end, circle, radius))
		return (true);
	wall_len = vector2_dist(start, end);
	/* dot = vector2_dot((t_vector2){(potential_pos.x - wall_line->start->x), (wall_line->end->x - wall_line->start->x)}, \
			(t_vector2){(potential_pos.y - wall_line->start->y), (wall_line->end->y - wall_line->start->y)}) / \
			(wall_len * wall_len); */ //didnt work so had to write it open below
	//sub = 
	dot = (((circle.x - start.x) * (end.x - start.x)) + \
		((circle.y - start.y) * (end.y - start.y))) / (wall_len * wall_len);
	closest.x = start.x + (dot * (end.x - start.x));
	closest.y = start.y + (dot * (end.y - start.y));
	if (linepoint(start, end, closest))
	{
		if (vector2_dist(closest, circle) <= radius)
			return (true);
	}
	return (false);
}

bool	check_collision(t_world *world, t_player *player, t_vector3 potential_pos, t_vector3 *new_pos)
{
	t_list		*l;
	t_room		*room;
	int			index;

	player->collision_radius = 10;
	l = world->roomlist;
	while (l != NULL)
	{
		room = l->content;
		if (room != NULL)
		{
			index = 0;
			while (index < room->wallcount)
			{
				//printf("potential_pos.z: %f\n", potential_pos.z);
				//room->walls[index].entity->obj->vertices[0].z
				//render_ray(world->sdl, player->transform.position, room->walls[index].entity->obj->vertices[0]);
				world->sdl->render.gizmocolor = CLR_RED;
				render_ray(world->sdl, vector3_add(player->transform.position, player->lookdir), room->walls[index].entity->obj->vertices[0]);
				render_gizmo3d(world->sdl, room->walls[index].entity->obj->vertices[0], 5, CLR_RED);
				render_gizmo3d(world->sdl, room->walls[index].entity->obj->vertices[1], 5, CLR_RED);
				render_gizmo3d(world->sdl, room->walls[index].entity->obj->vertices[2], 10, CLR_BLUE);
				render_gizmo3d(world->sdl, room->walls[index].entity->obj->vertices[3], 10, CLR_BLUE);
				if (!room->walls[index].entity->hidden && \
					(potential_pos.z > room->walls[index].entity->obj->vertices[0].z &&
					potential_pos.z < room->walls[index].entity->obj->vertices[2].z * 1.3f))
				{
					if (linecircle(*room->walls[index].edgeline.start, \
						*room->walls[index].edgeline.end, \
						(t_vector2){potential_pos.x, potential_pos.y}, \
						player->collision_radius))
					{
						printf("collision true\n");
						return (true);
					}
				}
				index++;
			}
		}
		l = l->next;
	}
	printf("collision false\n");
	return (false);
}

// if (pointcircle(*wall_line->start, (t_vector2){potential_pos.x, potential_pos.y}, radius) || \
// 		pointcircle(*wall_line->end, (t_vector2){potential_pos.x, potential_pos.y}, radius))
// 	return (true);
// wall_len = vector2_dist(*wall_line->start, *wall_line->end);
// /* dot = vector2_dot((t_vector2){(potential_pos.x - wall_line->start->x), (wall_line->end->x - wall_line->start->x)}, \
// 		(t_vector2){(potential_pos.y - wall_line->start->y), (wall_line->end->y - wall_line->start->y)}) / \
// 		(wall_len * wall_len); */ //didnt work so had to write it open below
// dot = (((potential_pos.x - wall_line->start->x) * (wall_line->end->x - wall_line->start->x)) + \
// 		((potential_pos.y - wall_line->start->y) * (wall_line->end->y - wall_line->start->y))) / \
// 		(wall_len * wall_len);
// closest.x = wall_line->start->x + (dot * (wall_line->end->x - wall_line->start->x));
// closest.y = wall_line->start->y + (dot * (wall_line->end->y - wall_line->start->y));
// if (linepoint(*wall_line->start, *wall_line->end, closest))
// {
// 	if (vector2_dist(closest, (t_vector2){potential_pos.x, potential_pos.y}) <= radius)
// 		return (true);
// }