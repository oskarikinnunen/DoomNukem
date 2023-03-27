/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fragment_light.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:00:33 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:29:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int	get_cubemap_face(t_vector3 dir)
{
	float	face;
	float	temp;

	face = 0;
	temp = dir.x;
	if (fabsf(dir.y) > fabsf(temp))
	{
		temp = dir.y;
		face = 1;
	}
	if (fabsf(dir.z) > fabsf(temp))
	{
		temp = dir.z;
		face = 2;
	}
	if (temp < 0.0f)
		face += 3;
	return (face);
}

static t_quaternion	location_to_screenspace(
	t_quaternion q, t_camera camera, t_vector2 res)
{
	const t_vector3	voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

	q = quaternion_mul_matrix(camera.mat_view, q);
	q = quaternion_mul_matrix(camera.mat_proj, q);
	q.v = vector3_div(q.v, q.w);
	q.v = vector3_negative(q.v);
	q.v = vector3_add(q.v, voffsetview);
	q.v.x = q.v.x * (res.x * 0.5f);
	q.v.y = q.v.y * (res.y * 0.5f);
	return (q);
}

static bool	sample_camera_buffer(
	t_lighting *l, t_quaternion q, float *dist, float angle)
{
	t_point	p;
	int		index;

	p.x = ft_clamp(q.v.x, 0, l->light->cubemap.resolution.x - 1.0f);
	p.y = ft_clamp(q.v.y, 0, l->light->cubemap.resolution.y - 1.0f);
	index = p.y * l->light->cubemap.resolution.x + p.x;
	if (1.0f / q.w >= l->zbuffer[index] * 0.98f)
	{
		*dist = ft_clampf((1.0f - (*dist / l->light->radius)), 0.0f, 1.0f);
		*dist = angle * (*dist) * \
		l->light->intensity + l->light->ambient * (*dist);
		*dist = ft_clampf(*dist, 0.0f, 1.0f);
		return (true);
	}
	return (false);
}

uint32_t	get_lighting_for_pixel(
	t_lighting *l, uint32_t light_amount, t_vector3 pixel_loc)
{
	int				face;
	t_quaternion	q;
	float			dist;
	t_vector3		dir;

	l->light = l->entity->component.data;
	dist = vector3_dist(l->light->world_position, pixel_loc);
	if (dist > l->light->radius)
		return (light_amount);
	dir = vector3_sub(pixel_loc, l->light->world_position);
	dir = vector3_normalise(dir);
	face = get_cubemap_face(dir);
	q = location_to_screenspace(vector3_to_quaternion(pixel_loc), \
	l->light->cubemap.cameras[face], l->light->cubemap.resolution);
	l->zbuffer = l->light->cubemap.shadowmaps[face];
	if (sample_camera_buffer(l, q, &dist, \
	fabsf(vector3_dot(dir, l->triangle_normal))))
		light_amount = get_light_amount(dist, l->light->clr, light_amount);
	return (light_amount);
}
