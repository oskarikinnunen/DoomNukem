#include "doomnukem.h"

inline static uint32_t sample_img_dynamic(t_render *render, uint32_t xsample, uint32_t ysample)
{
	xsample = xsample % (render->img->size.x);
	ysample = ysample % (render->img->size.y);
	return((render->img->data[ysample * render->img->size.x + xsample]));
}

inline static void scanline(int ax, int bx, int y, t_vector2*p, t_vector3 *t, t_sdlcontext *sdl)
{
	t_vector3	tex;
	t_vector2	bary;
	t_vector2	left;
	t_vector2	right;
	float		dist;
	float		steps;

	left = barycentric_coordinates(p, vector2_add_xy((t_vector2){ax, y}, 0.5f));
	right = barycentric_coordinates(p, vector2_add_xy((t_vector2){bx, y}, 0.5f));
	bary = left;
	dist = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
	int start = ax;
	steps = bx - start;
	while(ax < bx)
	{
		tex.z = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
		if (tex.z > sdl->zbuffer[ax + y * sdl->window_w])
		{
			tex.x = ft_flerp(t[0].x, t[1].x, bary.x) + ((t[2].x - t[0].x) * bary.y);
			tex.y = ft_flerp(t[0].y, t[1].y, bary.x) + ((t[2].y - t[0].y) * bary.y);
			tex.x = (tex.x / tex.z);
			tex.y = (tex.y / tex.z);
			sdl->zbuffer[ax + y * sdl->window_w] = tex.z;
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
				sample_img_dynamic(&sdl->render, tex.x, tex.y);
		}
		ax++;
		bary = vector2_lerp(left, right, (float)(ax - start) / steps);
	}
	render_bitmask_row(start, bx, dist, tex.z, y, sdl);
}

static void render_flat_top_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2		*p;
	t_vector3		*t;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	left.step = (p[0].x - p[1].x) / (p[0].y - p[1].y);
	right.step = (p[0].x - p[2].x) / (p[0].y - p[2].y);
	y = ceilf(p[2].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, p, t, sdl);
		y++;
	}
}

static void render_flat_bot_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2		*p;
	t_vector3		*t;
	int				y;
	float			delta;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	left.step = (p[1].x - p[0].x) / (p[1].y - p[0].y);
	right.step = (p[2].x - p[0].x) / (p[2].y - p[0].y);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, p, t, sdl);
		y++;
	}
}

void	render_triangle_unlit(t_sdlcontext *sdl, t_render *render, int index)
{
	t_point_triangle	triangle;
	t_point_triangle	tris[2];
	int	res;

	triangle = render->screenspace_ptris[index];
	if (sdl->ps1_tri_div > 1)
		triangle = ps1(triangle, sdl->ps1_tri_div);

	res = triangle_to_flat(triangle, tris);
	if (res == 0)
		render_flat_top_tri(sdl, tris[0]);
	else if (res == 1)
		render_flat_bot_tri(sdl, tris[0]);
	else
	{
		render_flat_top_tri(sdl, tris[0]);
		render_flat_bot_tri(sdl, tris[1]);
	}
}

/*
lerp = ((float)p[1].y - (float)p[2].y) / ((float)p[0].y - (float)p[2].y);
		p_split.x = p[2].x + (lerp * ((float)p[0].x - (float)p[2].x));
		p_split.y = p[1].y;
		t_split.x = ft_flerp(triangle.t[2].x, triangle.t[0].x, lerp);
		t_split.v = ft_flerp(triangle.t[2].v, triangle.t[0].v, lerp);
		t_split.z = ft_flerp(triangle.t[2].z, triangle.t[0].z, lerp);
		if (p_split.x < p[1].x)
		{
			ft_swap(&p[1], &p_split, sizeof(vector2));
			ft_swap(&triangle.t[1], &t_split, sizeof(t_vector3));
		}
		p_temp = p[2];
		t_temp = triangle.t[2];
		p[2] = p_split;
		triangle.t[2] = t_split;
		if (p[0].y != p[1].y && p[1].x != p[2].x)
			render_flat_top_tri(sdl, triangle);
		p[0] = p_temp;
		triangle.t[0] = t_temp;
		if (p[0].y != p[1].y && p[1].x != p[2].x)
			render_flat_bot_tri(sdl, triangle);
			*/