/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 18:03:40 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 14:38:55 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "objects.h"
#include "doomnukem.h"
#include "editor_tools.h"

//TODO: use infinity as default internal_info_distance?
//TODO: Do the other triangle
bool	raycast_plane(t_ray r, t_raycastinfo *info, float plane_z)
{
	t_vector3_tri	tri;
	t_raycastinfo	internal_info;
	bool			hit;

	hit = false;
	ft_bzero(&internal_info, sizeof(t_raycastinfo));
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
bool	raycast_new(t_ray r, t_raycastinfo *info, t_world *world)
{
	int				i;
	int				found;
	t_entitycache	*cache;
	t_raycastinfo	internal_info;
	t_entity		*ent;
	bool			hit;

	hit = false;
	cache = &world->entitycache;
	ft_bzero(&internal_info, sizeof(t_raycastinfo));
	internal_info.distance = 100000.0f;
	r.dir = vector3_normalise(r.dir);
	i = 0;
	found = 0;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free && !ent->hidden)
		{
			if (raycast_entity(r, &internal_info, ent))
				hit = true;
			found++;
		}
		i++;
	}
	if (info != NULL)
		*info = internal_info;
	return (hit);
}
