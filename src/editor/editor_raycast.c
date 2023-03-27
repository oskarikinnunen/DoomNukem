/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 18:03:40 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:57:38 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

//TODO: use infinity as default internal_info_distance?
//TODO: Do the other triangle
bool	raycast_plane(t_ray r, t_raycast_info *info, float plane_z)
{
	t_vector3_tri	tri;
	t_raycast_info	internal_info;
	bool			hit;

	hit = false;
	ft_bzero(&internal_info, sizeof(t_raycast_info));
	internal_info.distance = 100000.0f;
	r.dir = vector3_normalise(r.dir);
	tri.a = (t_vector3){.z = plane_z};
	tri.b = (t_vector3){50000.0f, 0.0f, plane_z};
	tri.c = (t_vector3){0.0f, 50000.0f, plane_z};
	if (raycast_tri(r, tri, &internal_info.distance))
	{
		internal_info.hit_pos = vector3_add(r.origin,
				vector3_mul(r.dir, internal_info.distance));
		hit = true;
	}
	if (info != NULL)
		*info = internal_info;
	return (hit);
}

//TODO: f_Infinity
bool	raycast(t_ray r, t_raycast_info *info, t_world *world)
{
	t_entitycache		*cache;
	t_editor_raycast	erc;

	cache = &world->entitycache;
	ft_bzero(&erc, sizeof(t_editor_raycast));
	erc.internal_info.distance = 100000.0f;
	r.dir = vector3_normalise(r.dir);
	while (erc.found < cache->existing_entitycount && \
			erc.i < cache->alloc_count)
	{
		erc.ent = &cache->entities[erc.i];
		if (erc.ent->status != es_free && !erc.ent->hidden)
		{
			if (raycast_entity(r, &erc.internal_info, erc.ent))
				erc.hit = true;
			erc.found++;
		}
		erc.i++;
	}
	if (info != NULL)
		*info = erc.internal_info;
	return (erc.hit);
}
