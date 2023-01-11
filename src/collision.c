/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:48:43 by raho              #+#    #+#             */
/*   Updated: 2023/01/11 23:12:30 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"

// t_vector2	ericleong_closestpointonline(t_vector2 point, t_line line)
// {
// 	float		a1;
// 	float		b1;
// 	double		c1;
// 	double		c2;
// 	double		determinant;
// 	t_vector2	closest;
	
// 	a1 = line.end.y - line.start.y;
// 	b1 = line.start.x - line.end.x;
// 	c1 = ((line.end.y - line.start.y) * line.start.x) + \
// 			((line.start.x - line.end.x) * line.start.y);
// 	c2 = (-b1 * point.x) + (a1 * point.y);
// 	determinant = (a1 * a1) - (-b1 * b1);
	
// 	if (determinant != 0)
// 	{
// 		closest.x = (float)(((a1 * c1) - (b1 * c2)) / determinant);
// 		closest.y = (float)(((a1 * c2) - (-b1 * c1)) / determinant);
// 	}
// 	else
// 	{
// 		closest.x = point.x;
// 		closest.y = point.y;
// 	}
// 	return (closest);
// }

// bool	ericleong_lineline(t_line first, t_line second, t_vector2 *collision_point)
// {
// 	float	a1;
// 	float	b1;
// 	float	c1;
// 	float	a2;
// 	float	b2;
// 	float	c2;
// 	float	determinant;

// 	a1 = first.end.y - first.start.y;
// 	b1 = first.start.x - first.end.x;
// 	c1 = (a1 * first.start.x) + (b1 * first.start.y);
// 	a2 = second.end.y - second.start.y;
// 	b2 = second.start.x - second.end.x;
// 	c2 = (a2 * second.start.x) + (b2 * second.start.y);
// 	determinant = (a1 * b2) - (a2 * b1);

// 	if (determinant != 0)
// 	{
// 		float	x;
// 		float	y;
		
// 		x = ((b2 * c1) - (b1 * c2)) / determinant;
// 		y = ((a1 * c2) - (a2 * c1)) / determinant;
		
// 		if (x >= ft_min(first.start.x, first.end.x) && \
// 				x <= ft_max(first.start.x, first.end.x) && \
// 				x >= ft_min(second.start.x, second.end.x) && \
// 				x <= ft_max(second.start.x, second.end.x) && \
// 				y >= ft_min(first.start.y, first.end.y) && \
// 				y <= ft_max(first.start.y, first.end.y) && \
// 				y >= ft_min(second.start.y, second.end.y) && \
// 				y <= ft_max(second.start.y, second.end.y))
// 		{
// 			collision_point->x = x;
// 			collision_point->y = y;
// 			return (true);
// 		}
// 	}
// 	return (false);
// }

// bool	ericleong(t_player *player, t_vector3 potential_pos, t_edgeline *line, t_vector3 *new_pos, t_world *world)
// {
// 	t_vector2	a;
// 	t_vector2	b;
// 	t_vector2	c;
// 	t_vector2	d;

// 	t_line	movement_line;
// 	t_vector2	direction;
	

// 	//printf("player position: %f, %f\n", player->transform.position.x, player->transform.position.y);
// 	//printf("player direction: %f, %f\n", player->lookdir.x, player->lookdir.y);
// 	//printf("potential position: %f, %f\n", potential_pos.x, potential_pos.y);
	

// 	movement_line.start = (t_vector2){player->transform.position.x, player->transform.position.y};
// 	movement_line.end = (t_vector2){potential_pos.x, potential_pos.y};

// 	direction = vector2_sub(movement_line.end, movement_line.start);
// 	direction = vector2_mul(direction, 10);
// 	movement_line.end = vector2_add(movement_line.end, direction);

// 	//printf("extended potential position: %f, %f\n", movement_line.end.x, movement_line.end.y);

// 	t_line	wall_line;
// 	wall_line.start = *line->start;
// 	wall_line.end = *line->end;

// 	//printf("line start: %f, %f		line end: %f, %f\n\n", wall_line.start.x, wall_line.start.y, wall_line.end.x, wall_line.end.y);

// 	if (ericleong_lineline(movement_line, wall_line, &a))
// 	{
// 		printf("a is on the line\n");
// 		render_gizmo3d(world->sdl, (t_vector3){a.x, a.y, 100}, 10, CLR_BLUE);
		
// 		b = ericleong_closestpointonline(movement_line.end, wall_line);
// 		if (vector2_dist(movement_line.end, b) < player->collision_radius)
// 		{
// 			printf("collision possible: b is less than r away from movement_line.end\n");
// 			render_gizmo3d(world->sdl, (t_vector3){b.x, b.y, 100}, 10, CLR_RED);
// 			return (true);
// 		}
// 		c = ericleong_closestpointonline(wall_line.start, movement_line);
// 		if (vector2_dist(wall_line.start, c) < player->collision_radius)
// 		{
// 			printf("collision possible: c is less than r away from wall_line.start\n");
// 			render_gizmo3d(world->sdl, (t_vector3){c.x, c.y, 100}, 10, CLR_GREEN);
// 			return (true);
// 		}
// 		d = ericleong_closestpointonline(wall_line.end, movement_line);
// 		if (vector2_dist(wall_line.end, d) < player->collision_radius)
// 		{
// 			printf("collision possible: d is less than r away from wall_line.end\n");
// 			render_gizmo3d(world->sdl, (t_vector3){d.x, d.y, 100}, 10, CLR_TURQ);
// 			return (true);
// 		}
// 	}
// 	return (false);
// }


static bool	linepoint(t_vector2 start, t_vector2 end, t_vector2 point)
{
	float	distance1;
	float	distance2;
	float	line_len;
	float	buffer;

	distance1 = vector2_dist(point, start);
	distance2 = vector2_dist(point, end);
	line_len = vector2_dist(start, end);
	buffer = 0.1f;
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

bool	linecircle(t_line line, t_vector2 circle, float radius, t_vector2 *closest_collision_point)
{
	float		wall_len;
	float		dot;
	t_vector2	closest;

	/* if (pointcircle(line.start, circle, radius) || pointcircle(line.end, circle, radius))
		return (true); */ // by quitting this soon, we dont get the closest point
	wall_len = vector2_dist(line.start, line.end);
	/* dot = vector2_dot((t_vector2){(potential_pos.x - wall_line->start->x), (wall_line->end->x - wall_line->start->x)}, \
			(t_vector2){(potential_pos.y - wall_line->start->y), (wall_line->end->y - wall_line->start->y)}) / \
			(wall_len * wall_len); */ //didnt work so had to write it open below
	dot = (((circle.x - line.start.x) * (line.end.x - line.start.x)) + \
		((circle.y - line.start.y) * (line.end.y - line.start.y))) / (wall_len * wall_len);
	closest.x = line.start.x + (dot * (line.end.x - line.start.x));
	closest.y = line.start.y + (dot * (line.end.y - line.start.y));
	if (linepoint(line.start, line.end, closest))
	{
		if (vector2_dist(closest, circle) <= radius)
		{
			*closest_collision_point = closest;
			return (true);
		}
	}
	return (false);
}

bool	lineline(t_line first, t_line second, t_vector2 *collision_point)
{
	float	a;
	float	b;
	
	
	a = (((second.end.x - second.start.x) * (first.start.y - second.start.y)) - \
		((second.end.y - second.start.y) * (first.start.x - second.start.x))) / \
		(((second.end.y - second.start.y) * (first.end.x - first.start.x)) - \
		((second.end.x - second.start.x) * (first.end.y - first.start.y)));
	
	b = (((first.end.x - first.start.x) * (first.start.y - second.start.y)) - \
		((first.end.y - first.start.y) * (first.start.x - second.start.x))) / \
		(((second.end.y - second.start.y) * (first.end.x - first.start.x)) - \
		((second.end.x - second.start.x) * (first.end.y - first.start.y)));

	if (a >= 0 && a <= 1 && b >= 0 && b <= 1)
	{
		collision_point->x = first.start.x + (a * (first.end.x - first.start.x));
		collision_point->y = first.start.y + (a * (first.end.y - first.start.y));
		return (true);
	}
	return (false);
}

t_vector2	closest_point_on_segment(t_vector2 point, t_line line)
{
	
}

bool	collide_circle_with_line(t_vector2 circle, t_line line, float radius, t_collision *collision)
{
	float	delta;
	

	delta = vector2_dist(circle, closest_point_on_segment(circle, line));
	if ((delta * delta) > (radius * radius))
		return (false);
	
}

bool	check_collision(t_world *world, t_player *player, t_vector3 potential_pos, t_vector3 *new_pos)
{
	t_list		*l;
	t_room		*room;
	int			index;

	player->collision_radius = 15;
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
				/* world->sdl->render.gizmocolor = CLR_RED;
				render_ray(world->sdl, vector3_add(player->transform.position, player->lookdir), room->walls[index].entity->obj->vertices[0]);
				render_gizmo3d(world->sdl, room->walls[index].entity->obj->vertices[0], 5, CLR_RED);
				render_gizmo3d(world->sdl, room->walls[index].entity->obj->vertices[1], 5, CLR_RED);
				render_gizmo3d(world->sdl, room->walls[index].entity->obj->vertices[2], 10, CLR_BLUE);
				render_gizmo3d(world->sdl, room->walls[index].entity->obj->vertices[3], 10, CLR_BLUE); */
				if (!room->walls[index].entity->hidden && \
					(potential_pos.z > room->walls[index].entity->obj->vertices[0].z &&
					potential_pos.z < room->walls[index].entity->obj->vertices[2].z * 1.2f))
				{
					/* t_vector2	closest;
					if (linecircle((t_line){*room->walls[index].edgeline.start, *room->walls[index].edgeline.end}, \
							(t_vector2){potential_pos.x, potential_pos.y}, player->collision_radius, &closest))
					{
						render_gizmo3d(world->sdl, (t_vector3){closest.x, closest.y, player->transform.position.z}, 10, CLR_GREEN);
						printf("collision true\n");
						return (true);
					} */
					t_collision	collision;
					if (collide_circle_with_line((t_vector2){potential_pos.x, potential_pos.y}, \
							(t_line){*room->walls[index].edgeline.start, *room->walls[index].edgeline.end}, \
							player->collision_radius, &collision))
					{
						
					}
				}
				index++;
			}
		}
		l = l->next;
	}
	//printf("collision false\n");
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