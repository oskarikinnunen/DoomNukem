/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfind.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 22:16:15 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 15:20:53 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

uint32_t	get_nearest_target_node(t_world *world, t_vector3 target)
{
	int		e;
	int		closest_point;
	float	dist;

	e = 0;
	closest_point = 0;
	dist = vector3_dist(world->nav.navmesh[0].mid_point, target);
	while (e < world->nav.node_amount)
	{
		if (dist > vector3_dist(world->nav.navmesh[e].mid_point, target))
		{
			dist = vector3_dist(world->nav.navmesh[e].mid_point, target);
			closest_point = e;
		}
		e++;
	}
	return (closest_point);
}

static uint32_t	add_valid_neighbours(t_world *world, t_nav_node *openlist,
										uint32_t lowest_f, uint32_t end)
{
	int			i;
	int			count;
	uint32_t	id;

	count = 0;
	i = 0;
	while (i < openlist[lowest_f].neighbors)
	{
		id = openlist[lowest_f].neighbors_id[i];
		if (openlist[id].visited == false && \
			openlist[id].blocked == false && openlist[id].valid == false)
		{
			openlist[id] = world->nav.navmesh[id];
			openlist[id].g = openlist[lowest_f].g + 1.0f;
			openlist[id].h = vector3_dist(openlist[id].mid_point, \
								world->nav.navmesh[end].mid_point);
			openlist[id].f = openlist[id].g + openlist[id].h;
			openlist[id].valid = true;
			openlist[id].parent = lowest_f;
			openlist[id].enter_point = openlist[lowest_f].line_point[i];
			count++;
		}
		i++;
	}
	return (count);
}

static t_path	construct_path(int end, int start,
								t_nav_node *openlist, t_path path)
{
	int	i;

	i = 0;
	path.path[i] = openlist[start];
	while (start != end && i < 32)
	{
		path.path[i] = openlist[start];
		start = openlist[start].parent;
		i++;
	}
	if (i != 32)
	{
		path.path[i] = openlist[start];
		path.path[i].enter_point = path.path[i].mid_point;
	}
	else
		path.path[i - 1].enter_point = path.path[i - 1].mid_point;
	path.ai = 0;
	path.bi = i;
	path.valid_path = true;
	return (path);
}

static uint32_t	get_best_node_id(t_nav_node *openlist,
								uint32_t lowest_f_index, uint32_t list_len)
{
	uint32_t	found;
	uint32_t	i;

	found = 0;
	i = 0;
	while (found < list_len)
	{
		if (openlist[i].valid == true)
		{
			if (found == 0 || openlist[i].f < openlist[lowest_f_index].f)
				lowest_f_index = i;
			found++;
		}
		i++;
	}
	return (lowest_f_index);
}

t_path	pathfind(t_vector3 start_vec, t_vector3 end_vec, t_world *world)
{
	t_nav_node	*openlist;
	uint32_t	list_len;
	uint32_t	i_f;
	t_path		path;

	openlist = world->nav.openlist;
	ft_bzero(&path, sizeof(t_path));
	ft_bzero(openlist, sizeof(t_nav_node) * world->nav.node_amount);
	path.start = get_nearest_target_node(world, start_vec);
	path.end = get_nearest_target_node(world, end_vec);
	openlist[path.end] = world->nav.navmesh[path.end];
	openlist[path.end].valid = true;
	openlist[path.end].enter_point = openlist[path.end].mid_point;
	list_len = 1;
	while (list_len > 0)
	{
		i_f = get_best_node_id(openlist, i_f, list_len);
		if (i_f == path.start)
			return (construct_path(path.end, path.start, openlist, path));
		openlist[i_f].valid = false;
		openlist[i_f].visited = true;
		list_len += add_valid_neighbours(world, openlist, i_f, path.start) - 1;
	}
	path.valid_path = false;
	return (path);
}
