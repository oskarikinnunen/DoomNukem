/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fragment_light.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:00:33 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/15 16:46:47 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static int  get_cubemap_face(t_vector3 dir)
{
    float   face;
    float   temp;

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
    return(face);
}

static t_vector3 get_direction_to_pixel(t_light *light, t_vector3 pixel_loc)
{
    t_vector3	dir;

    dir = vector3_sub(pixel_loc, light->world_position);
    dir = vector3_normalise(dir);
    return(dir);
}

static t_quaternion location_to_screenspace(t_quaternion q, t_camera camera, t_vector2 res)
{
    t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
    
    q = quaternion_mul_matrix(camera.matview, q);
    q = quaternion_mul_matrix(camera.matproj, q);
    q.v = vector3_div(q.v, q.w);
    q.v = vector3_negative(q.v);
    q.v = vector3_add(q.v, voffsetview);
    q.v.x = q.v.x * (res.x * 0.5f);
    q.v.y = q.v.y * (res.y * 0.5f);
	return(q);
}

uint32_t	get_light_amount(float delta, uint32_t clr, uint32_t light_amount)
{
	uint8_t channel1;
	uint8_t channel2;
	uint8_t channel3;
	uint8_t channel4;

	channel1 = ft_clamp((clr & 0xFF) * delta + (light_amount & 0xFF), 0, 255);
	channel2 = ft_clamp(((clr >> 8) & 0xFF) * delta + ((light_amount >> 8) & 0xFF), 0, 255);
	channel3 = ft_clamp(((clr >> 16) & 0xFF) * delta + ((light_amount >> 16) & 0xFF), 0, 255);
	channel4 = ft_clamp(((clr >> 24) & 0xFF) + ((light_amount >> 24) & 0xFF), 0, 255);

	light_amount = channel1 | (channel2 << 8) | (channel3 << 16) | (channel4 << 24);
	return(light_amount);
}

uint32_t	get_lighting_for_pixel(t_lighting *lighting, uint32_t light_amount, t_vector3 pixel_loc)
{
    int             j;
    int				i;
	int				found;
    t_quaternion    q;
    t_light         *light;
	float			dist;

    light = lighting->light_ent->component.data;
	t_vector3 dir;

	dist = vector3_dist(light->world_position, pixel_loc);
	if (dist > light->radius)
		return(light_amount);
    dir = vector3_sub(pixel_loc, light->world_position);
    dir = vector3_normalise(dir);
	float temp = fabsf(vector3_dot(dir, lighting->triangle_normal));
    j = get_cubemap_face(dir);
    q = location_to_screenspace(vector3_to_quaternion(pixel_loc), light->cubemap.cameras[j], light->cubemap.resolution);
    t_point p;
	p.x = ft_clamp(q.v.x, 0, light->cubemap.resolution.x - 1.0f);
    p.y = ft_clamp(q.v.y, 0, light->cubemap.resolution.y - 1.0f);
	int test = p.y * light->cubemap.resolution.x + p.x;
    if (1.0f / q.w >= light->cubemap.shadowmaps[j][test] * 0.98f)
	{
		dist = ft_clampf((1.0f - (dist / light->radius)), 0.0f, 1.0f);
		dist = temp * dist * light->intensity + light->ambient * dist;
		light_amount = get_light_amount(ft_clampf(dist, 0.0f, 1.0f), light->clr, light_amount);
	}
    return(light_amount);
}
