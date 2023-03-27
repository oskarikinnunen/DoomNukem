/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capsule_physics_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 19:07:24 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:23:56 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"
#include "collision.h"
#include "editor_tools.h"

static bool	charphys_ceil_share_z(t_character_physics *cp, t_meshtri *floor)
{
	return (floor->v->z >= cp->position->z
		+ (cp->height / 2.0f) && floor->v->z <= cp->position->z + cp->height);
}

bool	is_in_ceil(t_character_physics *cp, struct s_meshtri *ceil)
{
	t_ray			r;
	t_raycast_info	info;

	r.origin = vector3_add(*cp->position, (t_vector3){.z = 100.0f});
	r.dir = (t_vector3){.z = -1.0f};
	info.distance = 10000.0f;
	if (!ceil->entity)
		return (false);
	if ((raycast_entity(r, &info, ceil->entity)
			|| touches_edges(v3tov2(*cp->position), cp->radius, ceil))
		&& charphys_ceil_share_z(cp, ceil))
		return (true);
	return (false);
}

static bool	is_in_floor(t_character_physics *cp, t_meshtri *floor)
{
	t_ray			r;
	t_raycast_info	info;

	r.origin = vector3_add(*cp->position, (t_vector3){.z = 100.0f});
	r.dir = (t_vector3){.z = -1.0f};
	info.distance = 10000.0f;
	if (!floor->entity)
		return (false);
	if ((raycast_entity(r, &info, floor->entity)
			|| touches_edges(v3tov2(*cp->position), cp->radius, floor))
		&& charphys_floor_share_z(cp, floor))
		return (true);
	return (false);
}

bool	is_in_ramp(t_character_physics *cp, t_vector3_tri *ramp)
{
	t_ray			r;
	t_raycast_info	info;

	r.origin = vector3_add(*cp->position, (t_vector3){.z = 100.0f});
	r.dir = (t_vector3){.z = -1.0f};
	info.distance = 10000.0f;
	if ((raycast_tri(r, *ramp, &info.distance)
			|| touches_edges_ramp(v3tov2(*cp->position), cp->radius, *ramp))
		&& charphys_ramp_share_z(cp, *ramp))
	{
		return (true);
	}
	return (false);
}

float	get_z_from_areas(t_character_physics *cp, t_world *world)
{
	t_list	*list;
	t_area	*area;
	int		i;
	float	z;

	z = 0;
	list = world->arealist;
	while (list != NULL)
	{
		area = (t_area *)list->content;
		i = 0;
		while (i < area->floor_count)
		{
			if (is_in_floor(cp, &area->floors[i]))
				z = ft_maxf(z, (float)area->height);
			i++;
		}
		list = list->next;
	}
	return (z);
}
