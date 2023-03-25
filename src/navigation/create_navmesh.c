/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_navmesh.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:35:57 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/25 18:31:30 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	get_max_dist_index(t_world_triangle t)
{
	float	dist;
	int		index;
	int		i;

	index = 0;
	dist = vector3_dist(t.p[0].v, t.p[1].v);
	i = 1;
	while (i < 3)
	{
		if (dist < vector3_dist(t.p[i].v, t.p[(i + 1) % 3].v))
		{
			dist = vector3_dist(t.p[i].v, t.p[(i + 1) % 3].v);
			index = i;
		}
		i++;
	}
	return (index);
}

static t_world_triangle	*add_to_cliplist(t_world_triangle *arr, int start, int *end, int *size, int index)
{
	t_vector3	plane_p;
	t_vector3	plane_n;

	plane_p = vector3_lerp(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v, 0.5f);
	plane_n = vector3_normalise(vector3_sub(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v));
	if (*end + 4 >= *size)
	{
		arr = ft_realloc(arr, (*size + 1000) * sizeof(t_world_triangle), *size * sizeof(t_world_triangle));
		if (!arr)
			exit(0);
		*size += 1000;
	}
	*end += clip_triangle_plane(plane_p, plane_n, arr[start], &(arr[*end]));
	plane_n = vector3_negative(plane_n);
	*end += clip_triangle_plane(plane_p, plane_n, arr[start], &(arr[*end]));
	return(arr);
}

static void	add_to_navmesh(t_navigation *nav, t_world_triangle t, t_world *world)
{
	int i;
	t_vector3	midpoint;

	i = 0;
	while (i < 3)
	{
		nav->navmesh[nav->node_amount].vertex[i] = t.p[i].v;
		i++;
	}
	midpoint = vector3_div(vector3_add(vector3_add(nav->navmesh[nav->node_amount].vertex[0], nav->navmesh[nav->node_amount].vertex[1]), nav->navmesh[nav->node_amount].vertex[2]), 3.0f);
	if (!is_triangle_degenerate(nav->navmesh[nav->node_amount].vertex))
	{
		if (nav->node_amount + 1 == nav->malloc_size)
		{
			nav->navmesh = ft_realloc(nav->navmesh, (nav->malloc_size + 1000) * sizeof(t_navnode), nav->malloc_size * sizeof(t_navnode));
			nav->malloc_size += 1000;
			if (!nav->navmesh)
				doomlog(LOG_EC_MALLOC, NULL);
		}
		nav->navmesh[nav->node_amount].mid_point = vector3_div(vector3_add(vector3_add(nav->navmesh[nav->node_amount].vertex[0], nav->navmesh[nav->node_amount].vertex[1]), nav->navmesh[nav->node_amount].vertex[2]), 3.0f);
		nav->navmesh[nav->node_amount].index = nav->node_amount;
		nav->node_amount++;
	}
}

static void	clip_navmesh(t_navigation *nav, t_entity *ent, t_world *world)
{
	t_world_triangle	*arr;
	int			size;
	int			start = 0;
	int			end = 1;
	float		dist;
	int			index;

	size = 1000;
	arr = prot_memalloc(sizeof(t_world_triangle) * size);
	arr[0].p[0].v = ent->obj->vertices[0];
	arr[0].p[1].v = ent->obj->vertices[1];
	arr[0].p[2].v = ent->obj->vertices[2];
	while(start < end)
	{
		index = get_max_dist_index(arr[start]);
		dist = vector3_dist(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v);
		if (dist < nav->clip_size)
			add_to_navmesh(nav, arr[start], world);
		else
			arr = add_to_cliplist(arr, start, &end, &size, index);
		start++;
	}
	free(arr);
}

//TODO: change to use world->roomlist floors
void	malloc_space_for_navmesh(t_world *world)
{
	int			i;
	int			found;
	t_entity	*ent;

	if (world->nav.navmesh)
		free(world->nav.navmesh);
	world->nav.malloc_size = 1000;
	world->nav.navmesh = prot_memalloc(sizeof(t_navnode) * world->nav.malloc_size);
	bzero(world->nav.navmesh, sizeof(t_navnode) * world->nav.malloc_size);
	i = 0;
	found = 0;
	world->nav.node_amount = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
				if (ent->occlusion.world_tri_count == 1)
					clip_navmesh(&world->nav, ent, world);
			found++;
		}
		i++;
	}
}
