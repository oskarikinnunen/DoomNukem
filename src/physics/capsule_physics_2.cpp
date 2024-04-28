/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capsule_physics_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 19:05:43 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:33:04 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"
#include "collision.h"
#include "editor_tools.h"

static float	sample_ramp_z(t_vector2 position, t_vector3_tri tri, int mod)
{
	float	ff;
	float	trihigh;
	float	trilow;
	t_line	line;
	float	rampz;

	trihigh = ft_maxf(ft_maxf(tri.a.z, tri.b.z), tri.c.z);
	trilow = ft_minf(ft_minf(tri.a.z, tri.b.z), tri.c.z);
	if (mod == 0)
	{
		line.start = v3tov2(tri.c);
		line.end = v3tov2(tri.a);
	}
	else
	{
		line.start = v3tov2(tri.c);
		line.end = v3tov2(tri.b);
	}
	ff = vector2_dist_along_line(position, line);
	rampz = ft_flerp(trilow, trihigh, ff);
	return (rampz);
}

static float	get_z_from_ramps(t_character_physics *cp, t_world *world)
{
	t_vector3_tri	*tri;
	t_list			*list;
	float			z;
	int				i;

	list = world->ramps;
	z = 0.0f;
	i = 0;
	while (list != NULL)
	{
		tri = (t_vector3_tri *)list->content;
		if (is_in_ramp(cp, tri))
			z = ft_maxf(z, sample_ramp_z(v3tov2(*cp->position), *tri, i % 2));
		i++;
		list = list->next;
	}
	return (z);
}

// get_z_from_areas
// get_z_from_ramps
static float	get_z_floor(t_character_physics *cp, t_world *world)
{
	float	z;

	z = 0.0f;
	z = ft_maxf(z, get_z_from_areas(cp, world));
	z = ft_maxf(z, get_z_from_ramps(cp, world));
	return (z);
}

static float	get_z_ceil(t_character_physics *cp, t_world *world)
{
	t_list	*list;
	t_area	*room;
	int		i;
	float	z;

	z = INFINITY;
	list = world->arealist;
	while (list != NULL)
	{
		room = (t_area *)list->content;
		i = 0;
		while (i < room->ceiling_count)
		{
			if (is_in_ceil(cp, &room->ceilings[i]))
				z = ft_minf(z, (float)(room->height + room->ceiling_height));
			i++;
		}
		list = list->next;
	}
	return (z);
}

t_bound	get_bound(t_character_physics *cp, t_world *world)
{
	t_bound	bound;

	bound.min = get_z_floor(cp, world);
	bound.max = get_z_ceil(cp, world);
	return (bound);
}
