/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capsulephysics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 03:25:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/26 14:31:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"
#include "physics.h"
#include "editor_tools.h"
//#include ""

static bool	touches_edges(t_vector2 pos, float radius, t_meshtri *floor)
{
	t_line		line1;
	t_line		line2;
	t_line		line3;
	t_collision	unused_col;

	line1.start = v3tov2(floor->v[0]);
	line1.end = v3tov2(floor->v[1]);
	line2.start = v3tov2(floor->v[1]);
	line2.end = v3tov2(floor->v[2]);
	line3.start = v3tov2(floor->v[2]);
	line3.end = v3tov2(floor->v[0]);
	return (col_linecircle(line1, pos, radius, &unused_col)
			|| col_linecircle(line2, pos, radius, &unused_col)
			|| col_linecircle(line3, pos, radius, &unused_col));
}

static bool	is_in_floor(t_characterphysics *cp, t_meshtri	*floor)
{
	t_ray r;
	r.origin = vector3_add(*cp->position, (t_vector3){.z = 100.0f});
	r.dir = (t_vector3){.z = -1.0f};
	t_raycastinfo	info;
	info.distance = 10000.0f;
	if (raycast_entity(r, &info, floor->entity)
		|| touches_edges(v3tov2(*cp->position), cp->radius, floor))
	{
		return (true);
	}
	return (false);
}

static float	get_z_position(t_characterphysics *cp, t_world *world)
{
	t_list	*list;
	t_room	*room;
	int		i;
	float	z;

	z = 0;

	list = world->roomlist;
	while(list != NULL)
	{
		room = (t_room *)list->content;
		i = 0;
		while (i < room->floorcount)
		{
			if (is_in_floor(cp, &room->floors[i]))
			{
				z = ft_maxf(z, (float)room->height);
			}
			i++;
		}
		list = list->next;
	}
	return (z);
}

void apply_capsulephysics(t_characterphysics charp, t_world *world) //TODO: this takes only 2D velocity vector
{
	t_vector3	potential_pos;
	t_vector3	new_pos;

	potential_pos = vector3_add(*charp.position, vector3_mul(*charp.velocity, world->clock.delta));
	new_pos = potential_pos;
	int i = 0;
	while (check_collision_character(world, charp, new_pos, &new_pos) && i < 5)
		i++;
	if (!check_collision_character(world, charp, new_pos, &new_pos))
		*charp.position = new_pos;
	float floorz = get_z_position(&charp, world);
	if (floorz <= charp.position->z)
	{
		*charp.isgrounded = (charp.position->z <= floorz);
		float target_z = floorz;
		float gravityapply = 0.0012f;
		float zveltarget;
		zveltarget = GRAVITY;
		if (charp.gravity_override != NULL)
			zveltarget = *charp.gravity_override;
		if (*charp.isgrounded)
			zveltarget = 0.0f;
		charp.velocity->z = ft_fmovetowards(charp.velocity->z, zveltarget, gravityapply * world->clock.delta);
		if (charp.velocity->z <= 0.0f)
			target_z = floorz;
		else
			target_z = 10000.0f;
		charp.position->z = ft_fmovetowards(charp.position->z, target_z, ft_absf(charp.velocity->z) * world->clock.delta);
		charp.position->z = ft_clampf(charp.position->z, floorz, 10000.0f);
		*charp.isgrounded = (charp.position->z <= floorz);
	}
	else
		charp.position->z = ft_fmovetowards(charp.position->z, floorz, world->clock.delta * -GRAVITY * 0.5f);
	if (*charp.isgrounded && charp.velocity->z != 0)
	{
		if (charp.landingtrigger != NULL)
			*charp.landingtrigger = true;
		if (charp.impactvelocity != NULL)
			charp.impactvelocity->z = charp.velocity->z;
		charp.velocity->z = 0.0f;
	}
	*charp.isgrounded = (charp.position->z <= floorz);
}

/*static void collision_movement(t_player *player, t_vector3 move_vector, t_world *world) //TODO: this takes only 2D velocity vector
{
	t_vector3	potential_pos;
	t_vector3	new_pos;

	potential_pos = vector3_add(player->transform.position, v2tov3(v3tov2(move_vector)));
	new_pos = potential_pos;
	int i = 0;
	while (check_collision(world, player, new_pos, &new_pos) && i < 5)
		i++;
	if (!check_collision(world, player, new_pos, &new_pos))
		player->transform.position = new_pos;
	float floorz = get_z_position(player, world);
	//printf("floor z %f \n", floorz);
	if (floorz <= player->transform.position.z)
	{
		player->isgrounded = (player->transform.position.z <= floorz);
		float target_z = floorz;
		//float gravityapply = 0.5f;
		float gravityapply = 0.0012f;
		float zveltarget;
		zveltarget = GRAVITY;
		if (player->jump.active)
		{
			float lerp = 1.5f - player->jump.lerp;
			//zveltarget = GRAVITY + (lerp * 7.4f); //perfect rocket jump, lol
			zveltarget = GRAVITY + (lerp * 0.7f);
		}
		if (player->isgrounded)
			zveltarget = 0.0f;
		player->velocity.z = ft_fmovetowards(player->velocity.z, zveltarget, gravityapply * world->clock.delta);
		if (player->velocity.z <= 0.0f)
			target_z = floorz;
		else
			target_z = 10000.0f;
		player->transform.position.z = ft_fmovetowards(player->transform.position.z, target_z, ft_absf(player->velocity.z) * world->clock.delta);
		player->transform.position.z = ft_clampf(player->transform.position.z, floorz, 10000.0f);
		player->isgrounded = (player->transform.position.z <= floorz);
	}
	else
		player->transform.position.z = ft_fmovetowards(player->transform.position.z, floorz, world->clock.delta * -GRAVITY * 0.5f);
	
	if (player->isgrounded && player->velocity.z != 0)
	{
		//play_landingsound(player, world);
		player->velocity.z = 0.0f;
	}
}*/