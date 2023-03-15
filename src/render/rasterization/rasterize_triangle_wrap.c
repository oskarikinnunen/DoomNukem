#include "doomnukem.h"

inline static uint32_t sample_img_dynamic(t_render *render, uint32_t xsample, uint32_t ysample)
{
	xsample = xsample % (render->img->size.x);
	ysample = ysample % (render->img->size.y);
	return((render->img->data[ysample * render->img->size.x + xsample]));
}

inline static void scanline(int start, int end, int y, t_point_triangle triangle, t_sdlcontext *sdl)
{
	t_vector3	tex;
	float		dist;
	t_stepv3	slope;
	int			x;


	//printf("start: %i end: %i\n", start, end);
	slope = make_uv_slopev3(start, end, y, triangle);
	x = start;
	while(x < end)
	{
		float test = x - start;
		tex.z = slope.location.z + test * slope.step.z;
		if (tex.z > sdl->zbuffer[x + y * sdl->window_w])
		{
			tex.x = slope.location.x + test * slope.step.x;
			tex.y = slope.location.y + test * slope.step.y;
			sdl->zbuffer[x + y * sdl->window_w] = tex.z;
			((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
				sample_img_dynamic(&sdl->render, tex.x / tex.z, tex.y / tex.z);
		}
		x++;
	}
	render_bitmask_row(start, end, 1.0f / slope.location.z, 1.0f / tex.z, y, sdl);
}

inline static void render_flat_top_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2		*p;
	t_vector3		*t;
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	steps = p[0].y - p[1].y;
	left = make_slope(p[1].x, p[0].x, steps);
	right = make_slope(p[2].x, p[0].x, steps);
	y = ceilf(p[2].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, triangle, sdl);
		y++;
	}
}

inline static void render_flat_bot_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2			*p;
	t_vector3		*t;
	int				y;
	float			steps;
	t_step			left;
	t_step			right;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	steps = p[1].y - p[0].y;
	left = make_slope(p[0].x, p[1].x, steps);
	right = make_slope(p[0].x, p[2].x, steps);
	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);
	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		right.location = right.step * ((float)y + 0.5f - p[0].y) + p[0].x;
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, triangle, sdl);
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

	t_vector3 tcedgel, tcedger, tcbottom;

	tcedgel = t[1];
	tcedger = t[2];
	tcbottom = t[0];

	t_stepv3	tleft;
	t_stepv3	tright;


	tleft.step = vector3_div(vector3_sub(tcbottom, tcedgel), p[0].y - p[1].y);
	tright.step = vector3_div(vector3_sub(tcbottom, tcedger), p[0].y - p[2].y);

	tcedgel = vector3_add(tcedgel, vector3_mul(tleft.step, (float)y + 0.5f - p[1].y));
	tcedger = vector3_add(tcedger, vector3_mul(tright.step, (float)y + 0.5f - p[1].y));

	float tex_width = sdl->render.img->size.x;
	float tex_height = sdl->render.img->size.y;
	float tex_clamp_x = tex_width - 1.0f;
	float tex_clamp_y = tex_height - 1.0f;

	while (y < endy)
	{
		left.location = left.step * ((float)y + 0.5f - p[1].y) + p[1].x;
		right.location = right.step * ((float)y + 0.5f - p[2].y) + p[2].x;

		int ax, bx;
		ax = ceilf(left.location - 0.5f);
		bx = ceilf(right.location - 0.5f);
		t_vector3 tcscanstep = vector3_div(vector3_sub(tcedger, tcedgel), right.step - left.step);
		t_vector3 tc = vector3_mul(tcscanstep, (float)ax + 0.5f - left.step);
		for (int x = ax; x < bx; x++)
		{
			if (tc.z > sdl->zbuffer[ax + y * sdl->window_w])
			{
				int tempx = (tc.x / tc.z) * sdl->render.img->size.x;
				int tempy = (tc.y / tc.z) * sdl->render.img->size.y;
				sdl->zbuffer[x + y * sdl->window_w] = tc.z;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
					sample_img_dynamic(&sdl->render, tempx, tempy);
			}
			tc = vector3_add(tc, tcscanstep);
		}
		scanline(ceilf(left.location - 0.5f), ceilf(right.location - 0.5f), y, p, t, sdl);
		tcedgel = vector3_add(tcedgel, tleft.step);
		tcedger = vector3_add(tcedgel, tright.step);
		y++;
	}

	inline static void render_flat_top_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2			*p;
	t_vector3		*t;
	int				y;
	float			delta;
	float			w1;
	float			w2;
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

inline static void render_flat_bot_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2			*p;
	t_vector3		*t;
	int				y;
	float			delta;
	float			w1;
	float			w2;
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

		*/