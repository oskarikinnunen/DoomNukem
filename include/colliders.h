/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colliders.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:24:46 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:24:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLIDERS_H
# define COLLIDERS_H

typedef enum e_bound_type
{
	bt_ignore,
	bt_box,
	bt_plane
}	t_bound_type;

typedef struct s_limb_collider
{
	t_vector3	*start_vertex;
	t_vector3	*end_vertex;
}	t_limb_collider;

typedef struct s_box_collider
{
	t_vector3	*vertex;
	t_vector3	offset;
	t_vector3	size;
}	t_box_collider;

typedef struct s_human_colliders
{
	t_limb_collider	head;
	t_limb_collider	body;
	t_limb_collider	arms[2];
	t_limb_collider	legs[2];
}	t_human_colliders;

typedef struct s_colliders
{
	t_limb_collider	limb_colliders[4];
	uint32_t		limb_col_count;
	t_box_collider	box_colliders[8];
	uint32_t		box_col_count;
}	t_colliders;

typedef struct s_object_meta_data
{
	char		obj_name[64];
	t_colliders	col;
}	t_object_meta_data;

#endif