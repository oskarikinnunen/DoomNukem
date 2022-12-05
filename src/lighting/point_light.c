#include "doomnukem.h"

static void ft_swap(void * a, void * b, size_t len)
{
	unsigned char	*a1;
	unsigned char	*b1;
	unsigned char	temp;
	size_t			i;

	a1 = a;
	b1 = b;
	i = 0;
	while (i < len)
	{
		temp = a1[i];
		a1[i] = b1[i];
		b1[i] = temp;
		i++;
	}
}

static void	sort_tris(t_quaternion	*q, t_texture *t)
{
	int	s_x;
	int	s_j;
	t_quaternion	temp_q;
	t_texture 		temp_t;

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (q[s_j].v.y < q[s_j + 1].v.y)
			{
				temp_q = q[s_j];
				q[s_j] = q[s_j + 1];
				q[s_j + 1] = temp_q;

				temp_t = t[s_j];
				t[s_j] = t[s_j + 1];
				t[s_j + 1] = temp_t;
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

static void	sort_point_tris(t_point *p, t_texture *t)
{
	int	s_x;
	int	s_j;
	t_point			temp_p;
	t_texture 		temp_t;

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (p[s_j].y < p[s_j + 1].y)
			{
				temp_p = p[s_j];
				p[s_j] = p[s_j + 1];
				p[s_j + 1] = temp_p;

				temp_t = t[s_j];
				t[s_j] = t[s_j + 1];
				t[s_j + 1] = temp_t;
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

static void  calc_points_step(float x_step[2], t_texture t_step[2], t_point *p, t_texture *t, float delta)
{
	x_step[0] = (p[0].x - p[1].x) * delta;
	x_step[1] = (p[0].x - p[2].x) * delta;

	t_step[0].u = (t[0].u - t[1].u) * delta;
	t_step[0].v = (t[0].v - t[1].v) * delta;
	t_step[0].w = (t[0].w - t[1].w) * delta;

	t_step[1].u = (t[0].u - t[2].u) * delta;
	t_step[1].v = (t[0].v - t[2].v) * delta;
	t_step[1].w = (t[0].w - t[2].w) * delta;
}

static t_texture calc_step_texture(t_texture *t, float delta)
{	
	t_texture step;
	step.u = (t[2].u - t[1].u) * delta;
	step.v = (t[2].v - t[1].v) * delta;
	step.w = (t[2].w - t[1].w) * delta;
	return(step);
}

static void fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle, t_img *img)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				x;
	int				y;
	float			delta;

	p = triangle.p;
	t = triangle.t;
	calc_points_step(step, t_step, p, t, 1.0f / ((float)(p[1].y - p[0].y)));
	y = p[1].y;
	while (y >= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(p[1].y - y));
		int ax =  p[2].x + (step[1] * (float)(p[1].y - y));
		t_step[2] = calc_step_texture(t, 1.0f / (float)(ax - x));
		t[0].w = t[1].w;
		while(x <= ax)
		{
			if (t[0].w > sdl->zbuffer[x + y * sdl->window_w])
			{
				sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
			}
			t[0].w += t_step[2].w;
			x++;
		}
		t[1].w += t_step[0].w;

		t[2].w += t_step[1].w;
		y--;
	}
}


static void fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle, t_img *img)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				x;
	int				y;
	float			delta;

	p = triangle.p;
	t = triangle.t;
	calc_points_step(step, t_step, p, t, 1.0f/((float)(p[0].y - p[1].y)));
	y = p[1].y;
	while (y <= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(y - p[1].y));
		int ax =  p[2].x + (step[1] * (float)(y - p[1].y));
		t_step[2] = calc_step_texture(t, 1.0f / (float)(ax - x));
		t[0].w = t[1].w;
		while(x <= ax)
		{
			if (t[0].w > sdl->zbuffer[x + y * sdl->window_w])
			{
				sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
			}
			t[0].w += t_step[2].w;
			x++;
		}
		t[1].w += t_step[0].w;

		t[2].w += t_step[1].w;
		y++;
	}
}

/*
creates two triangles from the given triangle one flat top and one flat bottom.
both triangles are then assigned to t_point p[3] array and passed onto fill_tri_bot/top functions.
p[0] is always the pointy head of the triangle p[1] and p[2] are flat points where, p[1] x is smaller than p[2]
*/
void	render_z_triangle(t_sdlcontext *sdl, t_point_triangle triangle, t_img *img)
{
	t_point			p_split;
	t_texture		t_split;
	t_texture		t_temp;
	t_point			p_temp;
	t_point			*p;
	float			lerp;

	p = triangle.p;
	sort_point_tris(triangle.p, triangle.t);
	lerp = ((float)p[1].y - (float)p[2].y) / ((float)p[0].y - (float)p[2].y);
	p_split.x = p[2].x + (lerp * ((float)p[0].x - (float)p[2].x));
	p_split.y = p[1].y;
	t_split.w = ft_flerp(triangle.t[2].w, triangle.t[0].w, lerp);

	if (p_split.x < p[1].x)
	{
		ft_swap(&p[1], &p_split, sizeof(t_point));
		ft_swap(&triangle.t[1], &t_split, sizeof(t_texture));
	}
	p_temp = p[2];
	t_temp = triangle.t[2];
	p[2] = p_split;
	triangle.t[2] = t_split;
	if (p[0].y != p[1].y)
		fill_point_tri_top(sdl, triangle, img);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	if (p[0].y != p[1].y)
		fill_point_tri_bot(sdl, triangle, img);
}

static void draw_z_tri(t_sdlcontext *sdl, t_render *render)
{
	int index = 0;
	while (index < render->temp2_count)
	{
		render_z_triangle(sdl, render->temp2[index], render->img);
		index++;
	}
}

static void visible_fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle, t_lightmap *lightmap)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				x;
	int				y;
	float			delta;

	p = triangle.p;
	t = triangle.t;
	calc_points_step(step, t_step, p, t, 1.0f / ((float)(p[1].y - p[0].y)));
	y = p[1].y;
	while (y >= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(p[1].y - y));
		int ax =  p[2].x + (step[1] * (float)(p[1].y - y));
		t_step[2] = calc_step_texture(t, 1.0f / (float)(ax - x));
		t[0].u = t[1].u;
		t[0].v = t[1].v;
		t[0].w = t[1].w;
		while(x <= ax)
		{
			if (t[0].w >= sdl->zbuffer[x + y * sdl->window_w])
			{
				//sdl->zbuffer[x + y * sdl->window_w] = t[0].w;
				static uint8_t	x8b;
				static uint8_t	y8b;
				uint8_t	xsample;
				uint8_t	ysample;
				x8b = (t[0].u / t[0].w) * 255;
				xsample = (x8b * (lightmap->size.x - 1)) / 255;
				y8b = (t[0].v / t[0].w) * 255;
				ysample = (y8b * (lightmap->size.y - 1)) / 255;
				int result = ((500.0f - (1.0f / t[0].w)) / 500) * 255;
				result = ft_clamp(result, 0, 255);
				lightmap->data[(xsample * lightmap->size.x) + ysample] = result;
			}
			t[0].u += t_step[2].u;
			t[0].v += t_step[2].v;
			t[0].w += t_step[2].w;
			x++;
		}
		t[1].u += t_step[0].u;
		t[1].v += t_step[0].v;
		t[1].w += t_step[0].w;

		t[2].u += t_step[1].u;
		t[2].v += t_step[1].v;
		t[2].w += t_step[1].w;
		y--;
	}
}


static void visible_fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle, t_lightmap *lightmap)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	t_texture		t_step[3];
	int				x;
	int				y;
	float			delta;

	p = triangle.p;
	t = triangle.t;
	calc_points_step(step, t_step, p, t, 1.0f/((float)(p[0].y - p[1].y)));
	y = p[1].y;
	while (y <= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(y - p[1].y));
		int ax =  p[2].x + (step[1] * (float)(y - p[1].y));
		t_step[2] = calc_step_texture(t, 1.0f / (float)(ax - x));
		t[0].u = t[1].u;
		t[0].v = t[1].v;
		t[0].w = t[1].w;
		while(x <= ax)
		{
			if (t[0].w >= sdl->zbuffer[x + y * sdl->window_w])
			{
				static uint8_t	x8b;
				static uint8_t	y8b;
				uint8_t	xsample;
				uint8_t	ysample;
				x8b = (t[0].u / t[0].w) * 255;
				xsample = (x8b * (lightmap->size.x - 1)) / 255;
				y8b = (t[0].v / t[0].w) * 255;
				ysample = (y8b * (lightmap->size.y - 1)) / 255;
				int result = ((500.0f - (1.0f / t[0].w)) / 500) * 255;
				result = ft_clamp(result, 0, 255);
				lightmap->data[(xsample * lightmap->size.x) + ysample] = result;
			}
			t[0].u += t_step[2].u;
			t[0].v += t_step[2].v;
			t[0].w += t_step[2].w;
			x++;
		}
		t[1].u += t_step[0].u;
		t[1].v += t_step[0].v;
		t[1].w += t_step[0].w;

		t[2].u += t_step[1].u;
		t[2].v += t_step[1].v;
		t[2].w += t_step[1].w;
		y++;
	}
}

/*
creates two triangles from the given triangle one flat top and one flat bottom.
both triangles are then assigned to t_point p[3] array and passed onto fill_tri_bot/top functions.
p[0] is always the pointy head of the triangle p[1] and p[2] are flat points where, p[1] x is smaller than p[2]
*/
void	visible_render_z_triangle(t_sdlcontext *sdl, t_point_triangle triangle, t_lightmap *lightmap)
{
	t_point			p_split;
	t_texture		t_split;
	t_texture		t_temp;
	t_point			p_temp;
	t_point			*p;
	float			lerp;

	p = triangle.p;
	sort_point_tris(triangle.p, triangle.t);
	lerp = ((float)p[1].y - (float)p[2].y) / ((float)p[0].y - (float)p[2].y);
	p_split.x = p[2].x + (lerp * ((float)p[0].x - (float)p[2].x));
	p_split.y = p[1].y;
	t_split.u = ft_flerp(triangle.t[2].u, triangle.t[0].u, lerp);
	t_split.v = ft_flerp(triangle.t[2].v, triangle.t[0].v, lerp);
	t_split.w = ft_flerp(triangle.t[2].w, triangle.t[0].w, lerp);

	if (p_split.x < p[1].x)
	{
		ft_swap(&p[1], &p_split, sizeof(t_point));
		ft_swap(&triangle.t[1], &t_split, sizeof(t_texture));
	}
	p_temp = p[2];
	t_temp = triangle.t[2];
	p[2] = p_split;
	triangle.t[2] = t_split;
	if (p[0].y != p[1].y)
		visible_fill_point_tri_top(sdl, triangle, lightmap);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	if (p[0].y != p[1].y)
		visible_fill_point_tri_bot(sdl, triangle, lightmap);
}

static void visible_draw_z_tri(t_sdlcontext *sdl, t_render *render, t_lightmap *lightmap)
{
	int index = 0;
	while (index < render->temp2_count)
	{
		visible_render_z_triangle(sdl, render->temp2[index], lightmap);
		index++;
	}
}

static t_texture	vector2_to_texture(t_vector2 v)
{
	t_texture t;
	t.u = v.x;
	t.v = v.y;
	t.w = 1.0f;
	return(t);
}

void update_entity_lighting(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	int				index;
	t_object		*obj;
	t_quaternion	temp;

	obj = entity->obj;
	render->temp1_count = 0;
	render->temp2_count = 0;
	entity->lightmap = malloc(sizeof(t_lightmap) * entity->obj->material_count);
	index = 0;
	while (index < entity->obj->material_count)
	{
		entity->lightmap[index].size.x = entity->obj->materials[index].img->size.x;
		entity->lightmap[index].size.y = entity->obj->materials[index].img->size.y;
		entity->lightmap[index].data = malloc(sizeof(uint8_t) * entity->lightmap[index].size.x * entity->lightmap[index].size.y);
		memset(entity->lightmap[index].data, 0, sizeof(uint8_t) * entity->lightmap[index].size.x * entity->lightmap[index].size.y);
		index++;
	}
	if (obj == NULL)
		return;
	index = 0;
	while (index < obj->vertice_count)
	{
		temp.v = obj->vertices[index];
		if (entity->animation.active)
		{
			temp.v = vector3_add(entity->obj->o_anim.frames[entity->animation.frame].deltavertices[index].delta, temp.v);
		}
		render->q[index] = transformed_vector3(entity->transform, temp.v);
		render->q[index] = quaternion_mul_matrix(render->matworld, render->q[index]);
		index++;
	}
	index = 0;
	while (index < obj->face_count)
	{
		bool		draw = false;
		t_triangle	tritransformed;

		if (index + 1 == obj->face_count || obj->faces[index].materialindex != obj->faces[index + 1].materialindex)
			draw = true;
		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		if (obj->uv_count != 0 && !render->wireframe)
		{
			tritransformed.t[0] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[0] - 1]);
			tritransformed.t[1] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[1] - 1]);
			tritransformed.t[2] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[2] - 1]);
		}
		t_triangle clipped[2];
		int nclippedtriangles = clippedtriangles(tritransformed, render->matview, clipped);
		for (int n = 0; n < nclippedtriangles; n++)
			render->temp1[render->temp1_count++] = triangle_to_screenspace_point_triangle(render->matproj, clipped[n], sdl);
		if (draw == true)
		{
			clipped_point_triangle(render, sdl);
			visible_draw_z_tri(&sdl, render, &entity->lightmap[obj->faces[index].materialindex]);
			render->img = NULL;
			render->temp1_count = 0;
			render->temp2_count = 0;
		}
		index++;
	}
}

void render_entity_to_zbuffer(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	int				index;
	t_object		*obj;
	t_quaternion	temp;

	obj = entity->obj;
	render->temp1_count = 0;
	render->temp2_count = 0;
	if (obj == NULL)
		return;
	index = 0;
	while (index < obj->vertice_count)
	{
		temp.v = obj->vertices[index];
		if (entity->animation.active)
		{
			temp.v = vector3_add(entity->obj->o_anim.frames[entity->animation.frame].deltavertices[index].delta, temp.v);
		}
		render->q[index] = transformed_vector3(entity->transform, temp.v);
		render->q[index] = quaternion_mul_matrix(render->matworld, render->q[index]);
		index++;
	}
	index = 0;
	while (index < obj->face_count)
	{
		t_triangle	tritransformed;

		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		t_triangle clipped[2];
		int nclippedtriangles = clippedtriangles(tritransformed, render->matview, clipped);
		for (int n = 0; n < nclippedtriangles; n++)
			render->temp1[render->temp1_count++] = triangle_to_screenspace_point_triangle(render->matproj, clipped[n], sdl);
		index++;
	}
	clipped_point_triangle(render, sdl);
	draw_z_tri(&sdl, render);
}
