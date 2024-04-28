/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:19:19 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:30:49 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

uint32_t	update_pixel_brightness(uint32_t light, uint32_t clr)
{
	uint32_t	alpha;
	uint32_t	red;
	uint32_t	green;
	uint32_t	blue;

	alpha = clr & 0xFF000000;
	red = ((clr & 0x00FF0000) * (uint8_t)((light & 0xFF0000) >> 16)) >> 8;
	green = ((clr & 0x0000FF00) * (uint8_t)((light & 0xFF00) >> 8)) >> 8;
	blue = ((clr & 0x000000FF) * (uint8_t)((light & 0xFF))) >> 8;
	clr = \
	alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF);
	return (clr);
}

inline t_step	make_slope(float start, float end, float steps)
{
	t_step	temp;

	temp.location = start;
	temp.step = (end - start) / steps;
	return (temp);
}

inline t_stepv3	make_uv_slopev3(
	int start, int end, int y, t_screen_triangle triangle)
{
	t_stepv3	slope;
	t_vector2	left;
	t_vector2	right;

	left = barycentric_coordinates(triangle.p, (t_vector2){start, y});
	right = barycentric_coordinates(triangle.p, (t_vector2){end, y});
	slope.location = get_vector3_from_barycentric(triangle.t, left);
	slope.step = get_vector3_from_barycentric(triangle.t, right);
	slope.step = \
	vector3_div(vector3_sub(slope.step, slope.location), end - start);
	return (slope);
}

t_vector3	texcoord_to_loc(t_vector3 v[3], t_vector2 uv[3], t_vector2 p)
{
	t_vector2	bary;
	t_vector3	loc;
	t_vector3	w1;
	t_vector3	w2;

	bary = barycentric_coordinates(uv, p);
	w1 = vector3_lerp(v[0], v[1], bary.x);
	w2 = vector3_mul(vector3_sub(v[2], v[0]), bary.y);
	loc = vector3_add(w1, w2);
	return (loc);
}

uint32_t	get_light_amount(float delta, uint32_t clr, uint32_t light_amount)
{
	uint8_t	channel1;
	uint8_t	channel2;
	uint8_t	channel3;
	uint8_t	channel4;

	channel1 = ft_clamp((clr & 0xFF) * delta + (light_amount & 0xFF), 0, 255);
	channel2 = ft_clamp(((clr >> 8) & 0xFF) * \
	delta + ((light_amount >> 8) & 0xFF), 0, 255);
	channel3 = ft_clamp(((clr >> 16) & 0xFF) * \
	delta + ((light_amount >> 16) & 0xFF), 0, 255);
	channel4 = ft_clamp(((clr >> 24) & 0xFF) + \
	((light_amount >> 24) & 0xFF), 0, 255);
	light_amount = \
	channel1 | (channel2 << 8) | (channel3 << 16) | (channel4 << 24);
	return (light_amount);
}
