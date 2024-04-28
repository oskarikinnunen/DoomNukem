/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_character.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:58:20 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:27:18 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "collision.h"
#include "movement_defs.h"

bool	wall_capsule_step_threshold(t_wall	*wall, t_character_physics cp)
{
	float	topdist;

	topdist = cp.position->z - wall->entity->obj->vertices[2].z;
	return (topdist < 0.0f && topdist > -COL_STEP);
}

bool	wall_capsule_share_z(t_wall	*wall, t_character_physics cp)
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

static void	init_cc_struct(t_character_collision *cc, \
										t_character_physics cp, \
										t_vector3 potential_pos)
{
	ft_bzero(cc, sizeof(t_character_collision));
	cc->cp = cp;
	cc->potential_pos = potential_pos;
}

static bool	confirm_collision(t_character_collision *cc, t_vector3 *new_pos,
								uint32_t delta_time)
{
	if (!cc->wall->entity->hidden && \
			(wall_capsule_share_z(cc->wall, cc->cp) && \
			!wall_capsule_step_threshold(cc->wall, cc->cp)))
	{
		if (collision_line_circle_save_collision(\
				(t_line){(*cc->wall->edge_line.start), \
				(*cc->wall->edge_line.end)}, \
				(t_vector2){cc->potential_pos.x, cc->potential_pos.y}, \
				cc->cp.radius, &cc->collision))
		{
			*new_pos = vector3_add(cc->potential_pos, v2tov3(vector2_mul(\
					cc->collision.normal, cc->collision.depth + \
					0.00001f * delta_time)));
			return (true);
		}
	}
	return (false);
}

bool	check_character_collision(t_world *world, t_character_physics cp,
								t_vector3 potential_pos, t_vector3 *new_pos)
{
	t_list					*l;
	t_area					*room;
	int						index;
	t_character_collision	cc;

	init_cc_struct(&cc, cp, potential_pos);
	l = world->arealist;
	while (l != NULL)
	{
		room = l->content;
		if (room != NULL)
		{
			index = 0;
			while (index < room->wall_count)
			{
				cc.wall = &room->walls[index];
				if (confirm_collision(&cc, new_pos, \
						world->clock.delta) == true)
					return (true);
				index++;
			}
		}
		l = l->next;
	}
	return (false);
}
