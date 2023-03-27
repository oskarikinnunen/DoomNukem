/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:55:22 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:56:15 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NAVIGATION_H
# define NAVIGATION_H

# include "vectors.h"

# define NAV_CLIP_SIZE 250.0f

typedef struct s_nav_node
{
	t_vector3	vertex[3];
	t_vector3	mid_point;
	t_vector3	enter_point;
	t_vector3	line_point[100];
	uint32_t	neighbors;
	uint32_t	neighbors_id[100];
	uint32_t	parent;
	float		f;
	float		g;
	float		h;
	bool		valid;
	bool		visited;
	bool		blocked;
	uint32_t	index;
}	t_nav_node;

typedef struct s_path
{
	bool		valid_path;
	t_nav_node	path[32];
	uint32_t	ai;
	uint32_t	bi;
	uint32_t	start;
	uint32_t	end;
}	t_path;

typedef struct s_navigation
{
	uint32_t	node_amount;
	t_nav_node	*navmesh;
	t_nav_node	*openlist;
	uint32_t	malloc_size;
	bool		show_navmesh;
}	t_navigation;

typedef struct s_triangle_border_share
{
	t_navigation	*nav;
	int				i;
	int				i1;
	t_vector3		start;
	t_vector3		end;

}	t_triangle_border_share;

typedef struct s_navmesh_info
{
	int	start;
	int	end;
	int	size;
	int	index;
}	t_navmesh_info;

int	world_triangle_get_max_dist_index(t_world_triangle t);

#endif