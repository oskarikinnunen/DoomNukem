#include "doomnukem.h"


t_point_triangle triangle_to_bitmask_screenspace_point_triangle(t_mat4x4 matproj, t_triangle clipped, t_sdlcontext sdl)
{
	t_triangle			triprojected;
	t_point_triangle	tri;
	int					i;
	t_vector3			voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};

	i = 0;
	while (i < 3)
	{
		triprojected.p[i] = quaternion_mul_matrix(matproj, clipped.p[i]);
		tri.t[i].w = 1.0f / triprojected.p[i].w; //might not be necessary as we are not beding uvs with perspective and only need the distance
		triprojected.p[i].v = vector3_div(triprojected.p[i].v, triprojected.p[i].w);
		triprojected.p[i].v = vector3_negative(triprojected.p[i].v);
		triprojected.p[i].v = vector3_add(triprojected.p[i].v, voffsetview);
		tri.p[i].x = triprojected.p[i].v.x * (0.5f * (float)(sdl.window_w * sdl.resolution_scaling));
		tri.p[i].y = triprojected.p[i].v.y * (0.5f * (float)(sdl.window_h * sdl.resolution_scaling));
		i++;
	}
	return(tri);
}

/*
function coverage(x, left, right)
return (~0 >> max(0, left - x))
& ~(~0 >> max(0, right - x))
*/

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static __uint128_t mask_x(int x, int left_x, int right_x)
{
	const uint16_t	row = ~0;
	return(((row << MAX(0, left_x - x))& ~(row << MAX(0, right_x - x))));
}

static void fill_point_tri_bot2(t_bit_tri	bt, t_sdlcontext *sdl)
{
	t_point			*p;
	float			step[2];
	float			t_step[2];
	int				x;
	int				y;
	float			delta;
	int				y_chunk;
	int index;
	const __uint128_t line = 65535;

	p = bt.p;
	delta = 1.0f / ((float)(p[1].y - p[0].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;
	t_step[0] = (bt.w[0] - bt.w[1]) * delta;
	t_step[1] = (bt.w[0] - bt.w[2]) * delta;
	y = p[1].y;
	while (y >= p[0].y)
	{
		delta = (float)(p[1].y - y);
		x = p[1].x + (step[0] * delta);
		int ax =  p[2].x + (step[1] * delta);
		float aw = bt.w[1] + (t_step[0] * delta);
		float bw = bt.w[2] + (t_step[1] * delta);
		//t[0].u - t[2].u) * delta;1.0f / (float)(ax - x)
		float wstep = (bw - aw) * (8.0f / (float)(ax - x));

		int temp1, temp2;
		temp1 = x % 16;
		temp2 = ax - x + temp1 + 1;
		if (temp2 > 16)
			temp2 = 16;
		int chunk = (y / 8) * (sdl->bitmask.chunk_size.x) + (x / 16);
		int end_chunk = (y / 8) * (sdl->bitmask.chunk_size.x) + (ax / 16);

		int wchunk = (y / 8) * (sdl->window_w/8) + (x / 8);
		int start = wchunk;
		y_chunk = ((y % 8) * 16);
		sdl->bitmask.bitmask[chunk++] |= mask_x(0, temp1, temp2) << y_chunk;
		if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
		if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
		x += 16 - temp1;
		while (x <= ax - 16)
		{
			sdl->bitmask.bitmask[chunk++] |= line << y_chunk;
			if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
			if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
			x += 16;
		}
		sdl->bitmask.bitmask[chunk] |= mask_x(0, 0, ax - x + 1) << y_chunk;
		if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
		if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
		y--;
	}
}

__uint128_t create_bitmask(uint16_t mask1, uint16_t mask2, uint16_t mask3, uint16_t mask4) {
  __uint128_t result = 0;

  result |= (__uint128_t)mask1 << 112;
  result |= (__uint128_t)mask2 << 96;
  result |= (__uint128_t)mask3 << 80;
  result |= (__uint128_t)mask4 << 64;

  return result;
}

static void fill_point_tri_top2(t_bit_tri	bt, t_sdlcontext *sdl)
{
	t_point			*p;
	float			step[2];
	float			t_step[2];
	int				x;
	int				y;
	float			delta;
	int				y_chunk;
	int index;
	const __uint128_t line = 65535;

	p = bt.p;
	delta = 1.0f/((float)(p[0].y - p[1].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;
	t_step[0] = (bt.w[0] - bt.w[1]) * delta;
	t_step[1] = (bt.w[0] - bt.w[2]) * delta;
	y = p[1].y;
	while (y <= p[0].y)
	{
		delta = (float)(y - p[1].y);
		x = p[1].x + (step[0] * delta);
		int ax =  p[2].x + (step[1] * delta);
		float aw = bt.w[1] + (t_step[0] * delta);
		float bw = bt.w[2] + (t_step[1] * delta);
		float wstep = (bw - aw) * (8.0f / (float)(ax - x));
		//printf("wstep %f\n", wstep);
		int temp1, temp2;
		temp1 = x % 16;
		temp2 = ax - x + temp1 + 1;
		if (temp2 > 16)
			temp2 = 16;
		int chunk = (y / 8) * (sdl->bitmask.chunk_size.x) + (x / 16);
		int end_chunk = (y / 8) * (sdl->bitmask.chunk_size.x) + (ax / 16);

		int wchunk = (y / 8) * (sdl->window_w/8) + (x / 8);
		int start = wchunk;

		y_chunk = ((y % 8) * 16);
		sdl->bitmask.bitmask[chunk++] |= mask_x(0, temp1, temp2) << y_chunk;
		if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
		if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
		x += 16 - temp1;
		while (x <= ax - 16)
		{
			sdl->bitmask.bitmask[chunk++] |= line << y_chunk; // static line
			if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
			if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
			x += 16;
		}
		sdl->bitmask.bitmask[chunk] |= mask_x(0, 0, ax - x + 1) << y_chunk;
		if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
		if (aw > sdl->bitmask.dist[wchunk])
			sdl->bitmask.dist[wchunk] = aw + ((wchunk - start) * (bw - aw));
		wchunk++;
		y++;
	}
}

/*
	while(x <= ax)
	{

		//sdl->bitmask.bitmask[chunk_i] |= 1UL << (((y % 8) * 16) + (x % 16));
		sdl->bitmask.bitmask[(y / 8) * ((sdl->window_w)/16) + (x / 16)] |= (__uint128_t)((__uint128_t)1 << (__uint128_t)(((y % 8) * 16) + (x % 16)));
		x++;
	}
creates two triangles from the given triangle one flat top and one flat bottom.
both triangles are then assigned to t_point p[3] array and passed onto fill_tri_bot/top functions.
p[0] is always the pointy head of the triangle p[1] and p[2] are flat points where, p[1] x is smaller than p[2]
*/

t_point point_div_point(t_point src, t_point div)
{
	src.x /= div.x;
	src.y /= div.y;
	return (src);
}

void	render_bitmask_triangle(t_sdlcontext *sdl, t_render *render, int index)
{
	t_bit_tri		bt;
	t_point			p2_split;
	float			w_split;
	t_point			p2_temp;
	float			w_temp;
	t_point			*p2;
	t_point			tile_tri[3];
	float			lerp;

	for (int i = 0; i < 3; i++)
	{
		bt.p[i] = render->screenspace_ptris[index].p[i];
		bt.w[i] = render->screenspace_ptris[index].t[i].w;
	}
	p2 = bt.p;
	sort_point_tri(bt.p, bt.w);
	lerp = ((float)p2[1].y - (float)p2[2].y) / ((float)p2[0].y - (float)p2[2].y);
	p2_split.x = p2[2].x + (lerp * ((float)p2[0].x - (float)p2[2].x));
	p2_split.y = p2[1].y;
	w_split = ft_flerp(bt.w[2], bt.w[0], lerp);

	if (p2_split.x < p2[1].x)
	{
		ft_swap(&p2[1], &p2_split, sizeof(t_point));
		ft_swap(&bt.w[1], &w_split, sizeof(float));
	}
	p2_temp = p2[2];
	w_temp = bt.w[2];
	p2[2] = p2_split;
	bt.w[2] = w_split;
	/*t_point div = (t_point){16, 8};
	for (int i = 0; i < 3; i++)
	{
		tile_tri[i] = point_div_point(p2[i], div);
	}*/
	if (p2[0].y != p2[1].y)
		fill_point_tri_top2(bt, sdl);
	p2[0] = p2_temp;
	bt.w[0] = w_temp;
	if (p2[0].y != p2[1].y)
		fill_point_tri_bot2(bt, sdl);
}

void render_bitmask(t_sdlcontext *sdl, t_render *render)
{
	int index = 0;
	while (index < render->screenspace_ptri_count)
	{
		render_bitmask_triangle(sdl, render, index);	
		index++;
	}
}

static void bitmask_clip_and_render_triangles(t_sdlcontext *sdl, t_render *render)
{
	clipped_point_triangle(render, *sdl);
	render_bitmask(sdl, render);
	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
}


static void render_bitmask_tris(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_object		*obj;
	t_quaternion	temp;
	int				index;

	obj = entity->obj;
	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
	index = 0;
	while (index < obj->face_count)
	{
		t_triangle	tritransformed;

		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		if (!is_triangle_backface(tritransformed, render))
		{
			tritransformed.t[0].w = 1.0f;
			tritransformed.t[1].w = 1.0f;
			tritransformed.t[2].w = 1.0f;
			t_triangle clipped[2];
			for (int i = 0; i < 3; i++)
			{
				tritransformed.p[i] = quaternion_mul_matrix(render->camera.matview, tritransformed.p[i]);
			}
			int nclippedtriangles = clip_bitmask_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, tritransformed, clipped);
			for (int n = 0; n < nclippedtriangles; n++)
				render->worldspace_ptris[render->worldspace_ptri_count++] = triangle_to_bitmask_screenspace_point_triangle(render->camera.matproj, clipped[n], *sdl);
		}
		index++;
	}
	bitmask_clip_and_render_triangles(sdl, render);
}

void render_entity_to_bitmask(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	render_worldspace(render, entity);
	render_bitmask_tris(sdl, render, entity);
	//render->rs.render_count++;
}

bool is_entity_bitmask_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_square	s;
	const __uint128_t max = ~0;

	calculate_triangles(*sdl, render, entity);
	get_min_max_from_triangles(&s.min, &s.max, render->screenspace_ptris, render->screenspace_ptri_count);
	float w = vector3_dist(vector3_add(entity->obj->bounds.origin, entity->transform.position), render->camera.position);
	for (int y = s.min.y; y < s.max.y; y++)
	{
		for (int x = s.min.x; x < s.max.x; x++)
		{
			if (sdl->bitmask.bitmask[(y / 8) * sdl->bitmask.chunk_size.x + (x / 16)] != max)
				return(false);
			if (w < 1.0f / sdl->bitmask.dist[(y / 8) * (sdl->window_w/8) + (x / 8)])
				return(false);
		}
	}
	return (true);
}

/*
static void fill_point_tri_bot2(t_point_triangle triangle, t_sdlcontext *sdl)
{
	t_point			*p;
	float			step[2];
	int				x;
	int				y;
	float			delta;
	int				y_chunk;
	const __uint128_t line = 65535;

	p = triangle.p;
	delta = 1.0f / ((float)(p[1].y - p[0].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;
	y = p[1].y;
	while (y >= p[0].y)
	{
		delta = (float)(p[1].y - y);
		x = p[1].x + (step[0] * delta);
		int ax =  p[2].x + (step[1] * delta);
		int temp1, temp2;
		temp1 = x % 16;
		temp2 = ax - x + temp1 + 1;
		if (temp2 > 16)
			temp2 = 16;
		int chunk = (y / 8) * (sdl->bitmask.chunk_size.x) + (x / 16);
		int end_chunk = (y / 8) * (sdl->bitmask.chunk_size.x) + (ax / 16);
		y_chunk = ((y % 8) * 16);
		sdl->bitmask.bitmask[chunk++] |= mask_x(0, temp1, temp2) << y_chunk;
		x += 16 - temp1;
		while (x <= ax - 16)
		{
			sdl->bitmask.bitmask[chunk++] |= line << y_chunk;
			x += 16;
		}
		sdl->bitmask.bitmask[end_chunk] |= mask_x(0, 0, ax - x + 1) << y_chunk;
		y--;
	}
}

__uint128_t create_bitmask(uint16_t mask1, uint16_t mask2, uint16_t mask3, uint16_t mask4) {
  __uint128_t result = 0;

  result |= (__uint128_t)mask1 << 112;
  result |= (__uint128_t)mask2 << 96;
  result |= (__uint128_t)mask3 << 80;
  result |= (__uint128_t)mask4 << 64;

  return result;
}

static void fill_point_tri_top2(t_point *t, t_point_triangle triangle, t_sdlcontext *sdl)
{
	t_point			*p;
	float			step[2];
	float			t_step[2];
	int				x;
	int				y;
	float			delta;
	int				y_chunk;
	int index;
	const __uint128_t line = 65535;

	p = triangle.p;
	delta = 1.0f/((float)(p[0].y - p[1].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;
	y = p[1].y;
	while (y <= p[0].y)
	{
		delta = (float)(y - p[1].y);
		x = p[1].x + (step[0] * delta);
		int ax =  p[2].x + (step[1] * delta);
		int temp1, temp2;
		temp1 = x % 16;
		temp2 = ax - x + temp1 + 1;
		if (temp2 > 16)
			temp2 = 16;
		int chunk = (y / 8) * (sdl->bitmask.chunk_size.x) + (x / 16);
		int end_chunk = (y / 8) * (sdl->bitmask.chunk_size.x) + (ax / 16);
		y_chunk = ((y % 8) * 16);
		sdl->bitmask.bitmask[chunk++] |= mask_x(0, temp1, temp2) << y_chunk;
		x += 16 - temp1;
		while (x <= ax - 16)
		{
			sdl->bitmask.bitmask[chunk++] |= line << y_chunk; // static line
			x += 16;
		}
		sdl->bitmask.bitmask[end_chunk] |= mask_x(0, 0, ax - x + 1) << y_chunk;
		y++;
	}
}

static void fill_point_tri_top2(t_point *t, t_point_triangle triangle, t_sdlcontext *sdl)
{
	t_point			*p;
	float			step[2];
	float			t_step[2];
	int				x;
	int				y;
	t_point			i;
	float			delta;
	int				y_chunk;
	int index;
	const __uint128_t line = 65535;

	p = triangle.p;
	delta = 1.0f/((float)(p[0].y - p[1].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;

	delta = 1.0f/((float)(t[0].y - t[1].y));
	t_step[0] = (t[0].x - t[1].x) * delta;
	t_step[1] = (t[0].x - t[2].x) * delta;
	i.y = t[1].y;
	while (i.y <= t[0].y)
	{
		i.x = t[1].x + (t_step[0] * (float)(i.y - t[1].y));
		int ab =  t[2].x + (t_step[1] * (float)(i.y - t[1].y)); // - 1

		int start, end;
		start = p[1].x + (step[0] * (float)((i.y * 8) - p[1].y));
		end = p[1].x + (step[0] * (float)((i.y * 8 + 8) - p[1].y));

		int start_c, end_c;
		if (start < end)
		{
			start_c = i.y * (sdl->bitmask.chunk_size.x) + (start / 16);
			end_c = i.y * (sdl->bitmask.chunk_size.x) + (end / 16);
		}
		else
		{
			start_c = i.y * (sdl->bitmask.chunk_size.x) + (end / 16);
			end_c = i.y * (sdl->bitmask.chunk_size.x) + (start / 16);
		}
		y = i.y * 8;
		y = ft_clamp(y, p[1].y, sdl->window_h);
		index = 0;
		__uint128_t e = 0;
		if (i.y * 8 < p[1].y)
		{
			index = p[1].y - (i.y * 8);
			e = index * 16;
		}
		int ctemp;
		while (index < 8 && y < p[0].y)
		{
			x = p[1].x + (step[0] * (float)(y - p[1].y));
			int ax =  p[2].x + (step[1] * (float)(y - p[1].y));
			int temp1, temp2;
			temp1 = x % 16;
			temp2 = ax - x + temp1 + 1;
			if (temp2 > 16)
				temp2 = 16;
			ctemp = i.y * (sdl->bitmask.chunk_size.x) + (x / 16);
			sdl->bitmask.bitmask[ctemp++] |= mask_x(0, temp1, temp2) << e;
			while (ctemp <= end_c)
			{
				//exit(0);
				sdl->bitmask.bitmask[ctemp++] |= mask_x(0, 0, 16) << e;
			}//
			sdl->bitmask.bitmask[ctemp] |= mask_x(0, 0, ax % 16) << e;
			e += 16;
			y++;
			index++;
		}
		i.y++;
		continue;
		ctemp = end_c + 1;
		start = p[2].x + (step[1] * (float)((i.y * 8) - p[1].y));
		end = p[2].x + (step[1] * (float)((i.y * 8 + 8) - p[1].y));

		if (start < end)
		{
			start_c = i.y * (sdl->bitmask.chunk_size.x) + (start / 16);
			end_c = i.y * (sdl->bitmask.chunk_size.x) + (end / 16);
		}
		else
		{
			start_c = i.y * (sdl->bitmask.chunk_size.x) + (end / 16);
			end_c = i.y * (sdl->bitmask.chunk_size.x) + (start / 16);
		}
		while (ctemp < start_c)
		{
			sdl->bitmask.bitmask[ctemp++] = ~0; // const
		}
		y = i.y * 8;
		y = ft_clamp(y, p[1].y, sdl->window_h);
		index = 0;
		e = 0;
		if (i.y * 8 < p[1].y)
		{
			index = p[1].y - (i.y * 8);
			e = index * 16;
		}
		while (index < 8 && y < p[0].y)
		{
			//x = p[2].x + (step[0] * (float)(y - p[1].y));
			int ax =  p[2].x + (step[1] * (float)(y - p[1].y));
			int etemp = ctemp;
			while (etemp < end_c)
			{
				sdl->bitmask.bitmask[etemp++] |= mask_x(0, 0, 16) << e;
				etemp++;
			}
			sdl->bitmask.bitmask[end_c] |= mask_x(0, 0, ax % 16) << e;
			e += 16;
			y++;
			index++;
		}
		i.y++;
	}
}
*/