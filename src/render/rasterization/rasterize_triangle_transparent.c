/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize_triangle_transparent.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:02:34 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/27 23:07:59 by okinnune         ###   ########.fr       */
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

//TODO NORMINETEEEE
inline static void	scanline(
	int start, int end, int y, t_sdlcontext *sdl)
{
	uint32_t	clr;
	t_vector3	tex;
	t_stepv3	slope;
	int			x;

	slope = make_uv_slopev3(start, end, y, sdl->render.triangle);
	x = start;
	while (x < end)
	{
		tex.z = slope.location.z + ((x - start) + 0.5f) * slope.step.z;
		if (tex.z > sdl->zbuffer[x + y * sdl->window_w])
		{
			tex.x = slope.location.x + ((x - start) + 0.5f) * slope.step.x;
			tex.y = slope.location.y + ((x - start) + 0.5f) * slope.step.y;
			clr = sample_img(&sdl->render, tex.x / tex.z, tex.y / tex.z);
			if (((clr >> 24) & 0xFF) > 240)
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] = \
					clr;
			else if (((clr >> 24) & 0xFF) > 14)
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] = \
					blend_colors_alpha(((uint32_t *)sdl->surface->pixels)[\
					x + y * sdl->window_w], clr, 127);
		}
		x++;
	}
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

void	render_triangle_transparent(
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
