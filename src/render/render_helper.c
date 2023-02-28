#include "doomnukem.h"

t_point_triangle	ps1(t_point_triangle in, int div)
{
	t_point_triangle	res;

	res = in;
	res.p[0] = vector2_div(res.p[0], div);
	res.p[1] = vector2_div(res.p[1], div);
	res.p[2] = vector2_div(res.p[2], div);

	res.p[0] = vector2_mul(res.p[0], div);
	res.p[1] = vector2_mul(res.p[1], div);
	res.p[2] = vector2_mul(res.p[2], div);

	return (res);
}

void ft_swap(void * a, void * b, size_t len)
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

void	sort_point_uv_tri(t_vector2 *p, t_texture *t)
{
	int	s_x;
	int	s_j;
	t_vector2		temp_p;
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

void	sort_point_tri(t_vector2 *p2, float *w)
{
	int	s_x;
	int	s_j;
	t_vector2		temp_p2;
	float			temp_w;

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (p2[s_j].y < p2[s_j + 1].y)
			{
				temp_p2 = p2[s_j];
				p2[s_j] = p2[s_j + 1];
				p2[s_j + 1] = temp_p2;

				temp_w = w[s_j];
				w[s_j] = w[s_j + 1];
				w[s_j + 1] = temp_w;
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

void	sort_polygon_tri(t_vector2 *p2, t_vector2 *t, t_vector3 *p3)
{
	int	s_x;
	int	s_j;
	t_vector2		temp_p2;
	t_vector2 		temp_uv;
	t_vector3		temp_p3;

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (p2[s_j].y < p2[s_j + 1].y)
			{
				temp_p2 = p2[s_j];
				p2[s_j] = p2[s_j + 1];
				p2[s_j + 1] = temp_p2;

				temp_uv = t[s_j];
				t[s_j] = t[s_j + 1];
				t[s_j + 1] = temp_uv;

				temp_p3 = p3[s_j];
				p3[s_j] = p3[s_j + 1];
				p3[s_j + 1] = temp_p3;
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

void  calc_points_step(float x_step[2], t_texture t_step[2], t_vector2 *p, t_texture *t, float delta)
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

t_texture calc_step_texture(t_texture *t, float delta)
{	
	t_texture step;
	step.u = (t[2].u - t[1].u) * delta;
	step.v = (t[2].v - t[1].v) * delta;
	step.w = (t[2].w - t[1].w) * delta;
	return(step);
}

/*t_texture calc_step_texture2(t_texture *t, float delta)
{	
	t_texture step;
	step.u = (t[2].u - t[1].u) / delta;
	step.v = (t[2].v - t[1].v) / delta;
	step.w = (t[2].w - t[1].w) / delta;
	return(step);
}*/

t_point_triangle	wf_tri(t_point_triangle in, float scaling)
{
	t_point_triangle	res;

	res = in;
	res.p[0] = vector2_div(in.p[0], scaling);
	res.p[1] = vector2_div(in.p[1], scaling);
	res.p[2] = vector2_div(in.p[2], scaling);

	return (res);
}