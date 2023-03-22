/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fragment_light.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:00:33 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/22 18:07:06 by vlaine           ###   ########.fr       */
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

static t_vector3	get_direction_to_pixel(t_light *light, t_vector3 pixel_loc)
{
	t_vector3	dir;

	dir = vector3_sub(pixel_loc, light->world_position);
	dir = vector3_normalise(dir);
	return (dir);
}

static t_quaternion	location_to_screenspace(
	t_quaternion q, t_camera camera, t_vector2 res)
{
	const t_vector3	voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

	q = quaternion_mul_matrix(camera.matview, q);
	q = quaternion_mul_matrix(camera.matproj, q);
	q.v = vector3_div(q.v, q.w);
	q.v = vector3_negative(q.v);
	q.v = vector3_add(q.v, voffsetview);
	q.v.x = q.v.x * (res.x * 0.5f);
	q.v.y = q.v.y * (res.y * 0.5f);
	return (q);
}



static bool	sample_camera_buffer(t_lighting *l, t_quaternion q, float *dist, float angle)
{
	t_point	p;

	p.x = ft_clamp(q.v.x, 0, l->light->cubemap.resolution.x - 1.0f);
	p.y = ft_clamp(q.v.y, 0, l->light->cubemap.resolution.y - 1.0f);
	int test = p.y * l->light->cubemap.resolution.x + p.x;
	if (1.0f / q.w >= l->zbuffer[test] * 0.98f)
	{
		*dist = ft_clampf((1.0f - (*dist / l->light->radius)), 0.0f, 1.0f);
		*dist = angle * (*dist) * l->light->intensity + l->light->ambient * (*dist);
		*dist = ft_clampf(*dist, 0.0f, 1.0f);
		return (true);
	}
	return (false);
}

uint32_t	get_lighting_for_pixel(
	t_lighting *lighting, uint32_t light_amount, t_vector3 pixel_loc)
{
	int				face;
	int				i;
	int				found;
	t_quaternion	q;
	t_light			*light;
	float			dist;

	light = lighting->light_ent->component.data;
	lighting->light = light;
	t_vector3 dir;

	dist = vector3_dist(light->world_position, pixel_loc);
	if (dist > light->radius)
		return(light_amount);
	dir = vector3_sub(pixel_loc, light->world_position);
	dir = vector3_normalise(dir);
	float angle = fabsf(vector3_dot(dir, lighting->triangle_normal));
	face = get_cubemap_face(dir);
	q = location_to_screenspace(vector3_to_quaternion(pixel_loc), light->cubemap.cameras[face], light->cubemap.resolution);
	lighting->zbuffer = light->cubemap.shadowmaps[face];
	if (sample_camera_buffer(lighting, q, &dist, angle))
		light_amount = get_light_amount(dist, light->clr, light_amount);
	return(light_amount);
}

/*
static bool	sample_camera_buffer(t_lighting *l, t_quaternion q, float *dist, float angle)
{
	t_point	p;

	p.x = ft_clamp(q.v.x, 0, l->light->cubemap.resolution.x - 1.0f);
	p.y = ft_clamp(q.v.y, 0, l->light->cubemap.resolution.y - 1.0f);
	int test = p.y * l->light->cubemap.resolution.x + p.x;
	if (1.0f / q.w >= l->zbuffer[test] * 0.98f)
	{
		*dist = ft_clampf((1.0f - (*dist / l->light->radius)), 0.0f, 1.0f);
		*dist = angle * (*dist) * l->light->intensity + l->light->ambient * (*dist);
		*dist = ft_clampf(*dist, 0.0f, 1.0f);
		return (true);
	}
	return (false);
}

uint32_t	get_lighting_for_pixel(
	t_lighting *lighting, uint32_t light_amount, t_vector3 pixel_loc)
{
	int				face;
	int				i;
	int				found;
	t_quaternion	q;
	t_light			*light;
	float			dist;

	light = lighting->light_ent->component.data;
	lighting->light = light;
	t_vector3 dir;

	dist = vector3_dist(light->world_position, pixel_loc);
	if (dist > light->radius)
		return(light_amount);
	dir = vector3_sub(pixel_loc, light->world_position);
	dir = vector3_normalise(dir);
	float angle = fabsf(vector3_dot(dir, lighting->triangle_normal));
	face = get_cubemap_face(dir);
	q = location_to_screenspace(vector3_to_quaternion(pixel_loc), light->cubemap.cameras[face], light->cubemap.resolution);
	lighting->zbuffer = light->cubemap.shadowmaps[face];
	if (sample_camera_buffer(lighting, q, &dist, angle))
		light_amount = get_light_amount(dist, light->clr, light_amount);
	return(light_amount);
}
*/