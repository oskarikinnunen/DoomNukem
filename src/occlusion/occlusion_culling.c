#include "doomnukem.h"

static t_box get_entity_box_transformed(t_entity *e)
{
	t_vector3	temp;
	t_box		box;
	int			i;
	int			max;

	max = 8;
	if (e->obj->bounds.type == bt_plane)
		max = 4;
	i = 0;
	while (i < max)
	{
		temp = e->obj->bounds.box.v[i];
		temp = vector3_mul_vector3(e->transform.scale, temp);
		temp = vector3_add(e->transform.position, temp);
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
		t.p[index] = quaternion_mul_matrix(render->camera.matworld, t.p[index]);
		t.p[index] = quaternion_mul_matrix(render->camera.matview, t.p[index]);
		index++;
	}
	clipamount = clip_triangle_against_plane((t_vector3){.z = 0.1f}, (t_vector3){.z = 1.0f}, t, buff);
	i = 0;
	while (i < clipamount)
	{
		index = 0;
		while(index < 3)
		{
			buff[i].p[index] = quaternion_mul_matrix(render->camera.matproj, buff[i].p[index]);
			buff[i].t[index] = buff[i].t[index];

			buff[i].t[index].u = buff[i].t[index].u / buff[i].p[index].w;

			buff[i].t[index].v = buff[i].t[index].v / buff[i].p[index].w;

			buff[i].t[index].w = 1.0f / buff[i].p[index].w;

			buff[i].p[index].v = vector3_div(buff[i].p[index].v, buff[i].p[index].w);
			buff[i].p[index].v = vector3_negative(buff[i].p[index].v);

			t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
			buff[i].p[index].v = vector3_add(buff[i].p[index].v, voffsetview);

			buff[i].p[index].v.x *= 0.5f * (float)(sdl.window_w * sdl.resolution_scaling);
			buff[i].p[index].v.y *= 0.5f * (float)(sdl.window_h * sdl.resolution_scaling);

			index++;
		}
		render->occ_calc_tris[render->occ_calc_tri_count++] = buff[i];
		i++;
	}
}

void calculate_triangles(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	t_triangle	t;
	t_box		b;
	int			i;

	if (entity->obj->bounds.type == bt_plane)
	{
		b = get_entity_box_transformed(entity);
		t.p[0] = vector3_to_quaternion(b.v[0]);
		t.p[1] = vector3_to_quaternion(b.v[2]);
		t.p[2] = vector3_to_quaternion(b.v[3]);
		t.t[0] = vector2_to_texture((t_vector2){0, 0});
		t.t[1] = vector2_to_texture((t_vector2){0, 1});
		t.t[2] = vector2_to_texture((t_vector2){1, 1});
		t.clr = CLR_BLUE;
		triangle_to_projection(sdl, render, t);
		t.clr = CLR_PRPL;
		t.p[0] = vector3_to_quaternion(b.v[0]);
		t.p[1] = vector3_to_quaternion(b.v[1]);
		t.p[2] = vector3_to_quaternion(b.v[3]);
		triangle_to_projection(sdl, render, t);
	}
	else if (entity->obj->bounds.type == bt_box)
	{
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
			t.p[0] = vector3_to_quaternion(b.v[i]);
			t.p[1] = vector3_to_quaternion(b.v[(i + 1) % 4]);
			t.p[2] = vector3_to_quaternion(b.v[((i + 1) % 4) + 4]);
			triangle_to_projection(sdl, render, t);
			i++;
		}
	}
	else if (entity->obj->bounds.type == bt_ignore)
	{
		t_quaternion temp;
	
		i = 0;
		while (i < entity->obj->vertice_count)
		{
			temp.v = entity->obj->vertices[i];
			if (entity->animation.active)
			{
				temp.v = vector3_add(entity->obj->o_anim.frames[entity->animation.frame].deltavertices[i].delta, temp.v); // hmm the walls are dancing
			}
			render->q[i] = transformed_vector3(entity->transform, temp.v);
			i++;
		}
		i = 0;
		while (i < entity->obj->face_count)
		{
			t = (t_triangle){render->q[entity->obj->faces[i].v_indices[0] - 1], render->q[entity->obj->faces[i].v_indices[1] - 1], render->q[entity->obj->faces[i].v_indices[2] - 1]};
			t.t[0] = vector2_to_texture((t_vector2){0, 0});
			t.t[1] = vector2_to_texture((t_vector2){0, 1});
			t.t[2] = vector2_to_texture((t_vector2){1, 1});
			t.clr = CLR_BLUE;
			triangle_to_projection(sdl, render, t);
			i++;
		}
	}
	clipped(render, sdl);
}

static int	edge_clip_against_plane(t_vector3 plane_p, t_vector3 plane_n, t_edge *edge)
{
	float	delta;
	float	dist[2];

	dist[0] = vector3_fdist_to_plane(edge->v[0], plane_n, plane_p);
	dist[1] = vector3_fdist_to_plane(edge->v[1], plane_n, plane_p);
	if (dist[0] < 0.0f && dist[1] < 0.0f)
		return(0);
	if (dist[0] < 0.0f)
	{
		delta = line_intersect_plane(plane_p, plane_n, edge->v[0], edge->v[1]);
		edge->v[0] = vector3_lerp(edge->v[0], edge->v[1], delta);
	}
	else if (dist[1] < 0.0f)
	{
		delta = line_intersect_plane(plane_p, plane_n, edge->v[0], edge->v[1]);
		edge->v[1] = vector3_lerp(edge->v[0], edge->v[1], delta);
	}
	return(1);
}

static int	edge_to_screenspace(t_sdlcontext sdl, int *v, t_quaternion q[8], t_render *render, t_edge *edge)
{
	t_vector3		plane_n, plane_p;
	t_quaternion	temp;
	int				ret;

	plane_n = (t_vector3){.z = 1.0f};
	plane_p = (t_vector3){.z = 0.1f};
	edge->v[0] = q[v[0]].v;
	edge->v[1] = q[v[1]].v;
	ret = edge_clip_against_plane(plane_p, plane_n, edge);
	if (ret == 0)
		return(0);
	for (int i = 0; i < 2; i++)
	{
		temp = quaternion_mul_matrix(render->camera.matproj, (t_quaternion){edge->v[i], q[v[i]].w});
		edge->v[i] = vector3_div(temp.v, temp.w);
		edge->v[i].x = -edge->v[i].x;
		edge->v[i].y = -edge->v[i].y;

		t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
		edge->v[i] = vector3_add(edge->v[i], voffsetview);

		edge->v[i].x *= 0.5f * (float)(sdl.window_w * sdl.resolution_scaling);
		edge->v[i].y *= 0.5f * (float)(sdl.window_h * sdl.resolution_scaling);
	}
	return(1);
}

static int entity_plane_to_screenspace_edges(t_sdlcontext sdl, t_render *render, t_entity *entity, t_edge *edges, t_quaternion q[8])
{
	t_vector3		plane_n, plane_p;
	t_quaternion	temp;
	t_box			b;
	int				i;
	float			delta;
	float			dist[2];
	int				index[2];
	int				count = 0;

	b = get_entity_box_transformed(entity);
	i = 0;
	while (i < 4)
	{
		temp = vector3_to_quaternion(b.v[i]);
		temp = quaternion_mul_matrix(render->camera.matworld, temp);
		temp = quaternion_mul_matrix(render->camera.matview, temp);
		q[i] = temp;
		i++;
	}
	plane_n = (t_vector3){.z = 1.0f};
	plane_p = (t_vector3){.z = 0.1f};
	count += edge_to_screenspace(sdl, (int [2]){0, 1}, q, render, &edges[count]);
	count += edge_to_screenspace(sdl, (int [2]){1, 3}, q, render, &edges[count]);
	count += edge_to_screenspace(sdl, (int [2]){2, 3}, q, render, &edges[count]);
	count += edge_to_screenspace(sdl, (int [2]){0, 2}, q, render, &edges[count]);
	return(count);
}

static int entity_box_to_screenspace_edges(t_sdlcontext sdl, t_render *render, t_entity *entity, t_edge *edges, t_quaternion q[8])
{
	t_vector3		plane_n, plane_p;
	t_quaternion	temp;
	t_box			b;
	int				i;
	float			delta;
	float			dist[2];
	int				index[2];
	int				count = 0;

	b = get_entity_box_transformed(entity);
	i = 0;
	while (i < 8)
	{
		temp = vector3_to_quaternion(b.v[i]);
		temp = quaternion_mul_matrix(render->camera.matworld, temp);
		temp = quaternion_mul_matrix(render->camera.matview, temp);
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
		count += edge_to_screenspace(sdl, index, q, render, &edges[count]);
		index[1] = i + 4;
		count += edge_to_screenspace(sdl, index, q, render, &edges[count]);
		index[0] = ((i + 1) % 4) + 4;
		count += edge_to_screenspace(sdl, index, q, render, &edges[count]);
		i++;
	}
	return(count);
}

static t_vector3 normal_from_triangle(t_vector3 va, t_vector3 vb, t_vector3 vc)
{
	float		delta;
	t_vector3	n;

	delta = vector3_dist(va, vc) / vector3_dist(va, vb);
	n.x = ft_flerp(va.x, vb.x, delta);
	n.y = ft_flerp(va.y, vb.y, delta);

	n = vector3_sub(n, vc);
	n.z = 0.0f;
	vector3_normalise(n);
	return(n);
}

void update_occlusion_culling(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	t_occlusion *occlusion;
	int			i;
	int			j;
	int			clipamount;
	float		test;
	float		min;
	float		delta;
	t_quaternion	q[8];
	t_edge			edges[12];
	int				count;

	t_triangle	t;

	occlusion = &entity->occlusion;

	occlusion->occluder_count = 0;
	render->occ_tri_count = 0;
	render->occ_calc_tri_count = 0;
	if (entity->obj->bounds.type == bt_plane)
	{
		count = entity_plane_to_screenspace_edges(sdl, render, entity, edges, q);
	}
	else
		count = entity_box_to_screenspace_edges(sdl, render, entity, edges, q);
	i = 0;
	while (i < count)
	{
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

		int neg, pos;
		neg = 0;
		pos = 0;
		j = 0;
		while (j < count)
		{
			if (j == i)
			{
				j++;
				continue;
			}
			dist[0] = vector2_fdist_to_plane((t_vector2){edges[j].v[0].x, edges[j].v[0].y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y});
			dist[1] = vector2_fdist_to_plane((t_vector2){edges[j].v[1].x, edges[j].v[1].y}, (t_vector2){plane_n.x, plane_n.y}, (t_vector2){plane_p.x, plane_p.y});
			if (dist[0] >= -1.0f && dist[1] >= -1.0f && dist[0] + dist[1] > 0.0f)
				pos++;
			if (dist[0] <= 1.0f && dist[1] <= 1.0f && dist[0] + dist[1] < 0.0f)
				neg++;
			j++;
		}
		if (pos == 0 || neg == 0)
		{
			occlusion->occluder[occlusion->occluder_count].vector[0] = plane_p;
			occlusion->occluder[occlusion->occluder_count].vector[1] = edges[i].v[1];
			if (pos > 0)
			{
				plane_n.x = -plane_n.x;
				plane_n.y = -plane_n.y;
			}
			occlusion->occluder[occlusion->occluder_count].normal[0] = plane_n;
			occlusion->occluder[occlusion->occluder_count].normal[1] = plane_n;
			occlusion->occluder[occlusion->occluder_count].normal[2] = plane_n;
			occlusion->occluder_count++;
		}
		i++;
	}
	t_vector3	va,vb,vc;
	t_vector3	n;
	i = 0;
	while (i < occlusion->occluder_count)
	{
		j = 0;
		while (j < occlusion->occluder_count)
		{
			if (j == i)
			{
				j++;
				continue;
			}
			if (occlusion->occluder[i].vector[0].x == occlusion->occluder[j].vector[1].x && occlusion->occluder[i].vector[0].y == occlusion->occluder[j].vector[1].y)
				occlusion->occluder[i].normal[1] = normal_from_triangle(occlusion->occluder[i].vector[0], occlusion->occluder[i].vector[1], occlusion->occluder[j].vector[0]);
			if (occlusion->occluder[i].vector[0].x == occlusion->occluder[j].vector[0].x && occlusion->occluder[i].vector[0].y == occlusion->occluder[j].vector[0].y)
				occlusion->occluder[i].normal[1] = normal_from_triangle(occlusion->occluder[i].vector[0], occlusion->occluder[i].vector[1], occlusion->occluder[j].vector[1]);
			if (occlusion->occluder[i].vector[1].x == occlusion->occluder[j].vector[0].x && occlusion->occluder[i].vector[1].y == occlusion->occluder[j].vector[0].y)
				occlusion->occluder[i].normal[2] = normal_from_triangle(occlusion->occluder[i].vector[1], occlusion->occluder[i].vector[0], occlusion->occluder[j].vector[1]);
			if (occlusion->occluder[i].vector[1].x == occlusion->occluder[j].vector[1].x && occlusion->occluder[i].vector[1].y == occlusion->occluder[j].vector[1].y)
				occlusion->occluder[i].normal[2] = normal_from_triangle(occlusion->occluder[i].vector[1], occlusion->occluder[i].vector[0], occlusion->occluder[j].vector[0]);
			j++;
		}
		i++;
	}
	if (occlusion->occluder_count > 0)
	{
		render->rs.occluder_count++;
		occlusion->is_occluded = false;
	}
	else
	{
		occlusion->is_occluded = true;
	}
}


static void swap(t_triangle **a, t_triangle **b)
{
	t_triangle *temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static bool is_tri_nan(t_triangle t)
{
	for (int j = 0; j < 3; j++)
	{
		if (isnan(t.p[j].v.x) == true || isnan(t.p[j].v.y) == true || isnan(t.p[j].v.z) == true)
			return(true);
	}
	return(false);
}

static int	is_culled(t_entity *occlude, uint32_t input_count, t_triangle **input, t_triangle **output)
{
	int 		i;
	int			j;
	int			output_count;
	t_occlusion	*occlusion;
	uint32_t	uclipamount, vclipamount, wclipamount;
	t_triangle	uclipped[2], vclipped[2], wclipped[2];

	occlusion = &occlude->occlusion;
	output_count = 0;
	i = 0;
	while (i < occlusion->occluder_count)
	{
		j = 0;
		while (j < input_count)
		{
			uclipamount = clip_triangle_against_occluder_plane(occlusion->occluder[i].vector[0], occlusion->occluder[i].normal[0], (*input)[j], uclipped);
			for (int u = 0; u < uclipamount; u++)
			{
				vclipamount = clip_triangle_against_occluder_plane(occlusion->occluder[i].vector[0], occlusion->occluder[i].normal[1], uclipped[u], vclipped);
				for (int v = 0; v < vclipamount; v++)
				{
					wclipamount = clip_triangle_against_occluder_plane(occlusion->occluder[i].vector[1], occlusion->occluder[i].normal[2], vclipped[v], wclipped);
					for (int w = 0; w < wclipamount; w++)
					{
						(*output)[output_count++] = wclipped[w];
					}
				}
			}
			j++;
		}
		i++;
	}
	return(output_count);
}

static void get_min_max_from_edges(t_vector2 *min, t_vector2 *max, t_occluder *occl, int count)
{
	t_vector3	v;
	int			i;

	max->x = -1000000000;
	max->y = -1000000000;
	min->x = 1000000000;
	min->y = 1000000000;
	i = 0;
	while (i < count)
	{
		for (int j = 0; j < 2; j++)
		{
			v = occl[i].vector[j];
			if (v.x < min->x)
				min->x = v.x;
			if (v.x > max->x)
				max->x = v.x;
			if (v.y < min->y)
				min->y = v.y;
			if (v.y > max->y)
				max->y = v.y;
		}
		i++;
	}
}

static bool square_overlap(t_square a, t_square b)
{
	return (
    a.min.x <= b.max.x &&
    a.max.x >= b.min.x &&
    a.min.y <= b.max.y &&
    a.max.y >= b.min.y
  );
}

bool is_valid_occlude_check(int32_t id, t_entity *occlude, t_render *render, t_square cull_square, float cull_dist)
{
	t_square	occl_square;
	float		occl_dist;

	if (occlude->id != id && occlude->occlusion.occlude == true && occlude->occlusion.occluder_count > 0)
	{
		get_min_max_from_edges(&occl_square.min, &occl_square.max, occlude->occlusion.occluder, occlude->occlusion.occluder_count);
		if (square_overlap(cull_square, occl_square) == true)
		{
			occl_dist = vector3_dist(vector3_add(occlude->obj->bounds.origin, occlude->transform.position), render->camera.position);
			if (occl_dist < cull_dist)
				return(true);
		}
	}
	return(false);
}

bool is_entity_occlusion_culled(struct s_world *world, t_render *render, t_entity *cull)
{
	t_list		*l;
	t_triangle	*ta;
	t_triangle	*tb;
	t_square	cull_square;
	int			count;
	float		cull_dist;

	render->occ_tri_count = 0;
	render->occ_calc_tri_count = 0;
	if (cull->occlusion.is_occluded == true)
		return(true);
	calculate_triangles(*world->sdl, render, cull);
	ta = render->occ_draw_tris;
	tb = render->occ_calc_tris;
	cull_dist = vector3_dist(vector3_add(cull->obj->bounds.origin, cull->transform.position), render->camera.position);
	get_min_max_from_triangles(&cull_square.min, &cull_square.max, render->occ_draw_tris, render->occ_tri_count);
	count = calculate_tris_from_square(cull_square, cull, render);

	int			i;
	int			found;
	t_entity	*ent;

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = &world->entitycache.entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
			{
				if (is_valid_occlude_check(cull->id, ent, render, cull_square, cull_dist))
				{
					count = is_culled(ent, count, &ta, &tb);
					if (count > 100)
					{
						return(false);
					}
					if (count == 0)
						return(true);
					swap(&ta, &tb);
				}
			}
			found++;
		}
		i++;
	}
	render->occ_tri_count = 0;
	render->occ_calc_tri_count = 0;
	return(false);
}
