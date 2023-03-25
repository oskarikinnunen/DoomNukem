/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_navmesh.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:47:25 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/25 18:31:35 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "movement_defs.h"

static bool	is_already_neighbor(t_navigation *nav, int i, int nb)
{
	int	index;

	index = 0;
	while (index < nav->navmesh[i].neighbors)
	{
		if (nav->navmesh[i].neighbors_id[index] == nb)
			return (true);
		index++;
	}
	return (false);
}

//TODO: modify this in a way that allows higher nodes to connect to lower ones
static int	line_laps_line(t_vector3 start1, t_vector3 end1, t_vector3 p)
{
	float len;
	float dist[2];

	if (vector2_cmp_epsilon(v3tov2(p), v3tov2(start1), 0.001f) || vector2_cmp_epsilon(v3tov2(p), v3tov2(end1), 0.001f))
	{
		if (float_cmp_epsilon(p.z, start1.z, COL_STEP) || float_cmp_epsilon(p.z, end1.z, COL_STEP))
			return(1);
		return(0);
	}
	len = vector2_dist(v3tov2(start1), v3tov2(end1));
	dist[0] = vector2_dist(v3tov2(p), v3tov2(start1));
	dist[1] = vector2_dist(v3tov2(p), v3tov2(end1));
	if (dist[0] + dist[1] <= len + 0.0001f && fabsf(vector3_fdist_to_plane(p, vector3_up(), start1)) <= COL_STEP) //0.00001f
		return(2);
	return(0);
}

static bool	line_overlaps_parallel_line(t_vector3 start1, t_vector3 end1, t_vector3 start2, t_vector3 end2)
{
	int	count;

	count = 0;
	count += line_laps_line(start1, end1, start2);
	count += line_laps_line(start1, end1, end2);
	count += line_laps_line(start2, end2, start1);
	count += line_laps_line(start2, end2, end1);
	if (count >= 3)
		return(true);
	return(false);
}

static bool	triangles_share_border(t_navigation *nav, int i, int i1, t_vector3 start, t_vector3 end)
{
	int	j;

	if (vector3_dist(nav->navmesh[i].mid_point,  nav->navmesh[i1].mid_point) < 1000000.0f)
	{
		j = 0;
		while (j < 3)
		{
			if (line_overlaps_parallel_line(start, end, nav->navmesh[i1].vertex[j], nav->navmesh[i1].vertex[(j + 1) % 3]))
				return(true);
			j++;
		}
	}
	return(false);
}

void	check_neighbors(t_navigation *nav, int i, t_vector3 start, t_vector3 end)
{
	int	i1;

	i1 = 0;
	while (i1 < nav->node_amount)
	{
		if (i == i1 && i1++)
			continue;
		if (is_already_neighbor(nav, i, i1) && i1++)
			continue;
		if (triangles_share_border(nav, i, i1, start, end))
		{
			nav->navmesh[i].line_point[nav->navmesh[i].neighbors] = vector3_lerp(start, end, 0.5f);
			nav->navmesh[i].neighbors_id[nav->navmesh[i].neighbors++] = i1;
		}
		i1++;
	}
}
