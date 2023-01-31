/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npc_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:18:07 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/31 16:11:29 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/npctool.h"
#include "colliders.h"
#include "doomnukem.h"

void	npc_tool_lazy_init(t_editor *ed, t_sdlcontext *sdl, t_npctooldata *dat)
{
	t_vector3	forward;
	if (dat->ent == NULL) 
	{
		dat->ent = ft_memalloc(sizeof(t_entity));
		dat->ent->transform.scale = vector3_one();
		forward = lookdirection_3(ed->player.transform.rotation);
		forward = vector3_mul(forward, 100.0f);
		dat->ent->transform.position = vector3_add(ed->player.transform.position, forward);
		dat->ent->obj = get_object_by_name(*sdl, "cyborg.obj");
	}
	if (vector3_cmp(dat->ent->transform.position, vector3_zero()))
	{
		forward = lookdirection_3(ed->player.transform.rotation);
		forward = vector3_mul(forward, 100.0f);
		dat->ent->transform.position = vector3_add(ed->player.transform.position, forward);
		dat->ent->obj = get_object_by_name(*sdl, "cyborg.obj");
	}
	if (dat->main_gui.sdl == NULL)
		dat->main_gui = init_gui(sdl, &ed->hid, &ed->player ,(t_point) {20, 60}, "Collider tool");
	if (dat->object_gui.sdl == NULL)
	{
		dat->object_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {200, 60}, "Select object");
		dat->object_gui.hidden = true;
		dat->object_gui.allow_user_hide = true;
		dat->object_gui.dock = &dat->main_gui;
	}
	if (dat->collider_gui.sdl == NULL)
	{
		dat->collider_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {200, 60}, "Edit collider");
		dat->collider_gui.hidden = true;
		dat->collider_gui.allow_user_hide = true;
		dat->collider_gui.dock = &dat->main_gui;
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
	t_point p1 = vector3_to_screenspace(t.p[0].v, sdl);
	t_point p2 = vector3_to_screenspace(t.p[1].v, sdl);
	drawcircle(sdl, p1, 10, CLR_GREEN);
	drawline(sdl, p1, p2, CLR_BLUE);
	p1 = vector3_to_screenspace(t.p[1].v, sdl);
	p2 = vector3_to_screenspace(t.p[2].v, sdl);
	drawline(sdl, p1, p2, CLR_BLUE);
	p1 = vector3_to_screenspace(t.p[2].v, sdl);
	p2 = vector3_to_screenspace(t.p[0].v, sdl);
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
	ss_vertex = vector3_to_screenspace(ws_vertex, sdl);
	drawcircle(sdl, ss_vertex, 6, CLR_RED);
}

t_vector3	crss_add(t_vector3 vertex)
{
	t_vector3	forward;
	forward = vector3_crossproduct(vertex, vector3_up());
	forward = vector3_mul(forward, 10.0f);
	return (vector3_add(vertex, forward));
}

int		xyz_magnitude(t_transform *t, t_vector3 vec, t_render *r, t_sdlcontext *sdl)
{
	int			x;
	int			y;
	int			z;
	t_vector3	ws_vertex;
	t_point		ss_orig;
	t_point		ss_new;

	ws_vertex = transformed_vector3(*t, vec).v;
	ss_orig = vector3_to_screenspace(ws_vertex, *sdl);
	ws_vertex = transformed_vector3(*t, vector3_add(vec, (t_vector3){.x = 2.0f})).v;
	ss_new = vector3_to_screenspace(ws_vertex, *sdl);
	x = point_dist(ss_orig, ss_new);

	ws_vertex = transformed_vector3(*t, vector3_add(vec, (t_vector3){.y = 2.0f})).v;
	ss_new = vector3_to_screenspace(ws_vertex, *sdl);
	y = point_dist(ss_orig, ss_new);

	ws_vertex = transformed_vector3(*t, vector3_add(vec, (t_vector3){.z = 2.0f})).v;
	ss_new = vector3_to_screenspace(ws_vertex, *sdl);
	z = point_dist(ss_orig, ss_new);
	return (ft_max(ft_max(x, y), z));
}

void	render_vertexlines(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat)
{
	t_vector3	ws_vertex;
	t_point		ss_vertex1;
	t_point		ss_vertex2;
	t_vector2	normal;
	t_vector2	normaltemp;
	t_point		col_rect[4];

	ws_vertex = transformed_vector3(dat->ent->transform, *dat->col.start_vertex).v;
	col_rect[0] = vector3_to_screenspace(ws_vertex, sdl);
	ws_vertex = transformed_vector3(dat->ent->transform, *dat->col.end_vertex).v;
	col_rect[2] = vector3_to_screenspace(ws_vertex, sdl);
	drawline(sdl, col_rect[0], col_rect[2], CLR_RED);
	//drawline(sdl, col_rect[1], col_rect[3], CLR_RED);
	ss_vertex1 = point_sub(col_rect[0], col_rect[2]);
	int			temp = ss_vertex1.x;
	ss_vertex1.x = -ss_vertex1.y;
	ss_vertex1.y = temp;
	int	xyz = xyz_magnitude(&dat->ent->transform, *dat->col.start_vertex, &sdl.render, &sdl);
	normal = point_to_vector2(ss_vertex1);
	normal = vector2_normalise(normal);
	//Normal for start + pos x
	normaltemp = vector2_mul(normal, (float)xyz * 2.0f);
	normaltemp = vector2_add(point_to_vector2(col_rect[0]), normaltemp);
	ss_vertex1 = vector2_to_point(normaltemp);
	//Normal for start + neg x
	normaltemp = vector2_mul(normal, (float)xyz * -2.0f);
	normaltemp = vector2_add(point_to_vector2(col_rect[0]), normaltemp);
	ss_vertex2 = vector2_to_point(normaltemp);
	//drawline(sdl, col_rect[0], ss_vertex1, CLR_BLUE);
	drawline(sdl, ss_vertex1, ss_vertex2, CLR_BLUE);
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
		sdl->render.wireframe = true;
		sdl->render.gizmocolor = CLR_GREEN;
		render_entity(sdl, &sdl->render, &dat->col.entity);
		sdl->render.wireframe = false;
	}*/
}


void	render_box_colliders(t_entity *ent, t_editor *ed, t_sdlcontext sdl)
{
	t_entity		box;
	t_boxcollider	bc;
	int				i;

	ft_bzero(&box, sizeof(t_entity));
	box.obj = get_object_by_name(sdl, "cube.obj");
	if (ent->obj->col == NULL)
		return;
	i = 0;

	while (i < ent->obj->col->boxcol_count)
	{
		bc = ent->obj->col->boxcolliders[i];
		if (bc.vertex == NULL)
		{
			box.transform.parent = &ent->transform;
			box.transform.position = bc.offset;
			box.transform.scale = bc.size;
		}
		else
		{
			box.transform.parent = &ent->transform;
			box.transform.position = *bc.vertex;
			box.transform.position = vector3_add(box.transform.position, bc.offset);
			box.transform.scale = bc.size;
		}
		box.transform.scale = bc.size;
		sdl.render.wireframe = true;
		sdl.render.gizmocolor = CLR_GREEN;
		render_entity(&sdl, &sdl.render, &box);
		sdl.render.wireframe = false;
		i++;
	}
}

void	modify_box_collider_vertex(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat)
{
	t_boxcollider	*bc;

	bc = &dat->metadata.col.boxcolliders[dat->i_col];
	vector3_slider("size", &bc->size, ed, sdl, (t_point){20, 200});
	vector3_slider("offset", &bc->offset, ed, sdl, (t_point){20, 240});
	dat->ent->obj->col = &dat->metadata.col;
	if (!dat->vertex_set)
		bc->vertex = entity_lookedat_vertex(ed, sdl, dat->ent);
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
		dat->vertex_set = true;
	else if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->vertex_set = false;
	render_box_colliders(dat->ent, ed, sdl);
}

void	modify_box_collider(t_editor *ed, t_sdlcontext sdl, t_npctooldata *dat)
{
	//t_entity	ent;
	t_boxcollider	*bc;

	bc = &dat->metadata.col.boxcolliders[dat->i_col];
	vector3_slider("size", &bc->size, ed, sdl, (t_point){20, 200});
	vector3_slider("offset", &bc->offset, ed, sdl, (t_point){20, 240});
	dat->ent->obj->col = &dat->metadata.col;
	render_box_colliders(dat->ent, ed, sdl);
}

/*void	autoguitest(t_editor *ed, t_sdlcontext sdl)
{
	static t_autogui	autogui;
	static int			i = 0;
	static float		f = 0.0f;
	static t_vector3	vec = {0.0f};

	if (autogui.hid == NULL)
	{
		autogui = init_gui(&sdl, &ed->hid, (t_point){20, 80}, "Collider Tool");
		autogui.min_x = 80;
	}
	gui_start(&autogui);
	gui_starthorizontal(&autogui);
	gui_label("First label!", &autogui);
	gui_label("Second label!", &autogui);
	gui_endhorizontal(&autogui);
	gui_label("vertical mode", &autogui);
	gui_label("vmode2", &autogui);
	gui_int_slider(&i, 10, &autogui);
	if (gui_button("Buttontest", &autogui)) {
		debugconsole_addmessage(&ed->world.debugconsole, "Pushed autogui button \n");
	}
	gui_labeled_float_slider("Somefloat:", &f, 0.5f, &autogui);
	gui_labeled_vector3_slider("position:", &vec, 0.5f, &autogui);
	gui_update(&autogui);
}*/

void	maingui(t_editor *ed, t_sdlcontext *sdl, t_npctooldata *dat)
{
	t_autogui		*gui;
	t_boxcollider	*bc;

	gui = &dat->main_gui;
	gui_start(gui);
	if (gui_button("Select object ->", gui))
	{
		dat->collider_gui.hidden = true; //if this gets any more complicated, just link the state of these to eachother so cgui.hidden is always = !ogui.hidden
		dat->object_gui.hidden = false;
	}
	if (gui_button("Add new collider", gui))
	{
		bc = &dat->metadata.col.boxcolliders[dat->metadata.col.boxcol_count];
		dat->i_col = dat->metadata.col.boxcol_count++;
		bc->vertex = NULL;
		bc->size = (t_vector3) {10.0f, 10.0f, 10.0f};
		bc->offset = vector3_zero();
		dat->object_gui.hidden = true;
		dat->collider_gui.hidden = false;
		dat->ent->obj->col = &dat->metadata.col;
	}
	gui_end(gui);
}

void	objectgui(t_editor *ed, t_sdlcontext *sdl, t_npctooldata *dat)
{
	t_autogui	*gui;
	t_object	*obj;
	int			i;

	gui = &dat->object_gui;
	gui_start(gui);
	i = 0;
	while (i < sdl->objectcount)
	{
		obj = &sdl->objects[i];
		if (gui_button(obj->name, gui))
		{
			dat->ent->obj = obj;
			gui->hidden = true;
		}
		i++;
	}
	gui_end(gui);
}

void	collidergui(t_editor *ed, t_sdlcontext *sdl, t_npctooldata *dat)
{
	t_autogui		*gui;
	t_boxcollider	*bc;

	gui = &dat->collider_gui;
	bc = &dat->metadata.col.boxcolliders[dat->i_col];
	gui_start(gui);
	gui_labeled_float_slider("Rotate character preview:", &dat->ent->transform.rotation.x, 0.02f, gui);
	gui_labeled_vector3_slider("Offset:", &bc->offset, 0.5f, gui);
	gui_labeled_vector3_slider("Size:", &bc->size, 0.5f, gui);
	gui_end(gui);
	render_box_colliders(dat->ent, ed, *sdl);
}

void	npc_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_npctooldata	*dat;
	t_boxcollider	*bc;

	dat = ed->tool->tooldata;
	npc_tool_lazy_init(ed, sdl, dat);
	//autoguitest(ed, sdl);
	//sdl->render.wireframe = true;
	render_entity(sdl, &sdl->render, dat->ent);
	maingui(ed, sdl, dat);
	objectgui(ed, sdl, dat);
	collidergui(ed, sdl, dat);
	/*
	if (instant_text_button(sdl, &ed->hid.mouse, "Select Object", (t_point) {20, 80}))
		dat->ntm = ntm_select;
	if (dat->ntm == ntm_select)
	{
		dat->ent->obj = object_selector2(ed, sdl, dat->ent);
	}
	if (instant_text_button(sdl, &ed->hid.mouse, "Add box collider (vertex independent)", (t_point){20, 100}))
	{
		dat->ntm = ntm_addbox;
		bc = &dat->metadata.col.boxcolliders[dat->metadata.col.boxcol_count];
		dat->i_col = dat->metadata.col.boxcol_count++;
		bc->vertex = NULL;
		bc->size = (t_vector3) {10.0f, 10.0f, 10.0f};
		bc->offset = vector3_zero();
	}
	if (instant_text_button(sdl, &ed->hid.mouse, "Add box collider (vertex bound)", (t_point){20, 140}))
	{
		dat->ntm = ntm_addvbox;
		bc = &dat->metadata.col.boxcolliders[dat->metadata.col.boxcol_count];
		dat->i_col = dat->metadata.col.boxcol_count++;
		bc->vertex = NULL;
		bc->size = (t_vector3) {10.0f, 10.0f, 10.0f};
		bc->offset = vector3_zero();
	}
	if (dat->ntm == ntm_addvbox)
		modify_box_collider_vertex(ed, sdl, dat);
	if (dat->ntm == ntm_addbox)
		modify_box_collider(ed, sdl, dat);
	if (dat->metadata.col.boxcol_count >= 1
		&& instant_text_button(sdl, &ed->hid.mouse, "Remove last collider", (t_point) {20, 120}))
		dat->i_col = --dat->metadata.col.boxcol_count;
	if (instant_text_button(sdl, &ed->hid.mouse, "rotate", (t_point){400, 100}))
		dat->ent->transform.rotation.x += ft_degtorad(15.0f); */
	render_collider(ed, *sdl, dat);
	clamp_player(ed, *sdl, dat);
}

void	npc_tool_cleanup(t_editor *ed, t_sdlcontext *sdl)
{
	t_npctooldata	*dat;

	dat = ed->tool->tooldata;
	dat->ent->transform.position = vector3_zero();
}

t_tool	*get_npc_tool()
{
	static t_tool	tool
	= {
		.update = npc_tool_update, .cleanup = npc_tool_cleanup
	};
	t_npctooldata	*dat;
	t_entity		*ent;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_npctooldata));
		if (tool.tooldata == NULL)
			doomlog(LOGEC_MALLOC, NULL);
		dat = (t_npctooldata *)tool.tooldata;
		/*dat->ent->transform.position = vector3_zero();
		dat->ent->transform.scale = vector3_one();*/
	}
	ft_strcpy(tool.icon_name, "entitytool.png");
	return (&tool);
}
