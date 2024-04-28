/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:00:06 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/24 14:56:27 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_screen_triangle	ps1(t_screen_triangle in, int div)
{
	t_screen_triangle	res;

	res = in;
	res.p[0] = vector2_div(res.p[0], div);
	res.p[1] = vector2_div(res.p[1], div);
	res.p[2] = vector2_div(res.p[2], div);
	res.p[0] = vector2_mul(res.p[0], div);
	res.p[1] = vector2_mul(res.p[1], div);
	res.p[2] = vector2_mul(res.p[2], div);
	return (res);
}

t_screen_triangle	wf_tri(t_screen_triangle in, float scaling)
{
	t_screen_triangle	res;

	res = in;
	res.p[0] = vector2_div(in.p[0], scaling);
	res.p[1] = vector2_div(in.p[1], scaling);
	res.p[2] = vector2_div(in.p[2], scaling);
	return (res);
}

inline static int	flat_tri(t_vector2 p[3], t_vector3 t[3])
{
	if (p[1].y == p[2].y)
	{
		if (p[1].x > p[2].x)
		{
			ft_swap(&p[1], &p[2], sizeof(t_vector2));
			ft_swap(&t[1], &t[2], sizeof(t_vector3));
		}
		return (0);
	}
	else if (p[0].y == p[1].y)
	{
		ft_swap(&p[0], &p[2], sizeof(t_vector2));
		ft_swap(&t[0], &t[2], sizeof(t_vector3));
		if (p[1].x > p[2].x)
		{
			ft_swap(&p[1], &p[2], sizeof(t_vector2));
			ft_swap(&t[1], &t[2], sizeof(t_vector3));
		}
		return (1);
	}
	return (2);
}

inline static void	swap_vector2_and_vector3(
	t_vector3 *v3_a, t_vector3 *v3_b, t_vector2 *v2_a, t_vector2 *v2_b)
{
	ft_swap(v2_a, v2_b, sizeof(t_vector2));
	ft_swap(v3_a, v3_b, sizeof(t_vector3));
}

inline int	triangle_to_flat(t_screen_triangle t, t_screen_triangle b[2])
{
	t_vector2	p_split;
	t_vector3	t_split;
	int			res;
	float		delta;

	sort_vector2_vector3_by_vector2_height(t.p, t.t);
	res = flat_tri(t.p, t.t);
	if (res != 2)
	{
		b[0] = t;
		return (res);
	}
	delta = (t.p[1].y - t.p[2].y) / (t.p[0].y - t.p[2].y);
	p_split = (t_vector2){ft_flerp(t.p[2].x, t.p[0].x, delta), t.p[1].y};
	t_split = vector3_lerp(t.t[2], t.t[0], delta);
	if (p_split.x < t.p[1].x)
		swap_vector2_and_vector3(&t.t[1], &t_split, &t.p[1], &p_split);
	b[1] = t;
	t.p[2] = p_split;
	t.t[2] = t_split;
	b[0] = t;
	swap_vector2_and_vector3(&b[1].t[0], &b[1].t[2], &b[1].p[0], &b[1].p[2]);
	b[1].p[2] = p_split;
	b[1].t[2] = t_split;
	return (res);
}
