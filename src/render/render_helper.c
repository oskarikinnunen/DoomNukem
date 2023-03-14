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

void	sort_point_uv_tri(t_vector2 *p, t_vector3 *t)
{
	int			s_x;
	int			s_j;
	t_vector2	temp_p;
	t_vector3	temp_t;

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

t_point_triangle	wf_tri(t_point_triangle in, float scaling)
{
	t_point_triangle	res;

	res = in;
	res.p[0] = vector2_div(in.p[0], scaling);
	res.p[1] = vector2_div(in.p[1], scaling);
	res.p[2] = vector2_div(in.p[2], scaling);

	return (res);
}

inline static int flat_tri(t_vector2 p[3], t_vector3 t[3])
{
	if (p[1].y == p[2].y)
	{
		if (p[1].x > p[2].x)
		{
			ft_swap(&p[1], &p[2], sizeof(t_vector2));
			ft_swap(&t[1], &t[2], sizeof(t_vector3));
		}
		return(0);
	}
	else if (p[0].y == p[1].y)
	{
		ft_swap(&p[0], &p[2], sizeof(t_vector2));
		ft_swap(&t[0], &t[2], sizeof(t_vector3));
		if (p[1].x > p[2].x)
		{
			ft_swap(&p[1], &p[2], sizeof(t_vector2));
			ft_swap(&t[1], &t[2], sizeof(t_vector3));
		}
		return(1);
	}
	return(2);
}

inline int	triangle_to_flat(t_point_triangle triangle, t_point_triangle tris[2])
{
	t_vector2			p_split;
	t_vector3			t_split;
	t_vector2			*p;
	t_vector3			*t;

	p = triangle.p;
	t = triangle.t;
	sort_point_uv_tri(p, t);
	int res = flat_tri(p, t);
	if (res != 2)
	{
		tris[0] = triangle;
		return (res);
	}
	float delta = (p[1].y - p[2].y) / (p[0].y - p[2].y);

	p_split.x = ft_flerp(p[2].x, p[0].x, delta);
	p_split.y = p[1].y;
	t_split = vector3_lerp(t[2], t[0], delta);
	if (p_split.x < p[1].x)
	{
		ft_swap(&p[1], &p_split, sizeof(t_vector2));
		ft_swap(&t[1], &t_split, sizeof(t_vector3));
	}
	tris[1] = triangle;
	p[2] = p_split;
	t[2] = t_split;
	tris[0] = triangle;
	ft_swap(&tris[1].p[0], &tris[1].p[2], sizeof(t_vector2));
	ft_swap(&tris[1].t[0], &tris[1].t[2], sizeof(t_vector3));
	tris[1].p[2] = p_split;
	tris[1].t[2] = t_split;
	return(res);
}
