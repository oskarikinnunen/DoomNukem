/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/19 19:36:52 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"

static int get_max_min_vector3(t_vector3 *max, t_vector3 *min, t_object *obj)
{
	t_vector3	v;
	int			i;

	max->x = -1000000000;
	max->y = -1000000000;
	max->z = -1000000000;
	min->x = 1000000000;
	min->y = 1000000000;
	min->z = 1000000000;
	i = 0;
	while (i < obj->vertice_count)
	{
		v = obj->vertices[i];
		if (v.x < min->x)
			min->x = v.x;
		if (v.x > max->x)
			max->x = v.x;
		if (v.y < min->y)
			min->y = v.y;
		if (v.y > max->y)
			max->y = v.y;
		if (v.z < min->z)
			min->z = v.z;
		if (v.z > max->z)
			max->z = v.z;
		
		i++;
	}
	return(i);
}

static void	calculate_entity_boundingbox(t_entity *e)
{
	t_vector3 min;
	t_vector3 max;
	int temp;
	temp = get_max_min_vector3(&max, &min, e->obj);

	e->boundingbox.boundingbox[0] = (t_vector3){max.x, max.y, min.z};
	e->boundingbox.boundingbox[1] = (t_vector3){max.x, min.y, min.z};
	e->boundingbox.boundingbox[2] = (t_vector3){min.x, min.y, min.z};
	e->boundingbox.boundingbox[3] = (t_vector3){min.x, max.y, min.z};

	e->boundingbox.boundingbox[4] = (t_vector3){max.x, max.y, max.z};
	e->boundingbox.boundingbox[5] = (t_vector3){max.x, min.y, max.z};
	e->boundingbox.boundingbox[6] = (t_vector3){min.x, min.y, max.z};
	e->boundingbox.boundingbox[7] = (t_vector3){min.x, max.y, max.z};

	e->boundingbox.height = max.z - min.z;
}

static float flerp(float from, float to, float delta)
{
	return (from + ((to - from) * delta));
}

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

static void sort_quaternions_x(t_quaternion *q, int high)
{
	int i, j;
	t_quaternion key;
	for (i = 1; i < high; i++)
	{
		key = q[i];
		j = i - 1;
		while (j >= 0 && q[j].v.x > key.v.x)
		{
			q[j + 1] = q[j];
			j = j - 1;
		}
		q[j + 1] = key;
	}
}

static t_boundingbox get_entity_boundingbox_transformed(t_entity *e)
{
	t_boundingbox	boundingbox;
	t_vector3		temp;
	int				i;
	int				max;

	boundingbox.height = e->boundingbox.height;
	boundingbox.is_wall = e->boundingbox.is_wall;
	if (boundingbox.is_wall == 0)
		max = 8;
	else
		max = 4;
	i = 0;
	while (i < max)
	{
		temp = e->boundingbox.boundingbox[i];
		//temp       = vector3_add(temp, (t_vector3){0, 0, e->boundingbox.height});
		temp = vector3_mul_vector3(e->transform.scale, temp);
		temp = vector3_add(e->transform.location, temp);
		boundingbox.boundingbox[i] = temp;
		i++;
	}
	return(boundingbox);
}

static t_quaternion quaternion_to_matproj(t_render r, t_quaternion q, t_sdlcontext sdl)
{
	t_quaternion	proj_q;

	proj_q = quaternion_mul_matrix(r.matproj, q);


	proj_q.v = vector3_div(proj_q.v, proj_q.w);

	proj_q.v = vector3_negative(proj_q.v);

	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, voffsetview);

	proj_q.v.x *= 0.5f * (float)sdl.window_w;
	proj_q.v.y *= 0.5f * (float)sdl.window_h;

	return(proj_q);
}

float	ft_fclamp(float val, float min, float max)
{
	if (val < min)
		return (min);
	if (val > max)
		return (max);
	return (val);
}

static int boundingbox_to_screenspace(t_render *render, t_sdlcontext sdl, t_entity *entity, t_quaternion q[5])
{
	t_boundingbox	boundingbox;
	t_quaternion	valid_q[5];
	uint32_t		valid_count;
	float			dist[4];
	int				max;
	int				i;

	valid_count = 0;
	boundingbox = get_entity_boundingbox_transformed(entity);
	if (boundingbox.is_wall == false)
		max = 4;
	else
		max = 2;
	i = 0;
	while (i < max)
	{
		q[i] = vector3_to_quaternion(boundingbox.boundingbox[i]);
		q[i] = quaternion_mul_matrix(render->matworld, q[i]);
		q[i] = quaternion_mul_matrix(render->matview, q[i]);
		dist[i] = vector3_fdist_to_plane(q[i].v, (t_vector3){0.0f, 0.0f, 1.0f}, (t_vector3){0.0f, 0.0f, 0.1f});
		i++;
	}
	float t;
	i = 0;
	while (i < max)
	{
		if (dist[i] < 0.0f)
		{
			if (entity->boundingbox.is_wall == false)
			{
				if (i == 0 || i == 2)
				{
					if (dist[1] > 0.0f)
					{
						valid_q[valid_count++] = quaternion_intersectplane((t_vector3){0.0f, 0.0f, 0.1f}, (t_vector3){0.0f, 0.0f, 1.0f}, q[i], q[1], &t);
					}
					if (dist[3] > 0.0f)
					{
						valid_q[valid_count++] = quaternion_intersectplane((t_vector3){0.0f, 0.0f, 0.1f}, (t_vector3){0.0f, 0.0f, 1.0f}, q[i], q[3], &t);
					}
				}
				else
				{
					if (dist[0] > 0.0f)
					{
						valid_q[valid_count++] = quaternion_intersectplane((t_vector3){0.0f, 0.0f, 0.1f}, (t_vector3){0.0f, 0.0f, 1.0f}, q[i], q[0], &t);
					}
					if (dist[2] > 0.0f)
					{
						valid_q[valid_count++] = quaternion_intersectplane((t_vector3){0.0f, 0.0f, 0.1f}, (t_vector3){0.0f, 0.0f, 1.0f}, q[i], q[2], &t);
					}
				}
			}
			else
			{
				if (i == 0)
				{
					if (dist[1] > 0.0f)
					{
						valid_q[valid_count++] = quaternion_intersectplane((t_vector3){0.0f, 0.0f, 0.1f}, (t_vector3){0.0f, 0.0f, 1.0f}, q[i], q[1], &t);
					}
				}
				else
				{
					if (dist[0] > 0.0f)
					{
						valid_q[valid_count++] = quaternion_intersectplane((t_vector3){0.0f, 0.0f, 0.1f}, (t_vector3){0.0f, 0.0f, 1.0f}, q[i], q[0], &t);
					}
				}
			}
		}
		else
		{
			valid_q[valid_count++] = q[i];
		}
		i++;
	}
	if (valid_count == 0)
		return(0);
	i = 0;
	while (i < valid_count)
	{
		q[i] = valid_q[i];
		q[i] = quaternion_to_matproj(*render, q[i], sdl);
		i++;
	}
	sort_quaternions_x(q, max);
	if ((q[0].v.x < 0.0f && q[max - 1].v.x < 0.0f) || (q[0].v.x > sdl.window_w - 1 && q[max - 1].v.x > sdl.window_w - 1) || (q[0].w < 0.0f && q[max - 1].w < 0.0f))
		return(0);
	if (q[0].w < 0.0f)
		q[0].v.x = 0;
	else
		q[0].v.x = ft_fclamp(q[0].v.x, 0, sdl.window_w - 1);
	q[0].v.y = ft_fclamp(q[0].v.y, 0, sdl.window_h - 1);
	if (q[max - 1].w < 0.0f)
	{
		q[max - 1].v.x = sdl.window_w - 1;
	}
	else
		q[max -1].v.x = ft_fclamp(q[max - 1].v.x, 0, sdl.window_w - 1);
	q[max -1].v.y = ft_fclamp(q[max - 1].v.y, 0, sdl.window_h - 1);
	return(1);
}

static int check_occlusion(t_render *render, t_sdlcontext sdl, t_entity *entity)
{
	t_quaternion	q[5];
	int				max;
	int				i;

	if (boundingbox_to_screenspace(render, sdl, entity, q) == 0)
		return(0);
	if (entity->boundingbox.is_wall == 0)
		max = 4;
	else
		max = 2;
	int ret = 0;
	float temp_x = q[0].v.x;
	float temp_w = q[0].w;
	float delta;
	while (temp_x < q[max - 1].v.x)
	{
		if (temp_x > 0.0f && temp_x < sdl.window_w && (int)temp_w <= render->z_occlusion[(int)temp_x])
		{
			ret = 1;
			if (entity->boundingbox.is_wall)
				render->debug_occlusion[(int)temp_x] = INT_MAX;
			else
				render->debug_occlusion[(int)temp_x] = CLR_GREEN;
		}
		else if (flerp(q[0].v.y, q[max - 1].v.y, (temp_x - q[0].v.x)/(q[max - 1].v.x - q[0].v.x)) <= render->y_occlusion[(int)temp_x])
			ret = 1;
		temp_x++;
		delta = (temp_x - q[0].v.x)/(q[max - 1].v.x - q[0].v.x);
		temp_x = flerp(q[0].v.x, q[max - 1].v.x, delta);
		temp_w = flerp(q[0].w, q[max - 1].w, delta);
	}
	return (ret);
}

static void update_occlusion(t_render *render, t_sdlcontext sdl, t_entity *entity)
{
	t_quaternion	q[10];
	int				max;
	int				i;

	if (boundingbox_to_screenspace(render, sdl, entity, q) == 0)
		return;
	if (entity->boundingbox.is_wall == false)
		max = 4;
	else
		max = 2;
	int ret = 0;
	float temp_x = q[0].v.x;
	float temp_w = q[0].w;
	float delta;
	i = 0;
	while (temp_x < q[max - 1].v.x)
	{
		if (temp_x > 0.0f && temp_x < sdl.window_w && (int)temp_w < render->z_occlusion[(int)temp_x])
		{
			render->z_occlusion[(int)temp_x] = (int)temp_w;
			render->y_occlusion[(int)temp_x] = flerp(q[0].v.y, q[max - 1].v.y, (temp_x - q[0].v.x)/(q[max - 1].v.x - q[0].v.x));
			if (entity->boundingbox.is_wall)
				render->debug_occlusion[(int)temp_x] = INT_MAX;
			else
				render->debug_occlusion[(int)temp_x] = CLR_GREEN;
		}
		temp_x++;
		delta = (temp_x - q[0].v.x)/(q[max - 1].v.x - q[0].v.x);
		temp_x = flerp(q[0].v.x, q[max - 1].v.x, delta);
		temp_w = flerp(q[0].w, q[max - 1].w, delta);
	}
	/*
	step_x = flerp(start_q.v.x, end_q.v.x, 1.0f/(end_q.v.x - start_q.v.x));
	step_w = flerp(start_q.w, end_q.w, 1.0f/(end_q.v.x - start_q.v.x));
	while (start_q.v.x < end_q.v.x)
	{
		if (start_q.w < sdl.window_w)
		{
			render->z_occlusion[(int)start_q.v.x] = start_q.w;
			render->debug_occlusion[(int)start_q.v.x] = INT_MAX;
		}
		start_q.v.x += step_x;
		start_q.w += step_w;
	}
	*/
}

static int triangle_to_projection(t_sdlcontext sdl, t_render *render, t_triangle t, t_triangle	clipped[2])
{
	int	index;
	int	clipamount;
	int	i;

	index = 0;
	while(index < 3)
	{
		t.p[index] = quaternion_mul_matrix(render->matworld, t.p[index]);
		t.p[index] = quaternion_mul_matrix(render->matview, t.p[index]);
		index++;
	}
	clipamount = clip_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, t, clipped);
	i = 0;
	while (i < clipamount)
	{
		index = 0;
		while(index < 3)
		{
			clipped[i].p[index] = quaternion_mul_matrix(render->matproj, clipped[i].p[index]);
			clipped[i].p[index].v = vector3_div(clipped[i].p[index].v, clipped[i].p[index].w);
			clipped[i].p[index].v = vector3_negative(clipped[i].p[index].v);

			t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
			clipped[i].p[index].v = vector3_add(clipped[i].p[index].v, voffsetview);

			clipped[i].p[index].v.x *= 0.5f * (float)sdl.window_w;
			clipped[i].p[index].v.y *= 0.5f * (float)sdl.window_h;
			index++;
		}
		i++;
	}
	return (clipamount);
}

static float sign (t_vector3 p1, t_vector3 p2, t_vector3 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

static bool PointInTriangle (t_vector3 pt, t_triangle t)
{
    float d1, d2, d3;
    bool has_neg, has_pos;
	int	i;

	i = 0;
	while (i < 3)
	{
		if (pt.x == t.p[i].v.x && pt.y == t.p[i].v.y)
			return(false);
		i++;
	}
    d1 = sign(pt, t.p[0].v, t.p[1].v);
    d2 = sign(pt, t.p[1].v, t.p[2].v);
    d3 = sign(pt, t.p[2].v, t.p[0].v);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void tri_occluder(t_render *render, t_sdlcontext sdl, t_entity *entity)
{
	int				clipamount;
	int				index;
	int				max;
	int				add;
	int				i;
	t_boundingbox	b;
	t_triangle		t;
	t_triangle		clipped[2];
	t_quaternion	temp;
	uint32_t		id;

	//printf("\n\n\n\n\nOCCLUDER SETUP FUNCTION!\n\n\n\n\n");
	if (render->occluder_count != 0)
	{
		id = render->occluder[render->occluder_count - 1].id + 1;
	}
	else
		id = 0;
	render->tri_occluder_count = 0;
	b = get_entity_boundingbox_transformed(entity);
	if (b.is_wall == false)
	{
		//printf("ent\n\n");
		max = 4;
		add = 4;
	}
	else
	{
		max = 1;
		add = 2;
		//printf("wall\n\n");
	}
	i = 0;
	while (i < max)
	{
		t.p[0] = vector3_to_quaternion(b.boundingbox[i]);
		t.p[1] = vector3_to_quaternion(b.boundingbox[((i + 1) % 4) + add]);
		t.p[2] = vector3_to_quaternion(b.boundingbox[i + add]);
		clipamount = triangle_to_projection(sdl, render, t, clipped);
		index = 0;
		//printf("first clip %d\n\n", clipamount);
		while (index < clipamount)
		{
			render->tri_occluder[render->tri_occluder_count++] = clipped[index];
			//print_tri(render->tri_occluder[render->tri_occluder_count - 1]);
			index++;
		}
		t.p[2] = vector3_to_quaternion(b.boundingbox[(i + 1) % 4]);
		clipamount = triangle_to_projection(sdl, render, t, clipped);
		index = 0;
		//printf("second clip %d\n\n", clipamount);
		while (index < clipamount)
		{
			render->tri_occluder[render->tri_occluder_count++] = clipped[index];
			//print_tri(render->tri_occluder[render->tri_occluder_count - 1]);
			index++;
		}
		i++;
	}
	i = 0;
	//printf("occluder count start %d tri count %d\n", render->occluder_count, render->tri_occluder_count - start);
	while (i < render->tri_occluder_count)
	{
		bool distneg = false;
		bool distpos = false;
		float	dist[3];
		t_vector3 plane_p;
		t_vector3 plane_n;

		for (int e = 0; e < 3; e++)
		{
			plane_p = render->tri_occluder[i].p[e].v;
			plane_n = vector3_sub(plane_p, render->tri_occluder[i].p[(e + 1) % 3].v);
			plane_n.z = plane_n.x;
			plane_n.x = -plane_n.y;
			plane_n.y = plane_n.z;
			plane_n.z = 0.0f;
			float test = vector2_magnitude((t_vector2){plane_n.x, plane_n.y});
			plane_n = (t_vector3){.x = plane_n.x/test, .y = plane_n.y/test};
			//plane_n = vector3_normalise(plane_n);

			distneg = false;
			distpos = false;
			index = 0;
			while (index < render->tri_occluder_count)
			{
				/*
				if (ft_memcmp(&render->tri_occluder[index], &render->tri_occluder[i], sizeof(t_triangle)) == 0)
				{
					index++;
					continue;
				}*/
				dist[0] = roundf(vector3_fdist_to_plane(render->tri_occluder[index].p[0].v, plane_n, plane_p));
				dist[1] = roundf(vector3_fdist_to_plane(render->tri_occluder[index].p[1].v, plane_n, plane_p));
				dist[2] = roundf(vector3_fdist_to_plane(render->tri_occluder[index].p[2].v, plane_n, plane_p));
				//printf(" d1 %f d2 %f d3 %f\n", dist[0], dist[1], dist[2]);
				if (dist[0] >= 0.0f && dist[1] >= 0.0f && dist[2] >= 0.0f)
					distpos = true;
				else
				{
					if (dist[0] <= 0.0f && dist[1] <= 0.0f && dist[2] <= 0.0f)
						distneg = true;
					else
					{
						distneg = distpos;
						break;
					}
				}
				index++;
			}
			if (distneg != distpos)
			{
				if (distpos == true)
				{
					plane_n.x = -1.0f * plane_n.x;
					plane_n.y = -1.0f * plane_n.y;
					//flip normal
				}
				//printf("added\n");
				render->occluder[render->occluder_count].id = id;
				render->occluder[render->occluder_count].normal = plane_n;
				render->occluder[render->occluder_count++].vector = plane_p;
			}
			//else
				//printf("Didnt add\n");
		}
		i++;
	}
	//printf("occluder count end %d\n", render->occluder_count);
}

static int occluder(t_render *render)
{

}

int is_occluded_tri(t_render *render, t_sdlcontext sdl, t_entity *entity)
{
	int				clipamount;
	int				index;
	int				max;
	int				add;
	int				i;
	t_boundingbox	b;
	t_triangle		t;
	t_triangle		clipped[2];
	t_quaternion	temp;
	uint32_t		id;

	printf("\n\n\n\n\n\nCHECK OCCLUSION FUNCTION!\n\n\n");
	render->tri_occluder_count = 0;
	b = get_entity_boundingbox_transformed(entity);
	if (b.is_wall == false)
	{
		printf("ent\n\n");
		max = 4;
		add = 4;
	}
	else
	{
		max = 2;
		add = 1;
		printf("wall\n\n");
	}
	i = 0;
	while (i < max)
	{
		t.p[0] = vector3_to_quaternion(b.boundingbox[i]);
		t.p[1] = vector3_to_quaternion(b.boundingbox[((i + 1) % 4) + add]);
		t.p[2] = vector3_to_quaternion(b.boundingbox[i + add]);
		clipamount = triangle_to_projection(sdl, render, t, clipped);
		index = 0;
		while (index < clipamount)
		{
			render->tri_occluder[render->tri_occluder_count++] = clipped[index];
			index++;
		}
		t.p[2] = vector3_to_quaternion(b.boundingbox[(i + 1) % 4]);
		clipamount = triangle_to_projection(sdl, render, t, clipped);
		index = 0;
		while (index < clipamount)
		{
			render->tri_occluder[render->tri_occluder_count++] = clipped[index];
			index++;
		}
		i++;
	}
	//return(occluder(render));
	render->calc_tri_count = 0;
	id = 0;
	i = 0;
	while (i < render->occluder_count)
	{
		printf("occluder id %d %d %d\n", render->occluder[i].id, i, render->tri_occluder_count);
		if (id == render->occluder[i].id)
		{
			index = 0;
			while (index < render->tri_occluder_count)
			{
				clipamount = clip_triangle_against_plane(render->occluder[i].vector, render->occluder[i].normal, render->tri_occluder[index], clipped);

				for (int w = 0; w < clipamount; w++)
				{
					render->calc_triangles[render->calc_tri_count++] = clipped[w];
				}
				index++;
			}
		}
		else if (id + 1 == render->occluder[i].id)
		{
			if (render->occluder[i - 1].id == id)
			{
				render->tri_occluder_count = 0;
				if (render->calc_tri_count == 0)
				{
					printf("exit 1\n");
					return(1);
				}
			}
			index = 0;
			while (index < render->calc_tri_count)
			{
				clipamount = clip_triangle_against_plane(render->occluder[i].vector, render->occluder[i].normal, render->calc_triangles[index], clipped);

				for (int w = 0; w < clipamount; w++)
				{
					render->tri_occluder[render->tri_occluder_count++] = clipped[w];
				}
				index++;
			}
		}
		else
		{

			render->calc_tri_count = 0;
			if (render->tri_occluder_count == 0)
			{
				printf("exit 2\n");
				return(1);
			}
			id = render->occluder[i].id;
			continue;
		}
		i++;
	}
	if (render->occluder[i - 1].id == id)
		render->tri_occluder_count = 0;
	else
		render->calc_tri_count = 0;
	printf("end of function %d %d\n", render->calc_tri_count, render->tri_occluder_count);
	if (render->calc_tri_count > 0 || render->tri_occluder_count > 0)
		return(0);
	else
		return(1);
}

void render_world3d(t_sdlcontext sdl, t_world world, t_render *render)
{
	t_list			*l;
	t_entity		*ent;
	t_wall			wall;
	t_boundingbox	boundingbox;

/*
	for (int i = 0; i < sdl.window_h; i++)
	{
		for (int j = 0; j < sdl.window_w; j++)
		{
			((uint32_t *)sdl.surface->pixels)[j + (i * sdl.window_w)] = render->debug_img->data[j % 1000 * i % 1000];
			//draw(sdl, (t_point){j, i}, render->debug_img->data[j % 1000 * i % 1000]);
		}
	}
	return;*/
	//printf("%d\n", PointInTriangle((t_vector3){.x = 668.479187f, .y = 334.286407f}, (t_triangle){.p[0].v.x = 668.479187f, .p[0].v.y = 334.286407f, .p[1].v.x = 337.215668f, .p[1].v.y = 207.458160f, .p[2].v.x = 694.835144f, .p[2].v.y = 209.122528f}));
	memset(render->z_occlusion, INT_MAX, sizeof(uint32_t) * sdl.window_w);
	bzero(render->debug_occlusion, sizeof(uint32_t) * sdl.window_w);
	bzero(render->y_occlusion, sizeof(uint32_t) * sdl.window_w);
	bzero(&render->stats, sizeof(t_render_statistics));
	l = world.wall_list;
	render->rendermode = fill;
	while (l != NULL)
	{
		wall = *(t_wall *)l->content;
		update_occlusion(render, sdl, &wall.entity);
		l = l->next;
	}
	l = world.wall_list;
	render->rendermode = fill;
	render->tri_occluder_count = 0;
	render->occluder_count = 0;
	while (l != NULL)
	{
		wall = *(t_wall *)l->content;
		//if (check_occlusion(render, sdl, &wall.entity) == 1)
		tri_occluder(render, sdl, &wall.entity);
		l = l->next;
	}
	l = world.wall_list;
	while (l != NULL)
	{
		wall = *(t_wall *)l->content;
		//if (check_occlusion(render, sdl, &wall.entity) == 0)
		if (is_occluded_tri(render, sdl, &wall.entity) == 0)
			render_entity(sdl, render, &wall.entity);
		render->rendermode = fill;
		l = l->next;
	}
	
	l = world.entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		//if (check_occlusion(render, sdl, ent) == 0)	
		if (is_occluded_tri(render, sdl, ent) == 0)
			render_entity(sdl, render, ent);
		render->rendermode = fill;
		l = l->next;
	}
	if (0)
		for (int i = 0; i < sdl.window_w; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				draw(sdl, (t_point){i, sdl.window_h - 250 + j}, render->debug_occlusion[i]);
			}
		}
	print_render_statistics(render->stats);
}

void	calculate_colliders_for_entities(t_world *world)
{
	t_list *l;
	int		i;

	l = world->entitylist;
	i = 0;
	while (l != NULL)
	{
		world->physics.entities[i] = (t_entity *)l->content;
		l = l->next;
		i++;
	}
	world->physics.entities[i] = NULL;
	calculate_colliders(&world->physics);
}

static void	entity_init(t_world *world, t_sdlcontext sdl)
{
	t_list		*l;
	t_entity	*ent;

	l = world->entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		if (ent->object_index < sdl.objectcount)
			ent->obj = &sdl.objects[ent->object_index]; //TODO: getobjectbyindex
		else
			ent->obj = &sdl.objects[0];
		calculate_entity_boundingbox(ent);
		l = l->next;
	}
}

void	scale_skybox_uvs(t_object *obj)
{
	int	i;

	i = 0;
	while (i < obj->uv_count)
	{
		obj->uvs[i] = vector2_mul(obj->uvs[i], 10.0f);
		i++;
	}
}

static void load_walltextures(t_world *world, t_sdlcontext sdl)
{
	t_list		*l;
	t_wall		*wall;
	t_material	*mat;

	l = world->wall_list;
	while (l != NULL)
	{
		wall = (t_wall *)l->content;
		mat = &wall->entity.obj->materials[0];
		mat->img = get_image_by_name(sdl, mat->texturename);
		l = l->next;
	}
}

t_world	load_world(char *filename, t_sdlcontext sdl)
{
	t_world	world;

	ft_bzero(&world, sizeof(t_world));
	world.entitylist = load_chunk(filename, "ENT_", sizeof(t_entity));
	world.wall_list = load_chunk(filename, "WALL", sizeof(t_wall));
	entity_init(&world, sdl);
	walls_init(&world);
	load_walltextures(&world, sdl);
	ft_bzero(&world.skybox, sizeof(t_entity));
	world.skybox.obj = get_object_by_name(sdl, "cube");
	world.skybox.obj->materials[0].img = get_image_by_name(sdl, "grid.png");
	//scale_skybox_uvs(world.skybox.obj);
	world.skybox.transform.scale = vector3_mul(vector3_one(), 1000.0f);
	world.skybox.transform.location = (t_vector3){500.0f, 500.0f, 500.0f};
	calculate_colliders_for_entities(&world);
	return (world);
}


static int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1)
		error_log(EC_WRITE);
	return (fd);
}

void	save_world(char *filename, t_world world)
{
	int	fd;

	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	close(fd);
	save_chunk(filename, "ENT_", world.entitylist);
	save_chunk(filename, "WALL", world.wall_list);
	//unscale_skybox_uvs(world.skybox.obj);
}