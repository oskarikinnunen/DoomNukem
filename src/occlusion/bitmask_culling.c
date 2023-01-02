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
	uint16_t	row = ~0;
	return(((row << MAX(0, left_x - x))& ~(row << MAX(0, right_x - x))));
}

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

static void fill_point_tri_top(t_point *t, t_point_triangle triangle, t_sdlcontext *sdl)
{
	t_point			*p;
	float			step[2];
	float			t_step[2];
	t_point			i;
	int				x;
	int				y;
	float			delta;
	int index;

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
		int chunk_j = i.y * (sdl->bitmask.chunk_size.x) + i.x;
		int prev;
		while (index < 8 && y < p[0].y)
		{
			x = p[1].x + (step[0] * (float)(y - p[1].y));
			int ax =  p[2].x + (step[1] * (float)(y - p[1].y));
			chunk_j = i.y * (sdl->bitmask.chunk_size.x) + (x / 16);
			if (i.y == 45)
			{
				printf("x = %d\n", x);
				printf("1chunkj = %d\n", chunk_j);
			}
			if (index != 0 && prev != chunk_j)
			{
				//printf("prev %d %d\n", prev, chunk_j);
				while (prev < chunk_j)
				{
					prev++;
					for (int j = 0; j < index; j++)
					{
						sdl->bitmask.bitmask[prev] |= mask_x(0, 0, 16) << (j * 16); //(__uint128_t)65535
					}
				}
			}
			prev = chunk_j;
			if (chunk_j > sdl->bitmask.chunk_size.x * sdl->bitmask.chunk_size.y)
			{
				for (int i = 0; i < 3; i++)
				{
					print_point(triangle.p[i]);
				}
				printf("y %d x %d %d %f %f %d\n", y, x, p[1].x, step[0], (float)(y - p[1].y), p[1].y);
				printf("chunk j %d max size %d x %d %d %d\n", chunk_j, sdl->bitmask.chunk_size.x * sdl->bitmask.chunk_size.y, sdl->bitmask.chunk_size.x, sdl->bitmask.chunk_size.y, i.y);
				exit(0);
			}
			sdl->bitmask.bitmask[chunk_j] |= mask_x(0, x % 16, 16) << e;
			e += 16;
			y++;
			index++;
		}
		chunk_j = end_c;
		if (i.y == 45)
		{
			printf("chunkj = %d\n", chunk_j);

		}
		if (i.y == 45 && chunk_j == 3630)
		{
			chunk_j++;
			while (chunk_j <= i.y * (sdl->bitmask.chunk_size.x) + ab && i.y == 45)
			{
				sdl->bitmask.bitmask[chunk_j++] = ~0;
				i.x++;
			}
			if ((sdl->bitmask.bitmask[chunk_j++] & 0) == 0)
				printf("is zero\n\n");
		}
		i.y++;
	}

	return;
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
	t_point			p2_split;
	t_point			p2_temp;
	t_point			*p2;
	t_point			tile_tri[3];
	float			lerp;

	p2 = render->screenspace_ptris[index].p;
	//p2[0] = (t_point){0, 0};
	//p2[1] = (t_point){0, 450};
	//p2[2] = (t_point){899, 250};//175 176 breaks
	//for (int i = 0; i < 3; i++)
	//	print_point(p2[i]);
	//printf("end\n");
	sort_point_tri(p2);
	lerp = ((float)p2[1].y - (float)p2[2].y) / ((float)p2[0].y - (float)p2[2].y);
	p2_split.x = p2[2].x + (lerp * ((float)p2[0].x - (float)p2[2].x));
	p2_split.y = p2[1].y;

	if (p2_split.x < p2[1].x)
	{
		ft_swap(&p2[1], &p2_split, sizeof(t_point));
	}
	p2_temp = p2[2];
	p2[2] = p2_split;;
	t_point div = (t_point){16, 8};
	for (int i = 0; i < 3; i++)
	{
		tile_tri[i] = point_div_point(p2[i], div);
	}
	if (p2[0].y != p2[1].y)
		fill_point_tri_top2(tile_tri, render->screenspace_ptris[index], sdl);
	p2[0] = p2_temp;
	if (p2[0].y != p2[1].y)
		fill_point_tri_bot2(render->screenspace_ptris[index], sdl);
}

void erender_bitmask_triangle(t_sdlcontext *sdl, t_render *render, int index)
{
	return;
	t_point_triangle	t;
	t_point				max;
	t_point				min;
	__uint128_t			tile;
	int x,y;

	t = render->screenspace_ptris[index];

	sort_point_tri(t.p);
	max = t.p[0];
	min = t.p[0];
	for (int i = 1; i < 3; i++)
	{
		if (max.x < t.p[i].x)
			max.x = t.p[i].x;
		if (min.x > t.p[i].x)
			min.x = t.p[i].x;
		if (max.y < t.p[i].y)
			max.y = t.p[i].y;
		if (min.y > t.p[i].y)
			min.y = t.p[i].y;
	}
	max.x /= 16;
	min.x /= 16;
	max.y /= 8;
	min.y /= 8;
	tile = ~0;
	y = min.y;
	while (y < max.y)
	{
		x = min.x;
		while (x < max.x)
		{
			for (int y1 = y * 8; y1 < y * 8 + 8; y1++)
			{
				int e = vector2_fdist_to_plane((t_vector2){0, y1}, vector2_sub((t_vector2){t.p[0].x, t.p[0].y}, (t_vector2){t.p[2].x, t.p[2].y}), (t_vector2){t.p[2].x, t.p[2].y});
			//	tile = mask_x(0, 0, 18);
			}
			sdl->bitmask.bitmask[y * sdl->bitmask.chunk_size.x + x] |= tile;
			x++;
		}
		y++;
	}
	return;
	sort_point_tri(t.p);
	if (t.p[1].x > t.p[2].x)
	{
		int temp = t.p[2].x;
		t.p[2].x = t.p[1].x;
		t.p[1].x = temp;
	}
	max.x = t.p[2].x;
	max.y = t.p[0].y;
	if (max.x < t.p[0].x)
		max.x = t.p[0].x;
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