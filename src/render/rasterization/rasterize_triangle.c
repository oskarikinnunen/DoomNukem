#include "doomnukem.h"

#define FOG 0.0025f

/*
inline static uint32_t sample_img(t_render *render, t_vector3 t)
{
	uint32_t	xsample;
	uint32_t	ysample;

	xsample = t.x;
	ysample = t.y;
	return(render->map.data[ysample * render->map.size.x + xsample]);
}

inline static void scanline(int ax, int bx, int y, t_vector2 *p, t_vector3 *t, t_sdlcontext *sdl)
{
	t_vector3	tex;
	t_vector2	bary;
	float		dist;

	//printf("ax %d bx %d\n", ax, bx);
	bary = barycentric_coordinates(p, (t_vector2){ax, y});
	dist = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
	while(ax <= bx)
	{
		tex.z = ft_flerp(t[0].z, t[1].z, bary.x) + ((t[2].z - t[0].z) * bary.y);
		if (tex.z > sdl->zbuffer[ax + y * sdl->window_w])
		{
			tex.x = ft_flerp(t[0].x, t[1].x, bary.x) + ((t[2].x - t[0].x) * bary.y);
			tex.y = ft_flerp(t[0].y, t[1].y, bary.x) + ((t[2].y - t[0].y) * bary.y);
			tex.x = tex.x / tex.z;
			tex.y = tex.y / tex.z;
			sdl->zbuffer[ax + y * sdl->window_w] = tex.z;
			((uint32_t *)sdl->surface->pixels)[ax + y * sdl->window_w] =
				sample_img(&sdl->render, tex);
		}
		ax++;
		bary = barycentric_coordinates(p, (t_vector2){ax, y});
	}
	render_bitmask_row(ax, bx, dist * 1000.0f, tex.z * 1000.0f, y, sdl);
}
if (xsample < 0 || ysample < 0 || ysample * render->map.size.x + xsample > render->map.size.x * render->map.size.y)
{
	printf("xsample ysample %d %d\n%d %d %d %d\n%f %f\n", xsample, ysample, render->img->size.x, render->img->size.y, render->map.size.x, render->map.size.y, t.x, t.y);
	exit(0);
}
*/

inline static uint32_t sample_img(t_render *render, uint32_t xsample, uint32_t ysample)
{
	uint32_t	clr;
	//xsample = ft_clamp(xsample, 0, render->map.size.x);
	//ysample = ft_clamp(ysample, 0, render->map.size.y);
	clr = render->map.data[ysample * render->map.size.x + xsample];
	//render->map.data[ysample * render->map.size.x + xsample] = CLR_PRPL;
	return(clr);
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
				sample_img(&sdl->render, tex.x, tex.y);
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
	int				endy;

	p = triangle.p;
	t = triangle.t;
	delta = p[0].y - p[1].y;
	t_vector2 dv0 = vector2_div(vector2_sub(p[0], p[1]), delta);
	t_vector2 dv1 = vector2_div(vector2_sub(p[0], p[2]), delta);
	t_vector3 tdv0 = vector3_div(vector3_sub(t[0], t[1]), delta);
	t_vector3 tdv1 = vector3_div(vector3_sub(t[0], t[2]), delta);

	t_vector2 itedge0 = p[1];
	t_vector2 itedge1 = p[2];
	t_vector3 titedge0 = t[1];
	t_vector3 titedge1 = t[2];

	y = ceilf(p[1].y - 0.5f);
	endy = ceilf(p[0].y - 0.5f);

	itedge0 = vector2_add(itedge0, vector2_mul(dv0, (float)y + 0.5f - p[1].y));
	itedge1 = vector2_add(itedge1, vector2_mul(dv1, (float)y + 0.5f - p[1].y));
	titedge0 = vector3_add(titedge0, vector3_mul(tdv0, (float)y + 0.5f - p[1].y));
	titedge1 = vector3_add(titedge1, vector3_mul(tdv1, (float)y + 0.5f - p[1].y));

	while (y < endy)
	{
		int ax, bx;
		ax = ceilf(itedge0.x - 0.5f);
		bx = ceilf(itedge1.x - 0.5f);
		
		t_vector3 dtcline = vector3_div(vector3_sub(titedge1, titedge0), itedge1.x - itedge0.x);

		t_vector3 itcline = titedge0;

		for (int x = ax; x < bx; x++)
		{
			if (itcline.z > sdl->zbuffer[x + y * sdl->window_w])
			{
				int tempx = ft_clamp((itcline.x / itcline.z), 0, sdl->render.map.size.x - 1);
				int tempy = ft_clamp((itcline.y / itcline.z), 0, sdl->render.map.size.y - 1);
				sdl->zbuffer[x + y * sdl->window_w] = itcline.z;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
					sample_img(&sdl->render, tempx, tempy);
			}
			itcline = vector3_add(itcline, dtcline);
		}
		itedge0 = vector2_add(itedge0, dv0);
		itedge1 = vector2_add(itedge1, dv1);
		titedge0 = vector3_add(titedge0, tdv0);
		titedge1 = vector3_add(titedge1, tdv1);
		y++;
	}
}

static void render_flat_bot_tri(t_sdlcontext *sdl, t_point_triangle triangle)
{
	t_vector2		*p;
	t_vector3		*t;
	int				y;
	float			delta;
	int				endy;

	p = triangle.p;
	t = triangle.t;
	delta = p[2].y - p[0].y;
	t_vector2 dv0 = vector2_div(vector2_sub(p[1], p[0]), delta);
	t_vector2 dv1 = vector2_div(vector2_sub(p[2], p[0]), delta);
	t_vector3 tdv0 = vector3_div(vector3_sub(t[1], t[0]), delta);
	t_vector3 tdv1 = vector3_div(vector3_sub(t[2], t[0]), delta);

	t_vector2 itedge0 = p[0];
	t_vector2 itedge1 = p[0];
	t_vector3 titedge0 = t[0];
	t_vector3 titedge1 = t[0];

	y = ceilf(p[0].y - 0.5f);
	endy = ceilf(p[2].y - 0.5f);

	itedge0 = vector2_add(itedge0, vector2_mul(dv0, (float)y + 0.5f - p[0].y));
	itedge1 = vector2_add(itedge1, vector2_mul(dv1, (float)y + 0.5f - p[0].y));
	titedge0 = vector3_add(titedge0, vector3_mul(tdv0, (float)y + 0.5f - p[0].y));
	titedge1 = vector3_add(titedge1, vector3_mul(tdv1, (float)y + 0.5f - p[0].y));

	while (y < endy)
	{
		int ax, bx;
		ax = ceilf(itedge0.x - 0.5f);
		bx = ceilf(itedge1.x - 0.5f);
		
		t_vector3 dtcline = vector3_div(vector3_sub(titedge1, titedge0), itedge1.x - itedge0.x);

		t_vector3 itcline = titedge0;

		for (int x = ax; x < bx; x++)
		{
			if (itcline.z > sdl->zbuffer[x + y * sdl->window_w])
			{
				int tempx = ft_clamp((itcline.x / itcline.z), 0, sdl->render.map.size.x - 1);
				int tempy = ft_clamp((itcline.y / itcline.z), 0, sdl->render.map.size.y - 1);

				sdl->zbuffer[x + y * sdl->window_w] = itcline.z;
				((uint32_t *)sdl->surface->pixels)[x + y * sdl->window_w] =
					sample_img(&sdl->render, tempx, tempy);
				
			}
			itcline = vector3_add(itcline, dtcline);
		}
		itedge0 = vector2_add(itedge0, dv0);
		itedge1 = vector2_add(itedge1, dv1);
		titedge0 = vector3_add(titedge0, tdv0);
		titedge1 = vector3_add(titedge1, tdv1);
		y++;
	}
}


void	render_triangle_lit(t_sdlcontext *sdl, t_render *render, int index)
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

