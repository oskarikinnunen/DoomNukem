/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_triangle_wrap.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 15:59:32 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 13:30:11 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

inline static uint32_t	sample_img(
	t_render *render, uint32_t xsample, uint32_t ysample)
{
	xsample = xsample % (render->img->size.x);
	ysample = ysample % (render->img->size.y);
	return ((render->img->data[ysample * render->img->size.x + xsample]));
}

inline static void	scanline(
	int start, int end, int y, t_sdlcontext *sdl)
{
	t_vector3	tex;
	t_stepv3	slope;
	int			x;
	float		index;

	slope = make_uv_slopev3(start, end, y, sdl->render.triangle);
	x = start;
	index = 0.5f;
	while (x < end)
	{
		tex.z = slope.location.z + index * slope.step.z;
		if (tex.z > sdl->zbuffer[x + y * sdl->window_w])
		{
			tex.x = slope.location.x + index * slope.step.x;
			tex.y = slope.location.y + index * slope.step.y;
			sdl->zbuffer[x + y * sdl->window_w] = tex.z;
			((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] = \
				sample_img(&sdl->render, tex.x / tex.z, tex.y / tex.z);
		}
		index += 1.0f;
		x++;
	}
	render_bitmask_row((t_point){start, end}, \
	(t_vector2){1.0f / slope.location.z, 1.0f / tex.z}, y, sdl);
}

inline static void	render_flat_top_tri(
	t_sdlcontext *sdl, t_screen_triangle t)
{
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	steps = t.p[0].y - t.p[1].y;
	left = make_slope(t.p[1].x, t.p[0].x, steps);
	right = make_slope(t.p[2].x, t.p[0].x, steps);
	y = ceilf(t.p[2].y - 0.5f);
	endy = ceilf(t.p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - t.p[1].y) + t.p[1].x;
		right.location = right.step * ((float)y + 0.5f - t.p[2].y) + t.p[2].x;
		scanline(
			ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, sdl);
		y++;
	}
}

inline static void	render_flat_bot_tri(
	t_sdlcontext *sdl, t_screen_triangle t)
{
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	steps = t.p[1].y - t.p[0].y;
	left = make_slope(t.p[0].x, t.p[1].x, steps);
	right = make_slope(t.p[0].x, t.p[2].x, steps);
	y = ceilf(t.p[0].y - 0.5f);
	endy = ceilf(t.p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - t.p[0].y) + t.p[0].x;
		right.location = right.step * ((float)y + 0.5f - t.p[0].y) + t.p[0].x;
		scanline(
			ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, sdl);
		y++;
	}
}

void	render_triangle_unlit(
	t_sdlcontext *sdl, t_render *render, int index)
{
	t_screen_triangle	triangle;
	t_screen_triangle	tris[2];
	int					res;

	triangle = render->screenspace_ptris[index];
	if (sdl->ps1_tri_div > 1)
		triangle = ps1(triangle, sdl->ps1_tri_div);
	res = triangle_to_flat(triangle, tris);
	render->triangle = tris[0];
	if (res == 0)
		render_flat_top_tri(sdl, tris[0]);
	else if (res == 1)
		render_flat_bot_tri(sdl, tris[0]);
	else
	{
		render_flat_top_tri(sdl, tris[0]);
		render->triangle = tris[1];
		render_flat_bot_tri(sdl, tris[1]);
	}
}
