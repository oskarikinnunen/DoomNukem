/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:57:22 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:58:29 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTING_H
# define LIGHTING_H

# include "render_utils.h"
# include "shapes.h"

# define LIGHT_AMBIENT 0.15f

typedef struct s_map
{
	uint32_t	*texture;
	uint32_t	*lightmap;
	t_point		img_size;
	t_point		size;
}	t_map;

typedef enum e_cubemap_state
{
	cm_default,
	cm_1,
	cm_2,
	cm_3,
	cm_4,
	cm_5,
	cm_6
}	t_cubemap_state;

typedef struct s_cubemap
{
	t_vector2	resolution;
	float		*shadowmaps[6];
	t_camera	cameras[6];
}	t_cubemap;

typedef struct s_light
{
	float			radius;
	t_vector3		origin;
	t_vector3		world_position;
	t_cubemap_state	cm_state;
	t_cubemap		cubemap;
	bool			ignore_self;
	uint32_t		clr;
	float			intensity;
	float			ambient;
}	t_light;

typedef struct s_lighting
{
	struct s_world		*world;
	t_world_triangle	*world_triangles;
	t_light				*light;
	struct s_entity		*entity;
	struct s_img		*img;
	bool				*overdraw;
	float				*zbuffer;
	t_map				*map;
	t_vector2			resolution;
	t_v2_rectangle		screen_edge;
	t_camera			camera;
	t_screen_triangle	triangle;
	t_vector3			triangle_normal;
}	t_lighting;

t_v2_rectangle	get_bounds(t_screen_triangle *triangle);
t_vector2		get_barycentric_offset(t_vector2 p[3]);
void			rasterize_texture(t_screen_triangle triangle, \
									t_lighting *lighting);

#endif