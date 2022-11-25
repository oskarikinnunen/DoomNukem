#include "doomnukem.h"

static t_box get_entity_box_transformed(t_entity *e)
{
	t_vector3	temp;
	t_box		box;
	int			i;

	box = e->obj->bounds.box;
	i = 0;
	while (i < 8)
	{
		temp = box.v[i];
		temp = vector3_mul_vector3(e->transform.scale, temp);
		temp = vector3_add(e->transform.location, temp);
		box.v[i] = temp;
		i++;
	}
	return(box);
}

static void triangle_to_projection(t_sdlcontext sdl, t_render *render, t_triangle t, t_triangle	buff[2])
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
	clipamount = clip_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, t, buff);
	i = 0;
	while (i < clipamount)
	{
		index = 0;
		while(index < 3)
		{
			buff[i].p[index] = quaternion_mul_matrix(render->matproj, buff[i].p[index]);
			buff[i].p[index].v = vector3_div(buff[i].p[index].v, buff[i].p[index].w);
			buff[i].p[index].v = vector3_negative(buff[i].p[index].v);

			t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
			buff[i].p[index].v = vector3_add(buff[i].p[index].v, voffsetview);

			buff[i].p[index].v.x *= 0.5f * (float)sdl.window_w;
			buff[i].p[index].v.y *= 0.5f * (float)sdl.window_h;
			index++;
		}
		render->calc_triangles[render->calc_tri_count++] = buff[i];
		i++;
	}
}

float vector2_fdist_to_plane(t_vector2 p, t_vector2 plane_n, t_vector2 plane_p)
{
	t_vector2 n = vector2_normalise(p);
	return (plane_n.x * p.x + plane_n.y * p.y - vector2_dot(plane_n, plane_p));
}

void update_occlusion_culling(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	t_occlusion *occlusion;
	t_box		b;
	int			i;
	int			j;
	int			clipamount;
	t_triangle	t;
	t_triangle	buff[2];
	float		test;
	float		delta;

	occlusion = &entity->occlusion;

	occlusion->occluder_count = 0;
	render->draw_tri_count = 0;
	render->calc_tri_count = 0;
	b = get_entity_box_transformed(entity);

	i = 0;
	while (i < 4)
	{
		t.p[0] = vector3_to_quaternion(b.v[i]);
		t.p[1] = vector3_to_quaternion(b.v[i + 4]);
		t.p[2] = vector3_to_quaternion(b.v[((i + 1) % 4) + 4]);
		triangle_to_projection(sdl, render, t, buff);
		t.p[1] = vector3_to_quaternion(b.v[((i + 1) % 4)]);
		t.p[2] = vector3_to_quaternion(b.v[(i + 1) % 4]);
		triangle_to_projection(sdl, render, t, buff);
		i++;
	}
	clipped(render, sdl);
	i = 0;
	printf("triangle amount %d\n", render->draw_tri_count);
	while (i < render->draw_tri_count)
	{
		bool distneg = false;
		bool distpos = false;
		float	dist[3];
		t_vector3 plane_p;
		t_vector3 plane_n;

		for (int e = 0; e < 2; e++)
		{
			plane_p = render->draw_triangles[i].p[e].v;
			plane_n = vector3_sub(render->draw_triangles[i].p[(e + 1) % 3].v, plane_p);
			plane_n.z = plane_n.x;
			plane_n.x = -plane_n.y;
			plane_n.y = plane_n.z;
			plane_n.z = 0.0f;
			test = vector2_magnitude((t_vector2){plane_n.x, plane_n.y});
			plane_n = (t_vector3){.x = plane_n.x/test, .y = plane_n.y/test};

			distneg = false;
			distpos = false;
			j = 0;
			while (j < render->draw_tri_count)
			{
				dist[0] = roundf(vector2_fdist_to_plane((t_vector2){render->draw_triangles[j].p[0].v.x, render->draw_triangles[j].p[0].v.y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y}));
				dist[1] = roundf(vector2_fdist_to_plane((t_vector2){render->draw_triangles[j].p[1].v.x, render->draw_triangles[j].p[1].v.y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y}));
				dist[2] = roundf(vector2_fdist_to_plane((t_vector2){render->draw_triangles[j].p[2].v.x, render->draw_triangles[j].p[2].v.y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y}));
				//dist[0] = roundf(vector3_fdist_to_plane(render->draw_triangles[j].p[0].v, plane_n, plane_p));
				//dist[1] = roundf(vector3_fdist_to_plane(render->draw_triangles[j].p[1].v, plane_n, plane_p));
				//dist[2] = roundf(vector3_fdist_to_plane(render->draw_triangles[j].p[2].v, plane_n, plane_p));
			//	printf("dist0 %f dist1 %f dist2 %f\n", dist[0], dist[1], dist[2]);
				if (dist[0] >= 0.0f && dist[1] >= 0.0f && dist[2] >= 0.0f)
					distpos = true;
				else if (dist[0] <= 0.0f && dist[1] <= 0.0f && dist[2] <= 0.0f)
						distneg = true;
				else
				{
					distneg = distpos;
					break;
				}
				j++;
			}
			if (distneg != distpos)
			{
				printf("e is %d i is %d\n", e, i);
				occlusion->occluder[occlusion->occluder_count].normal[0] = plane_n;

				occlusion->occluder[occlusion->occluder_count].vector[0] = plane_p;
				occlusion->occluder[occlusion->occluder_count].vector[0].z = render->draw_triangles[i].p[e].w;
				occlusion->occluder[occlusion->occluder_count].vector[1] = render->draw_triangles[i].p[(e + 1) % 3].v;
				occlusion->occluder[occlusion->occluder_count].vector[1].z = render->draw_triangles[i].p[(e + 1) % 3].w;

				if (distpos == true)
				{
					occlusion->occluder[occlusion->occluder_count].normal[0].x = -occlusion->occluder[occlusion->occluder_count].normal[0].x;
					occlusion->occluder[occlusion->occluder_count].normal[0].y = -occlusion->occluder[occlusion->occluder_count].normal[0].y;
				}
				drawline(sdl, (t_point){occlusion->occluder[occlusion->occluder_count].vector[0].x, occlusion->occluder[occlusion->occluder_count].vector[0].y}, (t_point){occlusion->occluder[occlusion->occluder_count].vector[1].x, occlusion->occluder[occlusion->occluder_count].vector[1].y}, CLR_RED);
				print_vector3(occlusion->occluder[occlusion->occluder_count].vector[0]);
				print_vector3(occlusion->occluder[occlusion->occluder_count].vector[1]);
				occlusion->occluder_count++;
			}
		}
		i++;
	}
	printf("update occlusion count%d\n", occlusion->occluder_count);
	t_vector3	va,vb,vc;
	t_vector3	n;
	i = 0;
	while (i < occlusion->occluder_count)
	{
		j = 0;
		while (j < occlusion->occluder_count)
		{
			t_vector3 debug;
			if (j == i)
			{
				j++;
				continue;
			}
			if (occlusion->occluder[i].vector[0].x == occlusion->occluder[j].vector[1].x && occlusion->occluder[i].vector[0].y == occlusion->occluder[j].vector[1].y && 1)
			{
				va = occlusion->occluder[i].vector[0];
				vb = occlusion->occluder[i].vector[1];
				vc = occlusion->occluder[j].vector[0];

				delta = vector3_dist(va, vc) / vector3_dist(va, vb);
				n.x = ft_flerp(va.x, vb.x, delta);
				n.y = ft_flerp(va.y, vb.y, delta);

				n = vector3_sub(n, vc);
				n.z = 0.0f;
				test = vector2_magnitude((t_vector2){n.x, n.y});
				n = (t_vector3){.x = n.x/test, .y = n.y/test};

				occlusion->occluder[i].normal[1] = n;

				debug = vector3_add(va, vector3_mul(n, 25.0f));
				drawline(sdl, (t_point){va.x, va.y}, (t_point){debug.x, debug.y}, CLR_RED);
			}
			if (occlusion->occluder[i].vector[0].x == occlusion->occluder[j].vector[0].x && occlusion->occluder[i].vector[0].y == occlusion->occluder[j].vector[0].y && 1)
			{
				va = occlusion->occluder[i].vector[0];
				vb = occlusion->occluder[i].vector[1];
				vc = occlusion->occluder[j].vector[1];

				delta = vector3_dist(va, vc) / vector3_dist(va, vb);
				n.x = ft_flerp(va.x, vb.x, delta);
				n.y = ft_flerp(va.y, vb.y, delta);

				n = vector3_sub(n, vc);
				n.z = 0.0f;
				test = vector2_magnitude((t_vector2){n.x, n.y});
				n = (t_vector3){.x = n.x/test, .y = n.y/test};
				occlusion->occluder[i].normal[1] = n;
				debug = vector3_add(va, vector3_mul(n, 25.0f));
				drawline(sdl, (t_point){va.x, va.y}, (t_point){debug.x, debug.y}, CLR_GREEN);
			}
			if (occlusion->occluder[i].vector[1].x == occlusion->occluder[j].vector[0].x && occlusion->occluder[i].vector[1].y == occlusion->occluder[j].vector[0].y && 1)
			{
				va = occlusion->occluder[i].vector[1];
				vb = occlusion->occluder[i].vector[0];
				vc = occlusion->occluder[j].vector[1];

				delta = vector3_dist(va, vc) / vector3_dist(va, vb);
				n.x = ft_flerp(va.x, vb.x, delta);
				n.y = ft_flerp(va.y, vb.y, delta);

				n = vector3_sub(n, vc);
				n.z = 0.0f;
				test = vector2_magnitude((t_vector2){n.x, n.y});
				n = (t_vector3){.x = n.x/test, .y = n.y/test};

				occlusion->occluder[i].normal[2] = n;
				debug = vector3_add(va, vector3_mul(n, 25.0f));
				drawline(sdl, (t_point){va.x, va.y}, (t_point){debug.x, debug.y}, CLR_PRPL);
			}
			if (occlusion->occluder[i].vector[1].x == occlusion->occluder[j].vector[1].x && occlusion->occluder[i].vector[1].y == occlusion->occluder[j].vector[1].y && 1)
			{
				va = occlusion->occluder[i].vector[1];
				vb = occlusion->occluder[i].vector[0];
				vc = occlusion->occluder[j].vector[0];

				//print_vector3(va);
				//print_vector3(vb);
				//print_vector3(vc);
				delta = vector3_dist(va, vc) / vector3_dist(va, vb);
				//printf("delta %f dist 1 %f dist 2 %f\n", delta,vector3_dist(va, vc), vector3_dist(va, vb));
				n.x = ft_flerp(va.x, vb.x, delta);
				n.y = ft_flerp(va.y, vb.y, delta);

				n = vector3_sub(n, vc);
				n.z = 0.0f;
				test = vector2_magnitude((t_vector2){n.x, n.y});
				n = (t_vector3){.x = n.x/test, .y = n.y/test};
				occlusion->occluder[i].normal[2] = n;
				debug = vector3_add(va, vector3_mul(n, 25.0f));
				drawline(sdl, (t_point){va.x, va.y}, (t_point){debug.x, debug.y}, CLR_TURQ);
			}
			j++;
		}
		i++;
	}
	render->draw_tri_count = 0;
	render->calc_tri_count = 0;
}

static int	is_culled(t_entity *cull, t_entity *occlude, uint32_t input_count, t_triangle *input, t_triangle *output)
{
	int 		i;
	int			j;
	int			output_count;
	t_occlusion	*occlusion;
	uint32_t	uclipamount, vclipamount, wclipamount;
	t_triangle	uclipped[2], vclipped[2], wclipped[2];

	occlusion = &occlude->occlusion;
	if (cull->id == occlude->id || occlusion->occluder_count == 0)
	{
		memcpy(output, input, sizeof(t_triangle) * input_count);
		return(input_count);
	}
	output_count = 0;
	i = 0;
	while (i < occlusion->occluder_count)
	{
		j = 0;
		while (j < input_count)
		{
			uclipamount = clip_triangle_against_plane(occlusion->occluder[i].vector[0], occlusion->occluder[i].normal[0], input[j], uclipped);
			for (int u = 0; u < uclipamount; u++)
			{
				vclipamount = clip_triangle_against_plane(occlusion->occluder[i].vector[0], occlusion->occluder[i].normal[1], uclipped[u], vclipped);
				for (int v = 0; v < vclipamount; v++)
				{
					wclipamount = clip_triangle_against_plane(occlusion->occluder[i].vector[1], occlusion->occluder[i].normal[2], vclipped[v], wclipped);
					for (int w = 0; w < wclipamount; w++)
					{
						output[output_count++] = wclipped[w];
					}
				}
			}
			j++;
		}
		i++;
	}
	return(output_count);
}

static void swap(t_triangle *a, t_triangle *b)
{
	t_triangle temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

bool is_entity_occlusion_culled(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	t_occlusion *occlusion;
	t_box		b;
	int			i;
	int			j;
	int			clipamount;
	t_triangle	t;
	t_triangle	buff[2];
	float		test;
	float		delta;

	occlusion = &entity->occlusion;
	if (occlusion->occluder_count == 0)
		return(false);

	occlusion->occluder_count = 0;
	render->draw_tri_count = 0;
	render->calc_tri_count = 0;
	b = get_entity_box_transformed(entity);

	i = 0;
	while (i < 4)
	{
		t.p[0] = vector3_to_quaternion(b.v[i]);
		t.p[1] = vector3_to_quaternion(b.v[i + 4]);
		t.p[2] = vector3_to_quaternion(b.v[((i + 1) % 4) + 4]);
		triangle_to_projection(sdl, render, t, buff);
		t.p[1] = vector3_to_quaternion(b.v[((i + 1) % 4)]);
		t.p[2] = vector3_to_quaternion(b.v[(i + 1) % 4]);
		triangle_to_projection(sdl, render, t, buff);
		i++;
	}
	clipped(render, sdl);
	t_list		*l;
	t_entity	*ent;
	t_wall		wall;
	t_triangle	*ta;
	t_triangle	*tb;
	int			count;

	l = render->world->roomlist;
	ta = render->draw_triangles;
	tb = render->calc_triangles;
	count = render->draw_tri_count;
	while (l != NULL)
	{
		t_room room = *(t_room *)l->content;
		i = 0;
		while (i < room.wallcount)
		{
			count = is_culled(entity, &room.walls[i].entity, count, ta, tb);
			if (count == 0)
				return(true);
			swap(ta, tb);
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
		count = is_culled(entity, ent, count, ta, tb);
		if (count == 0)
			return(true);
		swap(ta, tb);
		l = l->next;
	}
	return(false);
}
