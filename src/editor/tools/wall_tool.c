/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:13:39 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/07 07:27:32 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
//#include "file_io.h"
#include "inputhelp.h"
#include "tools/walltool.h"
#include "objects.h"


static t_quaternion quaternion_to_screenspace(t_render r, t_quaternion q, t_sdlcontext sdl)
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
		t_quaternion q1 = vector3_to_quaternion(wall->object.vertices[0]);
		t_quaternion q2 = vector3_to_quaternion(wall->object.vertices[1]);
		t_quaternion q3 = vector3_to_quaternion(wall->object.vertices[2]);
		t_quaternion q4 = vector3_to_quaternion(wall->object.vertices[3]);
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
			
		//quaternoi
		//wall.object.vertices
		l = l->next;
	}
	return (NULL);
}

static void	draw_snapgrid(t_editor *ed, t_sdlcontext *sdl, t_vector2 wallpos, bool shift)
{
	t_point		indexer;
	t_vector3	origin;
	t_vector3	ws;
	float		clen;

	indexer = point_zero();
	origin = (t_vector3){wallpos.x - 40.0f, wallpos.y - 40.0f, 0.0f};
	clen = 40 + (shift * 500);
	ed->render.gizmocolor = CLR_GRAY;
	while (indexer.y < 90)
	{
		indexer.x = 0;
		while (indexer.x < 90)
		{
			ws = (t_vector3){origin.x + indexer.x, origin.y + indexer.y, 0.0f};
			if (indexer.x == 40 && indexer.y == 40)
			{
				ed->render.gizmocolor = CLR_RED;
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y, clen});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x + clen, ws.y, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x - clen, ws.y, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y + clen, 0.0f});
				render_ray(*sdl, ed->render, ws, (t_vector3){ws.x, ws.y - clen, 0.0f});
			}
			else
				ed->render.gizmocolor = CLR_GRAY;
			render_gizmo(*sdl, ed->render, ws);
			indexer.x += 10;
		}
		//render_gizmo(*sdl, ed->render, v.x)
		indexer.y += 10;
	}
}

static void	wall_tool_draw(t_editor *ed, t_sdlcontext sdl) //TODO: ROTATE AROUND AXIS, SCREENSPACE
{
	t_walltooldata	*dat;
	t_wall			*wall;

	dat = (t_walltooldata *)ed->tool->tooldata;
	wall = &dat->wall;

	if (wall->object.materials[0].img == NULL)
	{
		printf("NULL IMAGE IN WALLTOOLDRAW! '%s' \n", wall->object.materials[0].texturename);
		wall->object.materials[0].img =
			get_image_by_name(sdl, wall->object.materials[0].texturename);
	}
		
	draw_snapgrid(ed, &sdl, wall->line.end, (ed->keystate >> KEYS_SHIFTMASK) & 1);
	ed->render.gizmocolor = CLR_GREEN;
	render_object(sdl, ed->render, &wall->object);

	dat->hover = wallcollision(ed, &sdl);
	if (dat->hover != NULL)
	{
		ed->render.gizmocolor = CLR_RED;
		ed->render.wireframe = true;
		render_object(sdl, ed->render, &dat->hover->object);
		ed->render.wireframe = false;
	}
}

static void	applywallmesh(t_wall *wall)
{
	wall->object.vertices[0] = (t_vector3){wall->line.start.x, wall->line.start.y, 0.0f};
	wall->object.vertices[1] = (t_vector3){wall->line.end.x, wall->line.end.y, 0.0f};
	
	wall->object.vertices[2] = (t_vector3){wall->line.start.x, wall->line.start.y, wall->height};
	wall->object.vertices[3] = (t_vector3){wall->line.end.x, wall->line.end.y, wall->height};

	float dist = vector2_dist(wall->line.start, wall->line.end);
	wall->object.uvs[1] = (t_vector2){dist / 100.0f, 0.0f};
	wall->object.uvs[2] = (t_vector2){0.0f, wall->height / 100.0f};
	wall->object.uvs[3] = (t_vector2){dist / 100.0f, wall->height / 100.0f};
}

void	init_walls(t_world *world)
{
	t_wall	*wall;
	t_list	*l;

	l = world->wall_list;
	while (l != NULL) 
	{
		wall = (t_wall *)l->content;
		wall->object = *object_plane();
		applywallmesh(wall);
		l = l->next;
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
	if (dat->mode == place_first)
	{
		wall->line.start = (t_vector2){rc.x, rc.y};
		wall->line.end = (t_vector2){rc.x, rc.y};
	}
	if (dat->mode == place_end)
	{
		wall->line.end = (t_vector2){rc.x, rc.y};
		wall->height += ed->mouse.scroll_delta * 10;
		wall->height = ft_clamp(wall->height, 50, 150);
	}
	wall->line.start = vector2_snap(wall->line.start, 10);
	wall->line.end = vector2_snap(wall->line.end, 10);
	applywallmesh(wall);
	if (mouse_clicked(ed->mouse, MOUSE_LEFT))
	{
		dat->mode++;
		if (dat->mode == place_height)
		{
			list_push(&ed->world.wall_list, wall, sizeof(t_wall));
			init_walls(&ed->world);
			wall->object = *object_plane();
			dat->mode = 0;
		}
	}
	if (mouse_clicked(ed->mouse, MOUSE_RIGHT) && dat->hover != NULL)
	{
		list_remove(&ed->world.wall_list, dat->hover, sizeof(t_wall));
	}
	
}

static	t_material *wallmat()
{
	/*static t_material	*mat;

	if (mat == NULL)
	{
		mat = ft_memalloc(sizeof)
	}*/
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
		//tool.tooldata = ft_memalloc(sizeof(t_entity));
		tool.tooldata = ft_memalloc(sizeof(t_walltooldata));
		
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
		wtd = (t_walltooldata *)tool.tooldata;
		wtd->wall.object = *object_plane();
		//wtd->wall.object.
		//wtd->wall.object.materials[0].img
		//NEEDS UV COUNT, UVs and texture

		wtd->wall.height = 100.0f;
		wtd->mode = place_first;
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
