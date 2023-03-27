/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:44:01 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 17:09:41 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OCCLUSION_H
# define OCCLUSION_H

# include "vectors.h"
# include "shapes.h"

//TODO: Rename this and change the t_bound to t_heigh or smt
typedef struct s_ent_bounds
{
	t_vector3		origin;
	float			radius;
	t_box			box;
}	t_ent_bounds;

typedef struct s_occlusion
{
	t_world_triangle	world_tri[12];
	uint32_t			world_tri_count;
	t_ent_bounds		bounds;
	t_square			box;
	t_square			clip;
	float				z_dist[2];
	bool				is_backface_cull;
	bool				is_occluded;
	bool				is_frustrum_culled;
}	t_occlusion;

typedef struct s_bitmask_helper
{
	float			wstep;
	int				chunk;
	int				end_chunk;
	int				wchunk;
	int				row;
	int				x;
	uint16_t		res;
}	t_bitmask_helper;

typedef struct s_tile
{
	float		max1;
	float		max0;
	uint64_t	mask;
}	t_tile;

//bitmask_chunks: amount of bitmask chunks in x and y axis
//tile_chunks: amount of tile chunks in x and y axis
typedef struct s_bitmask
{
	t_point	bitmask_chunks;
	t_point	tile_chunks;
	t_tile	*tile;
	float	max_dist;
	float	last_max_dist;
}	t_bitmask;

uint16_t	mask_x(int x, int left_x, int right_x);

#endif