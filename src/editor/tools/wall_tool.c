/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:13:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/09 15:01:22 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "inputhelp.h"
#include "tools/walltool.h"
#include "objects.h"

static t_point vector3_to_screenspace(t_render r, t_vector3 vec, t_sdlcontext sdl)
{
	t_quaternion	proj_q;
	t_point			result;

	proj_q = vector3_to_quaternion(vec);
	proj_q = quaternion_mul_matrix(r.matworld, proj_q);
	proj_q = quaternion_mul_matrix(r.matview, proj_q);
	proj_q = quaternion_mul_matrix(r.matproj, proj_q);


	proj_q.v = vector3_div(proj_q.v, proj_q.w);

	proj_q.v = vector3_negative(proj_q.v);

	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, voffsetview);

	proj_q.v.x *= 0.5f * (float)sdl.window_w;
	proj_q.v.y *= 0.5f * (float)sdl.window_h;

	if (proj_q.w < 0.0f)
		return ((t_point) {-100, -100});
	result = (t_point){proj_q.v.x, proj_q.v.y};
	return(result);
}

t_quaternion quaternion_to_screenspace(t_render r, t_quaternion q, t_sdlcontext sdl)
{
	t_quaternion	proj_q;

	proj_q = quaternion_mul_matrix(r.matworld, q);
	proj_q = quaternion_mul_matrix(r.matview, proj_q);
	proj_q = quaternion_mul_matrix(r.matproj, proj_q);


	proj_q.v = vector3_div(proj_q.v, proj_q.w);

	proj_q.v = vector3_negative(proj_q.v);

	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, voffsetview);

	proj_q.v.x *= 0.5f * (float)sdl.window_w;
	proj_q.v.y *= 0.5f * (float)sdl.window_h;

	return(proj_q);
}

static bool quat_offscreen(t_sdlcontext *sdl, t_quaternion q)
{
	return (q.v.x < 0.0f || q.v.x > sdl->window_w
		|| q.v.y < 0.0f || q.v.y > sdl->window_h);
}

static bool has_selected(t_walltooldata *dat)
{
		int	i;

	i = 0;
	while (i < MAXSELECTED)
	{
		if (dat->selected[i] != NULL)
			return (true);
		i++;
	}
	return (false);
}

t_wall	*wallcollision(t_editor *ed, t_sdlcontext *sdl)
{
	t_list	*l;
	t_wall	*wall;

	l = ed->world.wall_list;
	while (l != NULL)
	{
		wall = (t_wall *)l->content;
		t_triangle tri;
		t_triangle tri2;
		t_quaternion q1 = vector3_to_quaternion(wall->entity.obj->vertices[0]);
		t_quaternion q2 = vector3_to_quaternion(wall->entity.obj->vertices[1]);
		t_quaternion q3 = vector3_to_quaternion(wall->entity.obj->vertices[2]);
		t_quaternion q4 = vector3_to_quaternion(wall->entity.obj->vertices[3]);
		q1 = quaternion_to_screenspace(ed->render, q1, *sdl);
		q2 = quaternion_to_screenspace(ed->render, q2, *sdl);
		q3 = quaternion_to_screenspace(ed->render, q3, *sdl);
		q4 = quaternion_to_screenspace(ed->render, q4, *sdl);
		tri.p[0] = q1;
		tri.p[1] = q2;
		tri.p[2] = q3;
		tri2.p[0] = q2;
		tri2.p[1] = q3;
		tri2.p[2] = q4;
		if (quat_offscreen(sdl, q1) || quat_offscreen(sdl, q2)
			|| quat_offscreen(sdl, q3) || quat_offscreen(sdl, q4))
			;
		else if (pointtrianglecollision((t_point){sdl->window_w / 2, sdl->window_h / 2}, tri)
			|| pointtrianglecollision((t_point){sdl->window_w / 2, sdl->window_h / 2}, tri2))
		{
			return (wall);
		}
		l = l->next;
	}
	return (NULL);
}

static void	draw_snapgrid(t_editor *ed, t_sdlcontext *sdl, t_vector2 wallpos, bool shift, bool hover)
{
	t_point		indexer;
	t_vector3	origin;
	t_vector3	ws;
	float		clen;

	indexer = point_zero();
	origin = (t_vector3){wallpos.x - 40.0f, wallpos.y - 40.0f, 0.0f};
	clen = 40 + (shift && !hover) * 220;
	ed->render.gizmocolor = CLR_GRAY;
	while (indexer.y < 90)
	{
		indexer.x = 0;
		while (indexer.x < 90)
		{
			ws = (t_vector3){origin.x + indexer.x, origin.y + indexer.y, 0.0f};
			if (indexer.x == 40 && indexer.y == 40)
			{
				if (hover)
					ed->render.gizmocolor = CLR_RED;
				else
					ed->render.gizmocolor = CLR_PRPL;
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y, clen});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x + clen, ws.y, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x - clen, ws.y, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y + clen, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y - clen, 0.0f});
			}
			else
				ed->render.gizmocolor = CLR_GRAY;
			render_gizmo(*sdl, ed->render, ws, 2);
			indexer.x += 10;
		}
		//render_gizmo(*sdl, ed->render, v.x)
		indexer.y += 10;
	}
}

static void draw_selected(t_editor *ed, t_sdlcontext sdl, t_walltooldata *dat)
{
	int			i;
	int			count;
	char		text[256] = { };

	i = 0;
	count = 0;
	ed->render.rendermode = wireframe;
	ed->render.gizmocolor = CLR_PRPL;
	while (i < MAXSELECTED)
	{
		if (dat->selected[i] != NULL)
		{
			sprintf(text, "room: %i wallheight %i", dat->selected[i]->roomindex, dat->selected[i]->height);
			t_vector3 corner = vector3_add(dat->selected[i]->entity.obj->vertices[0], dat->selected[i]->entity.obj->vertices[3]);
			corner = vector3_div(corner, 2.0f);
			draw_text_boxed(&sdl, text, vector3_to_screenspace(ed->render, corner, sdl), sdl.screensize);
			render_entity(sdl, &ed->render, &dat->selected[i]->entity);
			count++;
		}
		i++;
	}
	if (count != 0)
	{
		ft_bzero(text, 256);
		sprintf(text, "selected %i walls", count);
		draw_text_boxed(&sdl, text, (t_point) {20, 120}, sdl.screensize);
	}
	ed->render.rendermode = fill;
}

static void	wall_tool_draw(t_editor *ed, t_sdlcontext sdl) //TODO: ROTATE AROUND AXIS, SCREENSPACE
{
	t_walltooldata	*dat;
	t_wall			*wall;

	dat = (t_walltooldata *)ed->tool->tooldata;
	wall = &dat->wall;
	dat->hover = wallcollision(ed, &sdl);
	if (wall->entity.obj->materials[0].img == NULL)
	{
		printf("NULL IMAGE IN WALLTOOLDRAW! '%s' \n", wall->entity.obj->materials[0].texturename);
		wall->entity.obj->materials[0].img =
			get_image_by_name(sdl, wall->entity.obj->materials[0].texturename);
	}
	draw_selected(ed, sdl, dat);
	draw_snapgrid(ed, &sdl, wall->line.end, (ed->keystate >> KEYS_SHIFTMASK) & 1, dat->hover != NULL);
	ed->render.gizmocolor = CLR_GREEN;
	render_entity(sdl, &ed->render, &wall->entity);
	if (dat->hover != NULL)
	{
		drawcircle(sdl, point_div(sdl.screensize, 2), 2, CLR_RED);
		//char wallinfo[128];
		//sprintf(wallinfo, "room: %i height: %i", dat->hover->roomindex, dat->hover->height);
		//draw_text_boxed(&sdl, wallinfo, (t_point) { 20, 80}, (t_point){sdl.window_w, sdl.window_h});
		ed->render.gizmocolor = CLR_RED;
		ed->render.rendermode = wireframe;
		render_entity(sdl, &ed->render, &dat->hover->entity);
		ed->render.rendermode = fill;
	}

	if (has_selected(dat))
	{
		if (instantbutton((t_rectangle){20, 180, 80, 80}, &ed->mouse, sdl, "debug1.png"))
			dat->fc = generate_floor(dat);
	}
	if (dat->fc.edgecount != 0)
		floorcalc_debugdraw(ed, &sdl, dat->fc);
}

static void unselect_wall(t_walltooldata *dat)
{
	int	i;

	i = 0;
	while (i < MAXSELECTED)
	{
		if (dat->selected[i] == dat->hover)
		{
			dat->selected[i] = NULL;
			return ;
		}
		i++;
	}
}

static void	raise_selected(t_walltooldata *dat, int mousedelta)
{
	int	i;

	i = 0;
	while (i < MAXSELECTED)
	{
		if (dat->selected[i] != NULL)
		{
			dat->selected[i]->height += mousedelta * 10;
			dat->selected[i]->height = ft_clamp(dat->selected[i]->height, 0, 200);
			applywallmesh(dat->selected[i]);
		}
		i++;
	}
}

bool	has_points_incommon(t_walltooldata *dat, t_wall *check)
{
	int		i;
	t_wall	*cur;

	i = 0;
	while (i < MAXSELECTED)
	{
		cur = dat->selected[i];
		if (cur != NULL
			&& (vector2_cmp(check->line.start, cur->line.start)
			|| vector2_cmp(check->line.start, cur->line.end)
			|| vector2_cmp(check->line.end, cur->line.start)
			|| vector2_cmp(check->line.end, cur->line.end)))
			return (true);
		i++;
	}
	return (false);
}

static void	select_wall(t_walltooldata *dat, t_wall *select)
{
	int	i;

	i = 0;
	while (i < MAXSELECTED)
	{
		if (dat->selected[i] == select)
		{
			dat->selected[i] = NULL;
			return ;
		}
		if (dat->selected[i] == NULL)
		{
			if (has_points_incommon(dat, select) || !has_selected(dat))
				dat->selected[i] = select;
			return ;
		}
		i++;
	}
}

static void	wall_tool_update(t_editor *ed) //This needs to access editors state, so pass editor here??
{
	t_walltooldata	*dat;
	t_wall			*wall;
	t_vector3		rc;

	dat = (t_walltooldata *)ed->tool->tooldata;
	wall = &dat->wall;
	rc = raycast(ed);
	if (has_selected(dat) && dat->mode != place_end)
	{
		raise_selected(dat, ed->mouse.scroll_delta);
		if (mouse_clicked(ed->mouse, MOUSE_RIGHT) && dat->hover == NULL)
			ft_bzero(dat->selected, sizeof(t_wall *) * MAXSELECTED);
	}
	if (dat->hover == NULL)
	{
		if (dat->mode == place_first)
		{
			wall->line.start = (t_vector2){rc.x, rc.y};
			wall->line.end = (t_vector2){rc.x, rc.y};
		}
		if (dat->mode == place_end)
		{
			wall->line.end = (t_vector2){rc.x, rc.y};
			wall->height += ed->mouse.scroll_delta * 10;
			wall->height = ft_clamp(wall->height, 50, 200);
		}
		wall->line.start = vector2_snap(wall->line.start, 10);
		wall->line.end = vector2_snap(wall->line.end, 10);
		applywallmesh(wall);
		if (mouse_clicked(ed->mouse, MOUSE_RIGHT) && dat->mode == place_end)
			dat->mode = place_start;
		if (mouse_clicked(ed->mouse, MOUSE_LEFT))
		{
			dat->mode++;
			if (dat->mode == place_height)
			{
				list_push(&ed->world.wall_list, wall, sizeof(t_wall));
				walls_init(&ed->world);
				dat->mode = 0;
				if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
				{
					select_wall(dat, list_findlast(ed->world.wall_list));
					wall->line.start = (t_vector2){rc.x, rc.y};
					wall->line.end = (t_vector2){rc.x, rc.y};
					dat->mode++;
				}
			}
		}
	}
	else
	{
		if (mouse_clicked(ed->mouse, MOUSE_LEFT))
			select_wall(dat, dat->hover);
		if (mouse_clicked(ed->mouse, MOUSE_RIGHT))
		{
			list_remove(&ed->world.wall_list, dat->hover, sizeof(t_wall));
			unselect_wall(dat);
		}
	}
}

t_tool	*get_wall_tool()
{
	static t_tool	tool
	= {
		wall_tool_update, wall_tool_draw
	};
	t_walltooldata	*wtd;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_walltooldata));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
		wtd = (t_walltooldata *)tool.tooldata;
		wtd->wall.entity.transform.scale = vector3_one();
		wtd->wall.entity.transform.location = vector3_zero();
		wtd->wall.entity.obj = object_plane();
		wtd->wall.height = 100.0f;
		wtd->mode = place_first;
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
