#include "doomnukem.h"

static void fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle)
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


static void fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle)
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
void	render_z_triangle(t_sdlcontext *sdl, t_point_triangle triangle)
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
		fill_point_tri_top(sdl, triangle);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	if (p[0].y != p[1].y)
		fill_point_tri_bot(sdl, triangle);
}

// triangle degined by vertices v0, v1 and  v2
t_vector3 triIntersect(t_vector3 ro, t_vector3 rd, t_vector3 v0, t_vector3 v1, t_vector3 v2)
{
	rd = vector3_normalise(rd);
    t_vector3 v1v0 = vector3_sub(v1, v0);
    t_vector3 v2v0 = vector3_sub(v2, v0);
    t_vector3 rov0 = vector3_sub(ro, v0);
    t_vector3  n = vector3_crossproduct(v1v0, v2v0);
    t_vector3  q = vector3_crossproduct(rov0, rd);
    float d = 1.0/vector3_dot(rd, n);
    float u = d*vector3_dot(vector3_negative(q), v2v0);
    float v = d*vector3_dot(q, v1v0);
    float t = d*vector3_dot(vector3_negative(n), rov0);
    //if( u<0.0 || v<0.0 || (u+v)>1.0 ) t = -1.0;
    
	return(t_vector3){t, u, v};
}

static float sign1 (t_point p1, t_point p2, t_point p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

static bool PointInTriangle (t_point pt, t_point v1, t_point v2, t_point v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign1(pt, v1, v2);
    d2 = sign1(pt, v2, v3);
    d3 = sign1(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

// point_area calculates the area of a triangle given three points
float point_area(t_point p1, t_point p2, t_point p3) {
  // Compute the lengths of the sides of the triangle using the distance formula


  float a = point_fdist(p1, p2);
  float b = point_fdist(p2, p3);
  float c = point_fdist(p3, p1);

  // Use Heron's formula to compute the area of the triangle
  float s = (a + b + c) / 2.0f;
  float temp = s * (s - a) * (s - b) * (s - c);
  printf("temp is %f\n", temp);
 // if (temp < 0.0f)
//	temp = -temp;
  float area = sqrt(temp);
  return area;
}


/*
If you want to use this for any set of points, you'll have to handle the special cases. As far as I can tell, this means the following: -
1) If the 3 vertices of the triangle are collinear. In this case p can't be inside since the triangle has no area (it's a degenerate triangle). 
You can determine collinearity by comparing the edge gradient pairs, but note that you have to compare all 3 pairs to handle the case where 2 vertices are on top of each other. 
Be mindful of floating point precision error and division by zero.
2) If C.y == A.y. If you rule out collinearity as per special case 1, then simply swap B and C.
I implemented a solution in C# that takes these cases into account and it handles everything I've thrown at it, including all special case variants I could think of.

When you have three side lengths satisfying a ≤ b ≤ c and the sum a + b = c, then the triangle is degenerate.

(Valid triangles have a + b > c and triangles with a + b < c are not possible.)
*/
bool is_valid_tri(t_point *p)
{
	float	dist[3];
	int		max_i;

	dist[0] = point_fdist(p[0], p[1]);
	dist[1] = point_fdist(p[1], p[2]);
	dist[2] = point_fdist(p[2], p[0]);
	max_i = 0;
	for (int i = 0; i < 3; i++)
	{
		if (dist[i] > dist[max_i])
			max_i = i;
	}
	if (dist[(max_i + 1) % 3] + dist[(max_i + 2) % 3] == dist[max_i])
		return(false);
	return(true);
}

static t_vector2 texcoord_to_loc(t_point *v, t_point *t, t_vector2 p)
{
    float       i;
    float       s;
    float       delta;
    t_vector2  	r;
    i = 1 / ((float)(t[1].x - t[0].x) * (float)(t[2].y - t[0].y) - (float)(t[1].y - t[0].y) * (float)(t[2].x - t[0].x));
    s = i * (float)((float)(t[2].y - t[0].y) * (float)(p.x - t[0].x) - (float)(t[2].x - t[0].x) * (float)(p.y - t[0].y));
    delta = i * (float)(-(float)(t[1].y - t[0].y) * (float)(p.x - t[0].x) + (float)(t[1].x - t[0].x) * (float)(p.y - t[0].y));
    r.x = v[0].x + s * (float)(v[1].x - v[0].x) + delta * (float)(v[2].x - v[0].x);
    r.y = v[0].y + s * (float)(v[1].y - v[0].y) + delta * (float)(v[2].y - v[0].y);
    return (r);
}


static void visible_fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	if (!is_valid_tri(triangle.p))
		return;
	t_point			*p;
	t_texture		*t;
	t_point			max;
	t_point			min;
	t_point			tmax;
	t_point			tmin;
	t_point			lt[3];
	float			dist[3];
	int				x;
	int				y;

	p = triangle.p;
	t = triangle.t;
	if (p[2].y == p[0].y)
	{
		ft_swap(&p[1], &p[2], sizeof(t_point));
		ft_swap(&t[1], &t[2], sizeof(t_texture));
	}
	max.x = -100000;
	max.y = -100000;
	min.x = 100000;
	min.y = 100000;
	for(int i = 0; i < 3; i++)
	{
		lt[i].x = (t[i].u/t[i].w) * (render->img->size.x - 1);
		lt[i].y = (t[i].v/t[i].w) * (render->img->size.y - 1);
		//printf("u %f v %f w %f %f %f\n", t[i].u, t[i].v, t[i].w, (t[i].u/t[i].w), (t[i].v/t[i].w));
		if (lt[i].x > max.x)
			max.x = lt[i].x;
		if (lt[i].y > max.y)
			max.y = lt[i].y;
		if (lt[i].x < min.x)
			min.x = lt[i].x;
		if (lt[i].y < min.y)
			min.y = lt[i].y;
	}
	//printf("max %d %d\nmin %d %d\n\n", max.x, max.y, min.x, min.y);
	//printf("lightmap size smaller x %d y %d, max x %d y %d\n", render->lightmap->size.x, render->lightmap->size.y, max.x, max.y);
	if (render->lightmap->size.x < max.x || render->lightmap->size.y < max.y)
	{
		printf("lightmap size smaller x %d y %d, max x %d y %d\n", render->lightmap->size.x, render->lightmap->size.y, max.x, max.y);
		exit(0);
	}
	if (!is_valid_tri(lt))
		return;
	y = min.y;
	while (y < max.y)
	{
		x = min.x;
		while (x < max.x)
		{
			if (PointInTriangle((t_point){x, y}, lt[0], lt[1], lt[2]))
			{
				/**/
				t_vector2 test1 = texcoord_to_loc(p, lt, (t_vector2){x, y});

				float w1 = (p[0].x * (p[2].y - p[0].y) + (test1.y - p[0].y) * (p[2].x - p[0].x) - test1.x * (p[2].y - p[0].y)) / (float)((p[1].y - p[0].y) * (p[2].x - p[0].x) - (p[1].x - p[0].x) * (p[2].y - p[0].y));
				float w2 = (test1.y - p[0].y - w1 * (p[1].y - p[0].y))/(float)(p[2].y - p[0].y);
				if (isinf(w1) || isnan(w1) || isinf(w2) || isnan(w2))
				{
					printf("first2\n");
					printf("p0 %d p1 %d p2 %d\n", p[0].x, p[1].x, p[2].x);
					printf("p0 %d p1 %d p2 %d\n", p[0].y, p[1].y, p[2].y);
					printf("error on either w1 %f w2 %f", w1, w2);
					ft_swap(&lt[1], &lt[2], sizeof(t_point));
					ft_swap(&p[1], &p[2], sizeof(t_point));
					ft_swap(&t[1], &t[2], sizeof(t_texture));
					if (!is_valid_tri(p))
						exit(0);
					w1 = (p[0].x * (p[2].y - p[0].y) + (test1.y - p[0].y) * (p[2].x - p[0].x) - test1.x * (p[2].y - p[0].y)) / (float)((p[1].y - p[0].y) * (p[2].x - p[0].x) - (p[1].x - p[0].x) * (p[2].y - p[0].y));
					w2 = (test1.y - p[0].y - w1 * (p[1].y - p[0].y))/(float)(p[2].y - p[0].y);
					if (isinf(w1) || isnan(w1) || isinf(w2) || isnan(w2))
						return;
				}
				//printf("second w1 %f w2 %f\n", w1, w2);
				float da;
				float db;

				da = t[1].w - t[0].w;
				db = t[2].w - t[0].w;
				da = da * w1;
				db = db * w2;
				da = da + db;
				da = da + t[0].w;
				if (da >= sdl->zbuffer[(int)test1.x + (int)test1.y * sdl->window_w] * 0.99f)
				{
					render->lightmap->data[x + render->lightmap->size.x * y] = 255;//((500.0f - (1.0f / fa)) / 500) * 255; //TODO: ADD THIS 
					//printf("%d\n", render->lightmap->data[(x * render->lightmap->size.x) + y]);
				}
				/*
				float w1 = ((lt[0].x * (lt[2].y - lt[0].y) + (y - lt[0].y) * (lt[2].x - lt[0].x) - x * (lt[2].y - lt[0].y)) / (float)((lt[1].y - lt[0].y) * (lt[2].x - lt[0].x) - (lt[1].x - lt[0].x) * (lt[2].y - lt[0].y)));
				float w2 = ((y - lt[0].y - w1 * (lt[1].y - lt[0].y))/(float)(lt[2].y - lt[0].y));
				if (isinf(w1) || isnan(w1) || isinf(w2) || isnan(w2))
				{
					
					printf("first\n");
					printf("lt0 %d lt1 %d lt2 %d\n", lt[0].x, lt[1].x, lt[2].x);
					printf("lt0 %d lt1 %d lt2 %d\n", lt[0].y, lt[1].y, lt[2].y);
					printf("error on either w1 %f w2 %f\n", w1, w2);
					ft_swap(&lt[1], &lt[2], sizeof(t_point));
					ft_swap(&p[1], &p[2], sizeof(t_point));
					ft_swap(&t[1], &t[2], sizeof(t_texture));
					if (!is_valid_tri(lt))
						return;
					w1 = ((lt[0].x * (lt[2].y - lt[0].y) + (y - lt[0].y) * (lt[2].x - lt[0].x) - x * (lt[2].y - lt[0].y)) / (float)((lt[1].y - lt[0].y) * (lt[2].x - lt[0].x) - (lt[1].x - lt[0].x) * (lt[2].y - lt[0].y)));
					w2 = ((y - lt[0].y - w1 * (lt[1].y - lt[0].y)) / (float)(lt[2].y - lt[0].y));
					if (isinf(w1) || isnan(w1) || isinf(w2) || isnan(w2))
						exit(0);
				}
				//printf("first w1 %f w2 %f\n", w1, w2);
				t_point a = point_sub(p[1], p[0]);
				t_point b = point_sub(p[2], p[0]);
				a = point_fmul(a, w1);
				b = point_fmul(b, w2);
				a = point_add(a, b);
				a = point_add(a, p[0]);

				float fa;
				float fb;

				fa = t[1].w - t[0].w;
				fb = t[2].w - t[0].w;
				fa = fa * w1;
				fb = fb * w2;
				fa = fa + fb;
				fa = fa + t[0].w;

				w1 = (p[0].x * (p[2].y - p[0].y) + (a.y - p[0].y) * (p[2].x - p[0].x) - a.x * (p[2].y - p[0].y)) / (float)((p[1].y - p[0].y) * (p[2].x - p[0].x) - (p[1].x - p[0].x) * (p[2].y - p[0].y));
				w2 = (a.y - p[0].y - w1 * (p[1].y - p[0].y))/(float)(p[2].y - p[0].y);
				if (isinf(w1) || isnan(w1) || isinf(w2) || isnan(w2))
				{
					printf("first2\n");
					printf("p0 %d p1 %d p2 %d\n", p[0].x, p[1].x, p[2].x);
					printf("p0 %d p1 %d p2 %d\n", p[0].y, p[1].y, p[2].y);
					printf("error on either w1 %f w2 %f", w1, w2);
					ft_swap(&lt[1], &lt[2], sizeof(t_point));
					ft_swap(&p[1], &p[2], sizeof(t_point));
					ft_swap(&t[1], &t[2], sizeof(t_texture));
					if (!is_valid_tri(p))
						return;
					w1 = (p[0].x * (p[2].y - p[0].y) + (a.y - p[0].y) * (p[2].x - p[0].x) - a.x * (p[2].y - p[0].y)) / (float)((p[1].y - p[0].y) * (p[2].x - p[0].x) - (p[1].x - p[0].x) * (p[2].y - p[0].y));
					w2 = (a.y - p[0].y - w1 * (p[1].y - p[0].y))/(float)(p[2].y - p[0].y);
					if (isinf(w1) || isnan(w1) || isinf(w2) || isnan(w2))
						return;
				}
				//printf("second w1 %f w2 %f\n", w1, w2);
				float da;
				float db;

				da = t[1].w - t[0].w;
				db = t[2].w - t[0].w;
				da = da * w1;
				db = db * w2;
				da = da + db;
				da = da + t[0].w;
				if (da >= sdl->zbuffer[a.x + a.y * sdl->window_w] * 0.99f)
				{
					render->lightmap->data[x + render->lightmap->size.x * y] = 255;//((500.0f - (1.0f / fa)) / 500) * 255; //TODO: ADD THIS 
					//printf("%d\n", render->lightmap->data[(x * render->lightmap->size.x) + y]);
				}
				*/
				/*if (x % 2 == 0)
					render->lightmap->data[(x * render->img->size.x) + y] = 255;
				else
					render->lightmap->data[(x * render->img->size.x) + y] = 0;
				*/

			}
			x++;
		}
		y++;
	}
}

/*
*/
static void clip_draw_lightmap(t_sdlcontext *sdl, t_render *render)
{
	int			i;

	clipped_point_triangle(render, *sdl);
	printf("lightmap size %d %d\n", render->lightmap->size.x, render->lightmap->size.y);
	printf("img size %d %d\n", render->img->size.x, render->img->size.y);
	i = 0;
	while (i < render->screenspace_ptri_count)
	{
		visible_fill_point_tri_top(sdl, render->screenspace_ptris[i], render);
		//visible_render_z_triangle(sdl, render->screenspace_ptris[i], render);
		i++;
	}
}

typedef	struct s_rgb
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} t_rgb;

typedef struct s_color
{
	union cdata_u
	{
		t_rgb		rgb;
		uint32_t	color;
	} dat;
}	t_color;

static uint32_t	flip_channels(uint32_t clr)
{
	t_color		result;
	t_color		orig;
	orig.dat.color = clr;
	result.dat.rgb.r = orig.dat.rgb.b;
	result.dat.rgb.g = orig.dat.rgb.g;
	result.dat.rgb.b = orig.dat.rgb.r;
	result.dat.rgb.a = orig.dat.rgb.a;
	return (result.dat.color);
}

void update_arealights_for_entity(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	int				index;
	t_object		*obj;
	t_quaternion	temp;
	t_vector2		max;


	obj = entity->obj;
	entity->lightmap = malloc(sizeof(t_lightmap) * entity->obj->material_count);
	entity->map	= malloc(sizeof(t_map) * entity->obj->material_count);
	render_worldspace(render, entity);
	max.x = -10000.0f;
	max.y = -10000.0f;
	index = 0;
	while (index < obj->face_count)
	{
		t_triangle	tritransformed;

		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		if (obj->uv_count != 0 && !render->wireframe)
		{
			tritransformed.t[0] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[0] - 1]);
			tritransformed.t[1] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[1] - 1]);
			tritransformed.t[2] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[2] - 1]);
			for (int j = 0; j < 3; j++)
			{
				if (tritransformed.t[j].u > max.x)
					max.x = tritransformed.t[j].u;
				if (tritransformed.t[j].v > max.y)
					max.y = tritransformed.t[j].v;
			}
		}
		t_triangle clipped[2];
		tritransformed = triangle_to_viewspace(tritransformed, render->camera.matview);
			int nclippedtriangles = clip_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, tritransformed, clipped);
			for (int n = 0; n < nclippedtriangles; n++)
				render->worldspace_ptris[render->worldspace_ptri_count++] = triangle_to_screenspace_point_triangle(render->camera.matproj, clipped[n], sdl);
		if (index + 1 == obj->face_count || obj->faces[index].materialindex != obj->faces[index + 1].materialindex)
		{
			t_lightmap	*lightmap;
			int temp = obj->faces[index].materialindex;
			lightmap = &entity->lightmap[obj->faces[index].materialindex];
			render->lightmap = lightmap;
			render->img = obj->materials[obj->faces[index].materialindex].img;
			if (!render->img)
			{
				index++;
				continue;
			}
			if (max.x > 1.0f)
				lightmap->size.x = max.x * render->img->size.x;
			else
				lightmap->size.x = render->img->size.x;
			if (max.y > 1.0f)
				lightmap->size.y = max.y * render->img->size.y;
			else
				lightmap->size.y = render->img->size.y;
			lightmap->data = malloc(sizeof(uint8_t) * lightmap->size.x * lightmap->size.y);
			memset(lightmap->data, 50, sizeof(uint8_t) * lightmap->size.x * lightmap->size.y);
			clip_draw_lightmap(&sdl, render);
			entity->map[temp].size.x = lightmap->size.x;
			entity->map[temp].size.y = lightmap->size.y;
			entity->map[temp].img.size.x = render->img->size.x;
			entity->map[temp].img.size.y = render->img->size.y;
			entity->map[temp].img.data = malloc(sizeof(uint32_t) * entity->map[temp].size.x * entity->map[temp].size.y);
			for (int i = 0; i < entity->map[temp].size.y; i++)
			{
				for (int j = 0; j < entity->map[temp].size.x; j++)
				{
					uint32_t	clr;
					clr = render->img->data[(i % render->img->size.y) * render->img->size.x + (j % render->img->size.x)];
					uint8_t light = render->lightmap->data[i * entity->map[temp].size.x + j];
					Uint32 alpha = clr&0xFF000000;
					Uint32 red=  ((clr&0x00FF0000)*light)>>8;
					Uint32 green= ((clr&0x0000FF00)*light)>>8;
					Uint32 blue=((clr&0x000000FF)*light)>>8;
					clr = flip_channels(alpha|(red&0x00FF0000)|(green&0x0000FF00)|(blue&0x000000FF));
					entity->map[temp].img.data[i * entity->map[temp].size.x + j] = clr;
				}
			}
			render->img = NULL;
			render->lightmap = NULL;
			render->map.img.data = NULL;
			render->worldspace_ptri_count = 0;
			render->screenspace_ptri_count = 0;
			max.x = -10000.0f;
			max.y = -10000.0f;
		}
		index++;
	}
}

void clip_draw_buffer(t_sdlcontext *sdl, t_render *render)
{
	int	i;

	i = 0;
	clipped_point_triangle(render, *sdl);
	while (i < render->screenspace_ptri_count)
	{
		render_z_triangle(sdl, render->screenspace_ptris[i]);
		i++;
	}
	render->img = NULL;
	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
}

void render_entity_depth_buffer(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	int				index;
	t_object		*obj;
	t_quaternion	temp;

	render_worldspace(render, entity);
	obj = entity->obj;
	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
	index = 0;
	while (index < obj->face_count)
	{
		t_triangle	tritransformed;

		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		t_triangle clipped[2];
		tritransformed = triangle_to_viewspace(tritransformed, render->camera.matview);
		int nclippedtriangles = clip_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, tritransformed, clipped);
		for (int n = 0; n < nclippedtriangles; n++)
			render->worldspace_ptris[render->worldspace_ptri_count++] = triangle_to_screenspace_point_triangle(render->camera.matproj, clipped[n], sdl);
		index++;
	}
	clip_draw_buffer(&sdl, render);
}

//not in use
void	visible_render_z_triangle(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
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
	printf("1\n");
	if (p[0].y != p[1].y)
		visible_fill_point_tri_top(sdl, triangle, render);
	p[0] = p_temp;
	triangle.t[0] = t_temp;
	printf("2\n");
	if (p[0].y != p[1].y)
		visible_fill_point_tri_top(sdl, triangle, render);
	printf("3\n");
}