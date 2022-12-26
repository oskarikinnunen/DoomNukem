#include "doomnukem.h"
/*
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

static void	sort_point_tris(t_point *p)
{
	int	s_x;
	int	s_j;
	t_point			temp_p;

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
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

static void  calc_points_step(float x_step[2], t_point *p, float delta)
{
	x_step[0] = (p[0].x - p[1].x) * delta;
	x_step[1] = (p[0].x - p[2].x) * delta;
}

float	point_crossproduct(t_point first, t_point second)
{
	return ((first.x * second.y) + (first.y * second.x));// -
}

float sign1(t_point p1, t_point p2, t_point p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (t_point pt, t_point v1, t_point v2, t_point v3)
{
	return(true);
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign1(pt, v1, v2);
    d2 = sign1(pt, v2, v3);
    d3 = sign1(pt, v3, v1);
	//return(true);
    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

static void fill_point_tri_top1(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			*p;
	t_point			pt;

	p = triangle.p;
	pt.y = p[1].y;
	while (pt.y <= p[0].y)
	{
		pt.x = p[1].x;
		while(pt.x <= p[2].x)
		{
			if(PointInTriangle(pt, p[0], p[1], p[2]))
				render->bitmask[(pt.y / 4) * (sdl->window_w / 8) + (pt.x / 8)] |= 1UL << ((pt.y % 4) * 8) + (pt.x % 8);
			pt.x++;
		}
		pt.y++;
	}
}

static void fill_point_tri_bot(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			*p;
	t_texture		*t;
	float			step[2];
	int				x;
	int				y;
	float			delta;

	p = triangle.p;
	t = triangle.t;
	calc_points_step(step, p, 1.0f / ((float)(p[1].y - p[0].y)));
	y = p[1].y;
	while (y >= p[0].y)
	{
		delta = (float)(p[1].y - y);
		x = p[1].x + (step[0] * delta);
		int ax =  p[2].x + (step[1] * delta);
		while(x <= ax)
		{
			//(render->bitmask[(y / 4) * (sdl->window_w / 8) + (x / 8)] >> ((y % 4) * 8) + (x % 8)
			//printf("1x is %d, y is %d\n", x, y);
			render->bitmask[(y / 4) * (sdl->window_w / 8) + (x / 8)] |= 1UL << ((y % 4) * 8) + (x % 8);
			x++;
		}
		y--;
	}
}


static void fill_point_tri_top(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			*p;
	float			step[2];
	int				x;
	int				y;
	float			delta;

	p = triangle.p;
	calc_points_step(step, p, 1.0f/((float)(p[0].y - p[1].y)));
	y = p[1].y;
	while (y <= p[0].y)
	{
		delta = (float)(y - p[1].y);
		x = p[1].x + (step[0] * delta);
		int ax =  p[2].x + (step[1] * delta);
		while(x <= ax)
		{
			//printf("x is %d, y is %d\n", x, y);
			render->bitmask[(y / 4) * (sdl->window_w / 8) + (x / 8)] |= 1UL << ((y % 4) * 8) + (x % 8);
			x++;
		}
		y++;
	}
}


//creates two triangles from the given triangle one flat top and one flat bottom.
//both triangles are then assigned to t_point p[3] array and passed onto fill_tri_bot/top functions.
//p[0] is always the pointy head of the triangle p[1] and p[2] are flat points where, p[1] x is smaller than p[2]

void	render_tri(t_sdlcontext *sdl, t_point_triangle triangle, t_render *render)
{
	t_point			p_split;
	t_point			p_temp;
	t_point			*p;
	float			lerp;

	p = triangle.p;
	sort_point_tris(triangle.p);
	lerp = ((float)p[1].y - (float)p[2].y) / ((float)p[0].y - (float)p[2].y);
	p_split.x = p[2].x + (lerp * ((float)p[0].x - (float)p[2].x));
	p_split.y = p[1].y;
	if (p_split.x < p[1].x)
		ft_swap(&p[1], &p_split, sizeof(t_point));
	p_temp = p[2];
	p[2] = p_split;
	if (p[0].y != p[1].y)
		fill_point_tri_top(sdl, triangle, render);
	p[0] = p_temp;
	if (p[0].y != p[1].y)
		fill_point_tri_bot(sdl, triangle, render);
}


static void draw_tri(t_render *render, t_sdlcontext *sdl)
{
	int i = 0;
	while (i < render->temp2_count)
	{	
		render_tri(sdl, render->temp2[i], render);
		i++;
	}
}

static void draw_tris(t_entity *entity, t_render *render, t_sdlcontext sdl)
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
		t_vector3	normal;	
		t_vector3	vcameraray;	

		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		t_triangle clipped[2];
		int nclippedtriangles = clippedtriangles(tritransformed, render->matview, clipped);
		for (int n = 0; n < nclippedtriangles; n++)
			render->temp1[render->temp1_count++] = triangle_to_screenspace_point_triangle(render->matproj, clipped[n], sdl);
		index++;
	}
	clipped_point_triangle(render, sdl);
	draw_tri(render, &sdl);
	render->rs.occluder_count++;
}

void update_bitmask_culling(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	draw_tris(entity, render, sdl);
	return;
    int			i;
	t_list		*l;
	t_entity	*ent;

	render->rs.occluder_count = 0;
	l = render->world->roomlist;
	while (l != NULL)
	{
		t_room room = *(t_room *)l->content;
		i = 0;
		while (i < room.wallcount)
		{
			ent = &room.walls[i].entity;
			draw_tris(ent, render, sdl);
			i++;
		}
		i = 0;
		while (i < room.floorcount)
		{
			i++;
		}
		l = l->next;
	}
	l = render->world->entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		//draw_tris(ent, render, sdl);
		l = l->next;
	}
	printf("count %d\n", render->rs.occluder_count);
}

bool is_entity_bitmask_culled(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
    return(false);
}*/