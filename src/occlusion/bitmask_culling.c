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
		tri.t[i].w = triprojected.p[i].w; //might not be necessary as we are not beding uvs with perspective and only need the distance
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

0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1

0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
*/

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static uint16_t mask_x(int x, int left_x, int right_x)
{
	const uint16_t	row = ~0;
	return(((row << MAX(0, left_x - x))& ~(row << MAX(0, right_x - x))));
}

/*
Z1max value as the working layer and
Z0max as the reference layer

function updateHiZBuffer(tile, tri)
// Discard working layer heuristic
dist1t = tile.zMax1 - tri.zMax
dist01 = tile.zMax0 - tile.zMax1
if (dist1t > dist01)
tile.zMax1 = 0
tile.mask = 0

// Merge current triangle into working layer
tile.zMax1 = max(tile.zMax1, tri.zMax)
tile.mask |= tri.coverageMask

// Overwrite ref. layer if working layer full
if (tile.mask == ~0)
tile.zMax0 = tile.zMax1
tile.zMax1 = 0
tile.mask = 0
*/

static t_tile update_tile(float aw, t_tile tile, uint64_t res)
{
	float dist1t;
	float dist0t;

	if (aw <= tile.max0)
	{
		dist1t = tile.max1 - aw;
		dist0t = tile.max0 - tile.max1;
		
		if (dist1t > dist0t)
		{
			tile.max1 = 0;
			tile.mask = 0;
		}
		if (tile.max1 < aw)
			tile.max1 = aw;
		tile.mask |= res;
		if (tile.mask == ~0)
		{
			tile.max0 = tile.max1;
			tile.max1 = 0;
			tile.mask = 0;
		}
	}
	return (tile);
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
	const __uint128_t	line = 65535;
	uint16_t		res;
	const uint8_t	line1 = 255;

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
		float wstep = (bw - aw) * (8.0f / (float)(ax - x));
		if (ax == x)
			wstep = 0;
		int temp1, temp2;
		temp1 = x % 16;
		temp2 = ax - x + temp1 + 1;
		if (temp2 > 16)
			temp2 = 16;
		int chunk = (y / 8) * (sdl->bitmask.bitmask_chunks.x) + (x / 16);
		int end_chunk = (y / 8) * (sdl->bitmask.bitmask_chunks.x) + (ax / 16);
		int wchunk = chunk * 2;
		y_chunk = ((y % 8) * 16);
		int y1_chunk = ((y % 8) * 8);
		res = mask_x(0, temp1, temp2);
		sdl->bitmask.bitmask[chunk++] |= (__uint128_t)res << y_chunk;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)(res & 0xFF)) << y1_chunk);
		wchunk++;
		aw += wstep;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)((res >> 8) & 0xFF)) << y1_chunk);
		wchunk++;
		aw += wstep;
		x += 16 - temp1;
		while (x < ax)
		{
			sdl->bitmask.bitmask[chunk++] |= line << y_chunk;
			sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], line1 << y1_chunk);
			wchunk++;
			aw += wstep;
			sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], line1 << y1_chunk);
			wchunk++;
			aw += wstep;
			x += 16;
		}

		res = mask_x(0, 0, ax - x + 1);
		sdl->bitmask.bitmask[chunk] |= (__uint128_t)res << y_chunk;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)(res & 0xFF)) << y1_chunk);
		wchunk++;
		aw += wstep;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)((res >> 8) & 0xFF)) << y1_chunk);
		wchunk++;
		aw += wstep;
		y--;
	}
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
	uint16_t		res;
	const uint8_t	line1 = 255;

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
		if (ax == x)
			wstep = 0;
		int temp1, temp2;
		temp1 = x % 16;
		temp2 = ax - x + temp1 + 1;
		if (temp2 > 16)
			temp2 = 16;
		int chunk = (y / 8) * (sdl->bitmask.bitmask_chunks.x) + (x / 16);
		int end_chunk = (y / 8) * (sdl->bitmask.bitmask_chunks.x) + (ax / 16);

		int wchunk = chunk * 2;
		y_chunk = ((y % 8) * 16);
		int y1_chunk = ((y % 8) * 8);
		res = mask_x(0, temp1, temp2);
		sdl->bitmask.bitmask[chunk++] |= (__uint128_t)res << y_chunk;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)(res & 0xFF)) << y1_chunk);
		wchunk++;
		aw += wstep;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)((res >> 8) & 0xFF)) << y1_chunk);
		wchunk++;
		aw += wstep;
		x += 16 - temp1;
		while (x < ax)
		{
			sdl->bitmask.bitmask[chunk++] |= line << y_chunk;
			sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], line1 << y1_chunk);
			wchunk++;
			aw += wstep;
			sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], line1 << y1_chunk);
			wchunk++;
			aw += wstep;
			x += 16;
		}
		res = mask_x(0, 0, ax - x + 1);
		sdl->bitmask.bitmask[chunk] |= (__uint128_t)res << y_chunk;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)(res & 0xFF)) << y1_chunk);
		wchunk++;
		aw += wstep;
		sdl->bitmask.tile[wchunk] = update_tile(aw, sdl->bitmask.tile[wchunk], ((uint64_t)((res >> 8) & 0xFF)) << y1_chunk);
		wchunk++;
		aw += wstep;
		y++;
	}
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
		bt.w[i] = 1.0f / render->screenspace_ptris[index].t[i].w;
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

bool is_entity_bitmask_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_square	s;
	const __uint128_t max = ~0;

	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
	calculate_triangles(*sdl, render, entity);
	get_min_max_from_triangles(&s.min, &s.max, render->screenspace_ptris, render->screenspace_ptri_count);
	float w = vector3_dist(vector3_add(entity->obj->bounds.origin, entity->transform.position), render->camera.position);
	if (s.min.x < 0.0f || s.min.y < 0.0f || isnan(s.min.y) || isnan(s.min.x))
	{
		exit(0);
	}
	if (s.max.x > sdl->window_w || s.max.y > sdl->window_h)
		exit(0);
	for (int y = s.min.y; y < s.max.y; y++)
	{
		for (int x = s.min.x; x < s.max.x; x++)
		{
		//	printf("x %d y %d\n", x, y);
		//	if (sdl->bitmask.bitmask[(y / 8) * sdl->bitmask.chunk_size.x + (x / 16)] != max)
		//		return(false);
			if (w < sdl->bitmask.tile[(y / 8) * sdl->bitmask.tile_chunks.x + (x / 8)].max0)
				return(false);
		}
	}
	return (true);
}
