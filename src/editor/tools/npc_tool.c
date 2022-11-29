/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npc_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:18:07 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/29 17:20:33 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/npctool.h"
#include "doomnukem.h"

void	npc_tool_lazy_init(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat)
{
	t_vector3	forward;
	if (dat->ent == NULL) 
	{
		dat->ent = ft_memalloc(sizeof(t_entity));
		dat->ent->transform.scale = vector3_one();
		forward = lookdirection_3(ed->player.transform.rotation);
		forward = vector3_mul(forward, 100.0f);
		dat->ent->transform.position = vector3_add(ed->player.transform.position, forward);
		dat->ent->obj = get_object_by_name(sdl, "cyborg.obj");
	}
	if (vector3_cmp(dat->ent->transform.position, vector3_zero()))
	{
		forward = lookdirection_3(ed->player.transform.rotation);
		forward = vector3_mul(forward, 100.0f);
		dat->ent->transform.position = vector3_add(ed->player.transform.position, forward);
		dat->ent->obj = get_object_by_name(sdl, "cyborg.obj");
	}
}

void	clamp_player(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat)
{
	static	t_vector3	prevpos;
	if (vector3_sqr_magnitude(vector3_sub(ed->player.transform.position, dat->ent->transform.position)) > 17500.0f &&
		!vector3_cmp(prevpos, vector3_zero()))
		ed->player.transform.position = prevpos;
	prevpos = ed->player.transform.position;
}

void	highlight_tri(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat, int32_t index)
{
	t_triangle	t;
	t_object	*o;

	if (index == -1)
		return;
	o = dat->ent->obj;
	t.p[0].v = o->vertices[o->faces[index].v_indices[0] - 1];
	t.p[1].v = o->vertices[o->faces[index].v_indices[1] - 1];
	t.p[2].v = o->vertices[o->faces[index].v_indices[2] - 1];
	t.p[0] = transformed_vector3(dat->ent->transform, t.p[0].v);
	t.p[1] = transformed_vector3(dat->ent->transform, t.p[1].v);
	t.p[2] = transformed_vector3(dat->ent->transform, t.p[2].v);
	t_point p1 = vector3_to_screenspace(ed->render, t.p[0].v, sdl);
	t_point p2 = vector3_to_screenspace(ed->render, t.p[1].v, sdl);
	drawcircle(sdl, p1, 10, CLR_GREEN);
	drawline(sdl, p1, p2, CLR_BLUE);
	p1 = vector3_to_screenspace(ed->render, t.p[1].v, sdl);
	p2 = vector3_to_screenspace(ed->render, t.p[2].v, sdl);
	drawline(sdl, p1, p2, CLR_BLUE);
	p1 = vector3_to_screenspace(ed->render, t.p[2].v, sdl);
	p2 = vector3_to_screenspace(ed->render, t.p[0].v, sdl);
	drawline(sdl, p1, p2, CLR_BLUE);
}

t_vector3	*get_vertex(t_entity *e, int32_t	tri_index)
{
	t_object	*o;
	t_vector3	*result;

	o = e->obj;
	result = &o->vertices[o->faces[tri_index].v_indices[0] - 1];
	return (result);
}

static void	render_vertexgizmo(t_editor *ed, t_sdlcontext sdl, t_entity *entity, t_vector3 *vertex)
{
	t_vector3	ws_vertex;
	t_point		ss_vertex;

	ws_vertex = transformed_vector3(entity->transform, *vertex).v;
	ss_vertex = vector3_to_screenspace(ed->render, ws_vertex, sdl);
	drawcircle(sdl, ss_vertex, 6, CLR_RED);
}

void	render_vertexlines(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat)
{
	t_vector3	ws_vertex;
	t_point		ss_vertex1;
	t_point		ss_vertex2;
	t_point		col_rect[4];

	ws_vertex = transformed_vector3(dat->ent->transform, vector3_add(*dat->col.start_vertex, (t_vector3){.x = -10.0f})).v;
	col_rect[0] = vector3_to_screenspace(ed->render, ws_vertex, sdl);
	ws_vertex = transformed_vector3(dat->ent->transform, vector3_add(*dat->col.start_vertex, (t_vector3){.x = +10.0f})).v;
	col_rect[1] = vector3_to_screenspace(ed->render, ws_vertex, sdl);
	ws_vertex = transformed_vector3(dat->ent->transform, vector3_add(*dat->col.end_vertex, (t_vector3){.x = -10.0f})).v;
	col_rect[2] = vector3_to_screenspace(ed->render, ws_vertex, sdl);
	ws_vertex = transformed_vector3(dat->ent->transform, vector3_add(*dat->col.end_vertex, (t_vector3){.x = +10.0f})).v;
	col_rect[3] = vector3_to_screenspace(ed->render, ws_vertex, sdl);
	drawline(sdl, col_rect[0], col_rect[2], CLR_RED);
	drawline(sdl, col_rect[1], col_rect[3], CLR_RED);
}

void	render_collider(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat)
{
	if (dat->col.start_vertex != NULL)
		render_vertexgizmo(ed, sdl, dat->ent, dat->col.start_vertex);
	if (dat->col.end_vertex != NULL)
	{
		render_vertexgizmo(ed, sdl, dat->ent, dat->col.end_vertex);
		render_vertexlines(ed, sdl, dat);
	}
	/*
	if (dat->col.entity.obj != NULL)
	{
		ed->render.wireframe = true;
		ed->render.gizmocolor = CLR_GREEN;
		render_entity(sdl, ed->render, &dat->col.entity);
		ed->render.wireframe = false;
	}*/
}

void	npc_tool_update(t_editor *ed, t_sdlcontext sdl)
{
	t_npctooldata	*dat;

	dat = ed->tool->tooldata;
	npc_tool_lazy_init(ed, sdl, dat);
	//ed->render.wireframe = true;
	render_entity(sdl, ed->render, dat->ent);
	dat->vertex = entity_lookedat_vertex(ed, sdl, dat->ent);
	if (dat->vertex != NULL)
	{
		render_vertexgizmo(ed, sdl, dat->ent, dat->vertex);
		if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
		{
			if (dat->col.start_vertex == NULL)
				dat->col.start_vertex = dat->vertex;
			else if (dat->col.end_vertex == NULL)
			{
				dat->col.end_vertex = dat->vertex;
				dat->col.entity.obj = get_object_by_name(sdl, "cube.obj");
				dat->col.entity.transform.parent = &dat->ent->transform;
				dat->col.entity.transform.scale.x = 10.0f;
				dat->col.entity.transform.scale.y = 10.0f;
				dat->col.entity.transform.scale.z = vector3_dist(*dat->col.start_vertex, *dat->col.end_vertex);
				dat->col.entity.transform.position.x = -dat->col.start_vertex->y;
				dat->col.entity.transform.position.y = dat->col.start_vertex->x;
				dat->col.entity.transform.position.z = dat->col.start_vertex->z + (dat->col.entity.transform.scale.z / 2.0f);
				//dat->col.entity.transform.position = vector3_zero();
			}
				
		}
	}
	if (instant_text_button(sdl, &ed->hid.mouse, "rotate", (t_point){100, 100}))
		dat->ent->transform.rotation.x += ft_degtorad(15.0f);
	render_collider(ed, sdl, dat);
	clamp_player(ed, sdl, dat);
}

void	npc_tool_cleanup(t_editor *ed, t_sdlcontext sdl)
{
	t_npctooldata	*dat;

	dat = ed->tool->tooldata;
	dat->ent->transform.position = vector3_zero();
}

t_tool	*get_npc_tool()
{
	static t_tool	tool
	= {
		npc_tool_update, npc_tool_cleanup
	};
	t_npctooldata	*dat;
	t_entity		*ent;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_npctooldata));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
		dat = (t_npctooldata *)tool.tooldata;
		/*dat->ent->transform.location = vector3_zero();
		dat->ent->transform.scale = vector3_one();*/
	}
	ft_strcpy(tool.icon_name, "entitytool.png");
	return (&tool);
}
