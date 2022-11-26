#include "doomnukem.h"

static t_box get_entity_box_transformed(t_entity *e)
{
	t_vector3	temp;
	t_box		box;
	int			i;
	i = 0;
	while (i < 8)
	{
		temp = e->obj->bounds.box.v[i];
		temp = vector3_mul_vector3(e->transform.scale, temp);
		temp = vector3_add(e->transform.location, temp);
		box.v[i] = temp;
		i++;
	}
	return(box);
}

static void triangle_to_projection(t_sdlcontext sdl, t_render *render, t_triangle t)
{
	t_triangle	buff[2];
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
			buff[i].t[index] = buff[i].t[index];

			buff[i].t[index].u = buff[i].t[index].u / buff[i].p[index].w;

			buff[i].t[index].v = buff[i].t[index].v / buff[i].p[index].w;

			buff[i].t[index].w = 1.0f / buff[i].p[index].w;

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

static t_texture	vector2_to_texture(t_vector2 v)
{
	t_texture t;
	t.u = v.x;
	t.v = v.y;
	t.w = 1.0f;
	return(t);
}

static void calculate_triangles(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	t_triangle	t;
	t_box		b;
	int			i;

	b = get_entity_box_transformed(entity);
	i = 0;
	while (i < 4)
	{
		t.p[0] = vector3_to_quaternion(b.v[i]);
		t.p[1] = vector3_to_quaternion(b.v[i + 4]);
		t.p[2] = vector3_to_quaternion(b.v[((i + 1) % 4) + 4]);
		t.t[0] = vector2_to_texture((t_vector2){0, 0});
		t.t[1] = vector2_to_texture((t_vector2){0, 1});
		t.t[2] = vector2_to_texture((t_vector2){1, 1});
		t.clr = CLR_BLUE;
		triangle_to_projection(sdl, render, t);
		t.clr = CLR_PRPL;
		t.p[1] = vector3_to_quaternion(b.v[(i + 1) % 4]);
		//t.p[2] = vector3_to_quaternion(b.v[((i + 1) % 4) + 4]);
		triangle_to_projection(sdl, render, t);
		i++;
	}
	clipped(render, sdl);
	/*
	i = 0;
	while (i < render->draw_tri_count)
	{
		z_fill_tri_solid(sdl, render->draw_triangles[i++]);
	}*/
}

typedef struct s_edge
{
	t_vector3	v[2];
}	t_edge;

static t_edge	edge_clip_against_plane(t_vector3 plane_p, t_vector3 plane_n, t_edge	edge)
{
	float	delta;
	float	dist[2];

	dist[0] = vector3_fdist_to_plane(edge.v[0], plane_n, plane_p);
	dist[1] = vector3_fdist_to_plane(edge.v[1], plane_n, plane_p);
	if (dist[0] < 0.0f)
	{
		delta = line_intersect_plane(plane_p, plane_n, edge.v[0], edge.v[1]);
		edge.v[0] = vector3_lerp(edge.v[0], edge.v[1], delta);
	}
	else if (dist[1] < 0.0f)
	{
		delta = line_intersect_plane(plane_p, plane_n, edge.v[1], edge.v[0]);
		edge.v[1] = vector3_lerp(edge.v[1], edge.v[0], delta);
	}
	return(edge);
}

static void	box_edge_to_screenspace(int *v, t_quaternion q[8], t_render *render, t_edge *edge)
{
	t_vector3		plane_n, plane_p;
	t_quaternion	temp;

	plane_n = (t_vector3){.z = 1.0f};
	plane_p = (t_vector3){.z = 0.1f};
	*edge = edge_clip_against_plane(plane_p, plane_n, (t_edge){q[v[0]].v, q[v[1]].v});
	for (int i = 0; i < 2; i++)
	{
		temp = quaternion_mul_matrix(render->matproj, (t_quaternion){edge->v[i], q[v[i]].w});
		edge->v[i] = vector3_div(temp.v, temp.w);
		edge->v[i].x = -edge->v[i].x;
		edge->v[i].y = -edge->v[i].y;

		t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
		edge->v[i] = vector3_add(edge->v[i], voffsetview);

		edge->v[i].x *= 0.5f * render->sdl->window_w;
		edge->v[i].y *= 0.5f * render->sdl->window_h;
	}
	//return(edge);
}

static void entity_box_to_screenspace_edges(t_render *render, t_entity *entity, t_edge *edges, t_quaternion q[8])
{
	t_vector3		plane_n, plane_p;
	t_quaternion	temp;
	t_box			b;
	int				i;
	float			delta;
	float			dist[2];
	int				index[2];

	b = get_entity_box_transformed(entity);
	i = 0;
	while (i < 8)
	{
		temp = vector3_to_quaternion(b.v[i]);
		temp = quaternion_mul_matrix(render->matworld, temp);
		temp = quaternion_mul_matrix(render->matview, temp);
		q[i] = temp;
		i++;
	}
	plane_n = (t_vector3){.z = 1.0f};
	plane_p = (t_vector3){.z = 0.1f};
	i = 0;
	while (i < 4)
	{
		index[0] = i;
		index[1] = (i + 1) % 4;
		box_edge_to_screenspace(index, q, render, &edges[i]);
		index[1] = i + 4;
		box_edge_to_screenspace(index, q, render, &edges[i + 4]);
		index[0] = ((i + 1) % 4) + 4; //(int [2]){i + 4, ((i + 1) % 4) + 4}
		box_edge_to_screenspace(index, q, render, &edges[i + 8]);
		i++;
	}
	i = 0;
	while (i < 12 && 0)
	{
		drawline(*render->sdl, (t_point){edges[i].v[0].x, edges[i].v[0].y}, (t_point){edges[i].v[1].x, edges[i].v[1].y}, CLR_RED);
		i++;
	}
}

void update_occlusion_culling(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	t_occlusion *occlusion;
	t_box		b;
	int			i;
	int			j;
	int			clipamount;
	float		test;
	float		delta;
	t_quaternion	q[8];
	t_edge			edges[12];

	occlusion = &entity->occlusion;

	occlusion->occluder_count = 0;
	render->draw_tri_count = 0;
	render->calc_tri_count = 0;
	entity_box_to_screenspace_edges(render, entity, edges, q);
	i = 0;
	while (i < 12 && 0)
	{
		drawline(*render->sdl, (t_point){edges[i].v[0].x, edges[i].v[0].y}, (t_point){edges[i].v[1].x, edges[i].v[1].y}, CLR_RED);
		i++;
	}
	i = 0;
	while (i < 12)
	{
		bool distneg = false;
		bool distpos = false;
		float	dist[2];
		t_vector3 plane_p;
		t_vector3 plane_n;

		plane_p = edges[i].v[0];
		plane_n = vector3_sub(edges[i].v[1], plane_p);
		plane_n.z = plane_n.x;
		plane_n.x = -plane_n.y;
		plane_n.y = plane_n.z;
		plane_n.z = 0.0f;
		test = vector2_magnitude((t_vector2){plane_n.x, plane_n.y});
		plane_n = (t_vector3){.x = plane_n.x/test, .y = plane_n.y/test, .z = 0.0f};

		j = 0;
		while (j < 12)
		{
			if (j == i)
			{
				j++;
				continue;
			}
			dist[0] = vector2_fdist_to_plane((t_vector2){edges[j].v[0].x, edges[j].v[0].y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y});
			dist[1] = vector2_fdist_to_plane((t_vector2){edges[j].v[1].x, edges[j].v[1].y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y});
			printf("dist0 %f dist1 %f\n", dist[0], dist[1]);
			if (dist[0] > 1.0f && dist[1] < -1.0f)
			{
				j = -1;
				break;
			}
			if (dist[0] < -1.0f && dist[1] > 1.0f)
			{
				j = -1;
				break;
			}
			j++;
		}
		//printf("e is %d i is %d\n", e, i);
		if (j != -1)
		{
			printf("add occluder\n");
			occlusion->occluder[occlusion->occluder_count].normal[0] = plane_n;

			occlusion->occluder[occlusion->occluder_count].vector[0] = plane_p;
			//occlusion->occluder[occlusion->occluder_count].vector[0].z = render->draw_triangles[i].p[e].w;
			occlusion->occluder[occlusion->occluder_count].vector[1] = edges[i].v[1];
			//occlusion->occluder[occlusion->occluder_count].vector[1].z = render->draw_triangles[i].p[(e + 1) % 3].w;

			print_vector3(occlusion->occluder[occlusion->occluder_count].vector[0]);
			print_vector3(occlusion->occluder[occlusion->occluder_count].vector[1]);
			//print_vector3(occlusion->occluder[occlusion->occluder_count].normal[0]);
			//print_vector3(occlusion->occluder[occlusion->occluder_count].normal[1]);
			//print_vector3(occlusion->occluder[occlusion->occluder_count].normal[2]);
			if (dist[0] > 0.0f || dist[1] > 0.0f)
			{
				occlusion->occluder[occlusion->occluder_count].normal[0].x = -occlusion->occluder[occlusion->occluder_count].normal[0].x;
				occlusion->occluder[occlusion->occluder_count].normal[0].y = -occlusion->occluder[occlusion->occluder_count].normal[0].y;
			}
			//drawline(sdl, (t_point){occlusion->occluder[occlusion->occluder_count].vector[0].x, occlusion->occluder[occlusion->occluder_count].vector[0].y}, (t_point){occlusion->occluder[occlusion->occluder_count].vector[1].x, occlusion->occluder[occlusion->occluder_count].vector[1].y}, CLR_RED);
			//print_vector3(occlusion->occluder[occlusion->occluder_count].vector[0]);
			//print_vector3(occlusion->occluder[occlusion->occluder_count].vector[1]);
			occlusion->occluder_count++;
		}
		else
			printf("discard\n\n");
		i++;
	}
	/*LEGACY
	calculate_triangles(sdl, render, entity);
	i = 0;
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
				if( j == i)
				{
					j++;
					continue;
				}
				dist[0] = roundf(vector2_fdist_to_plane((t_vector2){render->draw_triangles[j].p[0].v.x, render->draw_triangles[j].p[0].v.y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y}));
				dist[1] = roundf(vector2_fdist_to_plane((t_vector2){render->draw_triangles[j].p[1].v.x, render->draw_triangles[j].p[1].v.y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y}));
				dist[2] = roundf(vector2_fdist_to_plane((t_vector2){render->draw_triangles[j].p[2].v.x, render->draw_triangles[j].p[2].v.y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y}));
				//dist[0] = roundf(vector3_fdist_to_plane(render->draw_triangles[j].p[0].v, plane_n, plane_p));
				//dist[1] = roundf(vector3_fdist_to_plane(render->draw_triangles[j].p[1].v, plane_n, plane_p));
				//dist[2] = roundf(vector3_fdist_to_plane(render->draw_triangles[j].p[2].v, plane_n, plane_p));
				//printf("dist0 %f dist1 %f dist2 %f\n", dist[0], dist[1], dist[2]);
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
			//printf("e is %d i is %d\n", e, i);
			if (distneg != distpos)
			{
				//printf("add occluder\n");
				occlusion->occluder[occlusion->occluder_count].normal[0] = plane_n;

				occlusion->occluder[occlusion->occluder_count].vector[0] = plane_p;
				occlusion->occluder[occlusion->occluder_count].vector[0].z = render->draw_triangles[i].p[e].w;
				occlusion->occluder[occlusion->occluder_count].vector[1] = render->draw_triangles[i].p[(e + 1) % 3].v;
				occlusion->occluder[occlusion->occluder_count].vector[1].z = render->draw_triangles[i].p[(e + 1) % 3].w;

				print_vector3(occlusion->occluder[occlusion->occluder_count].vector[0]);
				print_vector3(occlusion->occluder[occlusion->occluder_count].vector[1]);
				if (distpos == true)
				{
					occlusion->occluder[occlusion->occluder_count].normal[0].x = -occlusion->occluder[occlusion->occluder_count].normal[0].x;
					occlusion->occluder[occlusion->occluder_count].normal[0].y = -occlusion->occluder[occlusion->occluder_count].normal[0].y;
				}
				//drawline(sdl, (t_point){occlusion->occluder[occlusion->occluder_count].vector[0].x, occlusion->occluder[occlusion->occluder_count].vector[0].y}, (t_point){occlusion->occluder[occlusion->occluder_count].vector[1].x, occlusion->occluder[occlusion->occluder_count].vector[1].y}, CLR_RED);
				//print_vector3(occlusion->occluder[occlusion->occluder_count].vector[0]);
				//print_vector3(occlusion->occluder[occlusion->occluder_count].vector[1]);
				occlusion->occluder_count++;
			}
		}
		i++;
	}*/
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
				//drawline(sdl, (t_point){va.x, va.y}, (t_point){debug.x, debug.y}, CLR_GREEN);
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
				//drawline(sdl, (t_point){va.x, va.y}, (t_point){debug.x, debug.y}, CLR_GREEN);
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
				//drawline(sdl, (t_point){va.x, va.y}, (t_point){debug.x, debug.y}, CLR_PRPL);
			}
			if (occlusion->occluder[i].vector[1].x == occlusion->occluder[j].vector[1].x && occlusion->occluder[i].vector[1].y == occlusion->occluder[j].vector[1].y && 1)
			{
				va = occlusion->occluder[i].vector[1];
				vb = occlusion->occluder[i].vector[0];
				vc = occlusion->occluder[j].vector[0];

				//print_vector3(va);
				delta = vector3_dist(va, vc) / vector3_dist(va, vb);
				n.x = ft_flerp(va.x, vb.x, delta);
				n.y = ft_flerp(va.y, vb.y, delta);

				n = vector3_sub(n, vc);
				n.z = 0.0f;
				test = vector2_magnitude((t_vector2){n.x, n.y});
				n = (t_vector3){.x = n.x/test, .y = n.y/test};
				occlusion->occluder[i].normal[2] = n;
				debug = vector3_add(va, vector3_mul(n, 25.0f));
			//	printf("i %d j %d\n", i, j);
			//	drawline(sdl, (t_point){va.x, va.y}, (t_point){debug.x, debug.y}, CLR_TURQ);
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < occlusion->occluder_count && 1)
	{
		t_vector3 debug, v, n;
		//print_vector3(occlusion->occluder[i].vector[0]);
		//print_vector3(occlusion->occluder[i].vector[1]);
		//printf("i is %d\n", i);
		v = occlusion->occluder[i].vector[0];
		n = occlusion->occluder[i].normal[1];
		debug = vector3_add(v, vector3_mul(n, 25.0f));
		drawline(sdl, (t_point){v.x, v.y}, (t_point){debug.x, debug.y}, CLR_TURQ);
		v = occlusion->occluder[i].vector[1];
		n = occlusion->occluder[i].normal[2];
		debug = vector3_add(v, vector3_mul(n, 25.0f));
		drawline(sdl, (t_point){v.x, v.y}, (t_point){debug.x, debug.y}, CLR_GREEN);
		v = occlusion->occluder[i].vector[0];
		n = occlusion->occluder[i].vector[1];
		drawline(sdl, (t_point){v.x, v.y}, (t_point){n.x, n.y}, CLR_RED);
		i++;
	}
}

static int	is_culled(t_entity *cull, t_entity *occlude, uint32_t input_count, t_triangle *input, t_triangle *output, t_sdlcontext sdl)
{
	int 		i;
	int			j;
	int			output_count;
	t_occlusion	*occlusion;
	uint32_t	uclipamount, vclipamount, wclipamount;
	t_triangle	uclipped[2], vclipped[2], wclipped[2];

	occlusion = &occlude->occlusion;
	i = 0;
	while (i < occlusion->occluder_count && 0)
	{
		t_vector3 debug, v, n;
		v = occlusion->occluder[i].vector[0];
		n = occlusion->occluder[i].normal[1];
		debug = vector3_add(v, vector3_mul(n, 25.0f));
		drawline(sdl, (t_point){v.x, v.y}, (t_point){debug.x, debug.y}, CLR_TURQ);
		v = occlusion->occluder[i].vector[1];
		n = occlusion->occluder[i].normal[2];
		debug = vector3_add(v, vector3_mul(n, 25.0f));
		drawline(sdl, (t_point){v.x, v.y}, (t_point){debug.x, debug.y}, CLR_GREEN);
		v = occlusion->occluder[i].vector[0];
		n = occlusion->occluder[i].vector[1];
		drawline(sdl, (t_point){v.x, v.y}, (t_point){n.x, n.y}, CLR_RED);
		i++;
	}
	printf("occluder count %d cull id %d occlude id %d\n", occlusion->occluder_count, cull->id, occlude->id);
	if (cull->id == occlude->id || occlusion->occluder_count == 0)
	{
		memcpy(output, input, sizeof(t_triangle) * input_count);//can swap prob
		printf("early return\n");
		return(input_count);
	}
	output_count = 0;
	i = 0;
	while (i < occlusion->occluder_count)
	{
		j = 0;
		while (j < input_count)
		{
			occlusion->occluder[i].vector[0].z = 0.0f;
			occlusion->occluder[i].vector[1].z = 0.0f;
			occlusion->occluder[i].normal[0].z = 0.0f;
			occlusion->occluder[i].normal[1].z = 0.0f;
			occlusion->occluder[i].normal[2].z = 0.0f;
			input[j].p[0].v.z = 0.0f;
			input[j].p[1].v.z = 0.0f;
			input[j].p[2].v.z = 0.0f;
			uclipamount = clip_triangle_against_plane(occlusion->occluder[i].vector[0], occlusion->occluder[i].normal[0], input[j], uclipped);
			//printf("uclipamount %d\n", uclipamount);
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
		t_vector3 debug, v, n;
		v = occlusion->occluder[i].vector[0];
		n = occlusion->occluder[i].normal[0];
		v = vector3_lerp(v, occlusion->occluder[i].vector[1], 0.5f);
		debug = vector3_add(v, vector3_mul(n, 50.0f));
		//drawline(sdl, (t_point){v.x, v.y}, (t_point){debug.x, debug.y}, CLR_TURQ);
		i++;
	}
	//printf("is culled, output amount %d\n", output_count);
	return(output_count);
}

static void swap(t_triangle *a, t_triangle *b)
{
	t_triangle temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

bool is_entity_occlusion_culled(t_sdlcontext sdl, t_render *render, t_entity *cull)
{
	printf("start\n\n\n\n");
	t_occlusion *occlusion;
	t_box		b;
	int			i;
	int			j;
	int			clipamount;
	t_triangle	t;
	t_triangle	buff[2];
	float		test;
	float		delta;
	t_list		*l;
	t_entity	*ent;
	t_wall		wall;
	t_triangle	*ta;
	t_triangle	*tb;
	int			count;


	render->draw_tri_count = 0;
	render->calc_tri_count = 0;
	calculate_triangles(sdl, render, cull);
	l = render->world->roomlist;
	ta = render->draw_triangles;
	tb = render->calc_triangles;
	count = render->draw_tri_count;
	printf("draw count %d on is entity culled\n", count);
	while (l != NULL)
	{
		t_room room = *(t_room *)l->content;
		i = 0;
		while (i < room.wallcount)
		{
			//printf("1id %d\n", room.walls[i].entity.id);
			count = is_culled(cull, &room.walls[i].entity, count, ta, tb, sdl);
			//printf("1count is %d\n", count);
			//if (count == 0)
		//		return(true);
			//swap(ta, tb);
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
		printf("id %d %d\n", ent->id, ent->occlusion.occluder_count);
		count = is_culled(cull, ent, count, ta, tb, sdl);
		printf("count is %d\n", count);
		if (count == 0)
			return(true);
		swap(ta, tb);
		l = l->next;
	}
	i = 0;
	while (i < count && 0)
	{
		z_fill_tri(sdl, ta[i++], *render->debug_img);
	}
	printf("end %d\n\n\n\n", count);
	return(false);
}
