#include "doomnukem.h"

static t_box get_entity_box_transformed(t_entity *e)
{
	t_quaternion	temp;
	t_box			box;
	int				i;
	int				max;
	t_mat4x4		mattrans;

	mattrans = make_transform_matrix(e->transform);
	max = 8;
	if (e->obj->bounds.type == bt_plane)
		max = 4;
	i = 0;
	while (i < max)
	{
		temp = vector3_to_quaternion(e->obj->bounds.box.v[i]);
		box.v[i] = quaternion_mul_matrix(mattrans, temp).v;
		i++;
	}
	return(box);
}

static void triangle_to_projection(t_sdlcontext sdl, t_render *render, t_triangle t)
{
	t_triangle			buff[2];
	t_point_triangle	res_buff;
	int	index;
	int	clipamount;
	int	i;

	index = 0;
	while(index < 3)
	{
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

			res_buff.p[index].x = buff[i].p[index].v.x * (0.5f * (float)(sdl.window_w * sdl.resolution_scaling));
			res_buff.p[index].y = buff[i].p[index].v.y * (0.5f * (float)(sdl.window_h * sdl.resolution_scaling));

			res_buff.t[index] = buff[i].t[index];
			res_buff.clr = buff[i].clr;
			index++;
		}
		render->worldspace_ptris[render->worldspace_ptri_count++] = res_buff;
		i++;
	}
}

void calculate_triangles(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	t_triangle	t;
	t_box		b;
	int			i;

	render->worldspace_ptri_count = 0;
	render->screenspace_ptri_count = 0;
	render->screen_edge.max.x = (float)(sdl.window_w * sdl.resolution_scaling) - 1.0f;
	render->screen_edge.max.y = (float)(sdl.window_h * sdl.resolution_scaling) - 1.0f;
	render->screen_edge.min.x = 0.0f;
	render->screen_edge.min.y = 0.0f;
	t.t[0] = vector2_to_texture((t_vector2){0, 0});
	t.t[1] = vector2_to_texture((t_vector2){0, 1});
	t.t[2] = vector2_to_texture((t_vector2){1, 1});
	t.clr = CLR_BLUE;
	if (entity->obj->bounds.type == bt_plane)
	{
		b = get_entity_box_transformed(entity);
		t.p[0] = vector3_to_quaternion(b.v[0]);
		t.p[1] = vector3_to_quaternion(b.v[2]);
		t.p[2] = vector3_to_quaternion(b.v[3]);
		triangle_to_projection(sdl, render, t);
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
			triangle_to_projection(sdl, render, t);
			t.p[0] = vector3_to_quaternion(b.v[i]);
			t.p[1] = vector3_to_quaternion(b.v[(i + 1) % 4]);
			t.p[2] = vector3_to_quaternion(b.v[((i + 1) % 4) + 4]);
			triangle_to_projection(sdl, render, t);
			i++;
		}
		t.p[0] = vector3_to_quaternion(b.v[0]);
		t.p[1] = vector3_to_quaternion(b.v[1]);
		t.p[2] = vector3_to_quaternion(b.v[2]);
		triangle_to_projection(sdl, render, t);
		t.p[0] = vector3_to_quaternion(b.v[0]);
		t.p[1] = vector3_to_quaternion(b.v[2]);
		t.p[2] = vector3_to_quaternion(b.v[3]);
		triangle_to_projection(sdl, render, t);
		t.p[0] = vector3_to_quaternion(b.v[4]);
		t.p[1] = vector3_to_quaternion(b.v[5]);
		t.p[2] = vector3_to_quaternion(b.v[6]);
		triangle_to_projection(sdl, render, t);
		t.p[0] = vector3_to_quaternion(b.v[4]);
		t.p[1] = vector3_to_quaternion(b.v[6]);
		t.p[2] = vector3_to_quaternion(b.v[7]);
		triangle_to_projection(sdl, render, t);
	}
	else if (entity->obj->bounds.type == bt_ignore)
	{
		t_quaternion	temp;
		t_mat4x4		mattrans;
		
		mattrans = make_transform_matrix(entity->transform);
		i = 0;
		while (i < entity->obj->vertice_count)
		{
			render->q[i] = vector3_to_quaternion(entity->obj->vertices[i]);
			if (entity->animation.active)
				render->q[i].v = vector3_add(entity->obj->o_anim.frames[entity->animation.frame].deltavertices[i].delta, render->q[i].v);
			render->q[i] = quaternion_mul_matrix(mattrans, render->q[i]);
			i++;
		}
		i = 0;
		while (i < entity->obj->face_count)
		{
			t = (t_triangle){render->q[entity->obj->faces[i].v_indices[0] - 1], render->q[entity->obj->faces[i].v_indices[1] - 1], render->q[entity->obj->faces[i].v_indices[2] - 1]};
			triangle_to_projection(sdl, render, t);
			i++;
		}
	}
	clipped_point_triangle(render, sdl);
}

void clear_occlusion_buffer(t_sdlcontext *sdl)
{
	t_tile	temp;

	temp.mask = 0;
	temp.max0 = sdl->bitmask.max_dist + 1000.0f;
	temp.max1 = 0;
	for (int i = 0; i < ((sdl->window_h * sdl->window_w) / 64); i++)
	{
		sdl->bitmask.tile[i] = temp;
	}
}

bool is_entity_occlusion_culled(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_square	chunk;
	const __uint128_t max = ~0;
	float w;
	int y;
	int x;

	chunk = entity->occlusion.box;
	w = entity->occlusion.z_dist[1];
	chunk.min.x = chunk.min.x / 8;
	chunk.max.x = ceilf(chunk.max.x / 8.0f);
	chunk.min.y = chunk.min.y / 8;
	chunk.max.y = ceilf(chunk.max.y / 8.0f);
	entity->occlusion.is_occluded = true;
	entity->occlusion.clip.max.x = 0;
	entity->occlusion.clip.max.y = 0;
	entity->occlusion.clip.min.x = sdl->window_w;
	entity->occlusion.clip.min.y = sdl->window_h;
	y = chunk.min.y;
	while (y <= chunk.max.y)
	{
		x = chunk.min.x;
		while (x <= chunk.max.x)
		{
			if (w < sdl->bitmask.tile[y * sdl->bitmask.tile_chunks.x + x].max0)
			{
				entity->occlusion.is_occluded = false;
				if (entity->occlusion.clip.max.x < x)
					entity->occlusion.clip.max.x = x;
				if (entity->occlusion.clip.max.y < y)
					entity->occlusion.clip.max.y = y;
				if (entity->occlusion.clip.min.x > x)
					entity->occlusion.clip.min.x = x;
				if (entity->occlusion.clip.min.y > y)
					entity->occlusion.clip.min.y = y;
			}
			x++;
		}
		y++;
	}
	entity->occlusion.clip.min = point_mul(entity->occlusion.clip.min, 8);
	entity->occlusion.clip.max = point_mul(entity->occlusion.clip.max, 8);
	entity->occlusion.clip.max.x = ft_clamp(entity->occlusion.clip.max.x, 0, (sdl->window_w * sdl->resolution_scaling) - 1);
	entity->occlusion.clip.max.y = ft_clamp(entity->occlusion.clip.max.y, 0, (sdl->window_h * sdl->resolution_scaling) - 1);
	entity->occlusion.clip.min.x = ft_clamp(entity->occlusion.clip.min.x, 0, (sdl->window_w * sdl->resolution_scaling) - 1);
	entity->occlusion.clip.min.y = ft_clamp(entity->occlusion.clip.min.y, 0, (sdl->window_h * sdl->resolution_scaling) - 1);
	return (entity->occlusion.is_occluded); // unnecessary to return anything
}
