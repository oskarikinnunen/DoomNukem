/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npc_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:18:07 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/29 15:06:32 by okinnune         ###   ########.fr       */
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
	drawline(sdl, p1, p2, CLR_GREEN);
	p1 = vector3_to_screenspace(ed->render, t.p[1].v, sdl);
	p2 = vector3_to_screenspace(ed->render, t.p[2].v, sdl);
	drawline(sdl, p1, p2, CLR_GREEN);
	p1 = vector3_to_screenspace(ed->render, t.p[2].v, sdl);
	p2 = vector3_to_screenspace(ed->render, t.p[0].v, sdl);
	drawline(sdl, p1, p2, CLR_GREEN);
	
	ed->render.gizmocolor = CLR_GREEN;
	ed->render.wireframe = false;
	/*render_ray(sdl, ed->render, t.p[0].v, t.p[1].v);
	render_ray(sdl, ed->render, t.p[1].v, t.p[2].v);
	render_ray(sdl, ed->render, t.p[2].v, t.p[0].v);*/
	
}

void	render_collider(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat)
{
	if (dat->col.start_vertex != 0)
	{
		highlight_tri(ed, sdl, dat, dat->col.start_vertex);
	}
	if (dat->col.end_vertex != 0)
	{
		highlight_tri(ed, sdl, dat, dat->col.end_vertex);
	}
}

void	npc_tool_update(t_editor *ed, t_sdlcontext sdl)
{
	t_npctooldata	*dat;
	int32_t			tri_index;

	dat = ed->tool->tooldata;
	npc_tool_lazy_init(ed, sdl, dat);
	//ed->render.wireframe = true;
	ed->render.gizmocolor = CLR_BLUE;
	render_entity(sdl, ed->render, dat->ent);
	ed->render.wireframe = false;
	tri_index = entity_lookedat_triangle_index(ed, sdl, dat->ent);
	char *str = ft_itoa(tri_index);
	draw_text_boxed(&sdl, str, (t_point){ 200, 200}, sdl.screensize);
	free(str);
	highlight_tri(ed, sdl, dat, tri_index);

	if (tri_index != -1 && mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		if (dat->col.start_vertex == 0)
		{
			dat->col.start_vertex = tri_index;
		}
	}
	
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
