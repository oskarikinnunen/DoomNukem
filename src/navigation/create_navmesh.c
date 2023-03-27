/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_navmesh.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 17:17:11 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 18:50:02 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

static t_world_triangle	*add_to_clip_list(t_navmesh_info *info,
											t_world_triangle *arr)
{
	t_vector3	plane_p;
	t_vector3	plane_n;

	plane_p = vector3_lerp(arr[info->start].p[info->index].v, \
				arr[info->start].p[(info->index + 1) % 3].v, 0.5f);
	plane_n = vector3_normalise(\
				vector3_sub(arr[info->start].p[info->index].v, \
				arr[info->start].p[(info->index + 1) % 3].v));
	if (info->end + 4 >= info->size)
	{
		arr = ft_realloc(arr, \
					(info->size + 1000) * sizeof(t_world_triangle), \
					info->size * sizeof(t_world_triangle));
		if (!arr)
			doomlog(LOG_EC_MALLOC, "add_to_cliplist");
		info->size += 1000;
	}
	info->end += clip_triangle_plane(plane_p, plane_n, \
			arr[info->start], &(arr[info->end]));
	plane_n = vector3_negative(plane_n);
	info->end += clip_triangle_plane(plane_p, plane_n, \
			arr[info->start], &(arr[info->end]));
	return (arr);
}

// TODO: Name better
static void	add_to_navmesh_helper(t_navigation *nav)
{
	if (nav->node_amount + 1 == nav->malloc_size)
	{
		nav->navmesh = ft_realloc(nav->navmesh, \
						(nav->malloc_size + 1000) * sizeof(t_nav_node), \
						nav->malloc_size * sizeof(t_nav_node));
		if (nav->navmesh == NULL)
			doomlog(LOG_EC_MALLOC, "add_to_navmesh");
		nav->malloc_size += 1000;
		if (!nav->navmesh)
			doomlog(LOG_EC_MALLOC, NULL);
	}
	nav->navmesh[nav->node_amount].mid_point = vector3_div(vector3_add(\
			vector3_add(nav->navmesh[nav->node_amount].vertex[0], \
			nav->navmesh[nav->node_amount].vertex[1]), \
			nav->navmesh[nav->node_amount].vertex[2]), 3.0f);
	nav->navmesh[nav->node_amount].index = nav->node_amount;
	nav->node_amount++;
}

static void	add_to_navmesh(t_navigation *nav, t_world_triangle t)
{
	int			i;
	t_vector3	midpoint;

	i = 0;
	while (i < 3)
	{
		nav->navmesh[nav->node_amount].vertex[i] = t.p[i].v;
		i++;
	}
	midpoint = vector3_div(vector3_add(vector3_add(\
				nav->navmesh[nav->node_amount].vertex[0], \
				nav->navmesh[nav->node_amount].vertex[1]), \
				nav->navmesh[nav->node_amount].vertex[2]), 3.0f);
	if (!is_triangle_degenerate(nav->navmesh[nav->node_amount].vertex))
		add_to_navmesh_helper(nav);
}

static void	clip_navmesh(t_navigation *nav, t_entity *ent, t_world *world)
{
	t_world_triangle	*arr;
	t_navmesh_info		info;
	float				dist;

	info.start = 0;
	info.end = 1;
	info.size = 1000;
	arr = prot_memalloc(sizeof(t_world_triangle) * info.size);
	arr[0].p[0].v = ent->obj->vertices[0];
	arr[0].p[1].v = ent->obj->vertices[1];
	arr[0].p[2].v = ent->obj->vertices[2];
	while (info.start < info.end)
	{
		info.index = world_triangle_get_max_dist_index(arr[info.start]);
		dist = vector3_dist(\
				arr[info.start].p[info.index].v, \
				arr[info.start].p[(info.index + 1) % 3].v);
		if (dist < NAV_CLIP_SIZE)
			add_to_navmesh(nav, arr[info.start]);
		else
			arr = add_to_clip_list(&info, arr);
		info.start++;
	}
	free(arr);
}

void	create_navmesh(t_world *world)
{
	int			i;
	int			found;
	t_entity	*ent;

	if (world->nav.navmesh)
		free(world->nav.navmesh);
	world->nav.malloc_size = 1000;
	world->nav.navmesh = prot_memalloc(\
	sizeof(t_nav_node) * world->nav.malloc_size);
	ft_bzero(world->nav.navmesh, sizeof(t_nav_node) * world->nav.malloc_size);
	i = 0;
	found = 0;
	world->nav.node_amount = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
				if (ent->occlusion.world_tri_count == 1 && ent->map != NULL)
					clip_navmesh(&world->nav, ent, world);
			found++;
		}
		i++;
	}
}
