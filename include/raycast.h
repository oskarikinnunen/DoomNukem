/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:43:35 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 22:47:57 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_H
# define RAYCAST_H

# include "vectors.h"
# include "collision.h"

struct	s_entity;
struct	s_world;

typedef struct s_raycast_info
{
	struct s_entity	*hit_entity;
	t_vector3		hit_pos;
	t_vector3		face_normal;
	float			distance;
}	t_raycast_info;

bool	raycast(t_ray r, t_raycast_info *info, struct s_world *world);
bool	raycast_plane(t_ray r, t_raycast_info *info, float plane_z);
bool	raycast_entity(t_ray r, t_raycast_info *info, struct s_entity *entity);
bool	raycast_tri(t_ray r, t_vector3_tri tri, float *dist);

#endif