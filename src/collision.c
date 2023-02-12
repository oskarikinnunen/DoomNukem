/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:48:43 by raho              #+#    #+#             */
/*   Updated: 2023/02/03 18:06:23 by okinnune         ###   ########.fr       */
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

bool	col_linecircle(t_line line, t_vector2 circle, float radius, t_collision *collision)
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

	l = world->roomlist;
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
						//room->walls[index].entity->obj->materials->img = get_image_by_name(*world->sdl, "barrel.cng");
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

bool	check_collision(t_world *world, t_player *player, t_vector3 potential_pos, t_vector3 *new_pos)
{
	t_list		*l;
	t_area		*room;
	int			index;
	t_collision	collision;

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
				if (!room->walls[index].entity->hidden && \
					(playerwall_share_z(&room->walls[index], player)
					&& !playerwall_stepthreshold(&room->walls[index], player)))
				{
					if (col_linecircle((t_line){*room->walls[index].edgeline.start, *room->walls[index].edgeline.end}, \
							(t_vector2){potential_pos.x, potential_pos.y}, player->collision_radius, &collision))
					{
						*new_pos = vector3_add(potential_pos, v2tov3(vector2_mul(collision.normal, collision.depth + 1)));
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


// alaiwan disk/segment collision
// t_vector2	closest_point_on_segment(t_vector2 point, t_line line)
// {
// 	t_vector2	tangent;
// 	t_vector2	normalized_tangent;
// 	t_vector2	relative_position;

// 	tangent = vector2_sub(line.end, line.start);
	
// 	if (vector2_dot(vector2_sub(point, line.start), tangent) <= 0)
// 		return (line.start);
	
// 	if (vector2_dot(vector2_sub(point, line.end), tangent) >= 0)
// 		return (line.end);

// 	normalized_tangent = vector2_normalise(tangent);
// 	relative_position = vector2_sub(point, line.start);
// 	return (vector2_add(line.start, \
// 			vector2_multiply(normalized_tangent, \
// 			vector2_multiply(normalized_tangent, relative_position))));
	
// }

// bool	collide_circle_with_line(t_vector2 circle, t_line line, float radius, t_collision *collision)
// {
// 	t_vector2	delta;
// 	float		distance;
// 	float		length;

// 	delta = closest_point_on_segment(circle, line);
// 	distance = vector2_dist(circle, delta);

// 	if ((distance * distance) > (radius * radius))
// 		return (false);

// 	length = vector2_magnitude(delta);
// 	//printf("collision delta length: %f	depth: %f\n", length, radius - length);
// 	collision->depth = radius - length;
// 	collision->normal = vector2_mul(delta, (1.0f / distance));
// 	return (true);
// }

// bool	collide_circle_with_lines(t_world *world, t_player *player, t_vector3 potential_pos, t_collision *collision)
// {
// 	t_list		*l;
// 	t_room		*room;
// 	int			index;
// 	t_collision	deepest;

// 	ft_bzero(&deepest, sizeof(t_collision));
// 	l = world->roomlist;
// 	while (l != NULL)
// 	{
// 		room = l->content;
// 		if (room != NULL)
// 		{
// 			index = 0;
// 			while (index < room->wallcount)
// 			{
// 				if (!room->walls[index].entity->hidden && \
// 					(potential_pos.z > room->walls[index].entity->obj->vertices[0].z &&
// 					potential_pos.z < room->walls[index].entity->obj->vertices[2].z * 1.2f))
// 				{
// 					if (collide_circle_with_line((t_vector2){potential_pos.x, potential_pos.y}, \
// 							(t_line){*room->walls[index].edgeline.start, *room->walls[index].edgeline.end}, \
// 							player->collision_radius, collision))
// 					{
// 						printf("collided with a line - saving deepest.depth\n");
// 						printf("collision->depth: %f	collision_normal: %f, %f\n", collision->depth, collision->normal.x, collision->normal.y);
// 						if (collision->depth < deepest.depth)
// 						{
// 							deepest.depth = collision->depth;
// 							deepest.normal = collision->normal;
// 						}
// 						printf("deepest->depth: %f	deepest_normal: %f, %f\n", deepest.depth, deepest.normal.x, deepest.normal.y);
// 					}
// 				}
// 				index++;
// 			}
// 		}
// 		l = l->next;
// 	}
// 	if (deepest.depth != 0)
// 		return (true);
// 	return (false);			
// }

//bool	alaiwan_collision(t_world *world, t_player *player, t_vector3 potential_pos, t_vector3 *new_pos)
// {
// 	t_collision	collision;

// 	player->collision_radius = 15;
// 	if (collide_circle_with_lines(world, player, potential_pos, &collision))
// 	{
// 		printf("collided with at least one of the lines - saving new_pos\n");
// 		*new_pos = vector3_add(*potential_pos, vector2_to_vector3(vector2_mul(collision.normal, collision.depth)));
// 		return (true);
// 	}
// 	printf("no collision\n");
// 	return (false);
// }




















// ericleong collision
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