/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/11 16:40:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"
#include "inputhelp.h"

static t_wall *firstwall(t_wall *from)
{
	t_wall	*first;

	first = from;
	while (first->prev != NULL)
		first = first->prev;
	return (first);
}

static void render_selected(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	t_wall	*cur;
	
	cur = &dat->wall;
	ed->render.wireframe = true;
	while (cur != NULL)
	{
		
		ed->render.gizmocolor = CLR_GREEN;
		render_object(sdl, ed->render, &cur->object);
		cur = cur->prev;
	}
	ed->render.wireframe = false;
}

static void render_connect(t_editor *ed, t_sdlcontext sdl, t_roomtooldata dat)
{
	t_wall	*first;
	t_wall	*last;
	t_wall	*w;
	
	w = &dat.wall;
	while (w->prev != NULL)
		w = w->prev;
	first = w;
	last = &dat.wall;
	if (first != NULL && last != NULL)
	{
		t_vector3 from = (t_vector3){first->line.start.x, first->line.start.y, 0.0f};
		t_vector3 to = (t_vector3){last->line.end.x, last->line.end.y, 0.0f};
		ed->render.gizmocolor = CLR_PRPL;
		if (from.x == to.x || from.y == to.y)
			ed->render.gizmocolor = CLR_GREEN;
		render_ray(sdl, ed->render, from, to);
	}
}

static void	createmode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	t_wall			*cur;
	t_vector3		rc;
	t_vector2		snap;

	cur = &dat->wall;
	rc = raycast(ed);
	snap = vector2_snap((t_vector2){rc.x, rc.y}, 10);
	bool placefirst = (cur->prev == NULL && cur->next == NULL
		&& vector2_cmp(cur->line.start, vector2_zero()));
	if (placefirst)
		draw_text_boxed(&sdl, "place room starting point", (t_point) {20, 200}, sdl.screensize);
	cur->line.end = snap;
	if (cur->object.materials->img == NULL)
		cur->object.materials->img = get_image_by_name(sdl, cur->object.materials->texturename);
	if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		cur->line.end = firstwall(cur)->line.start;
	if (mouse_clicked(ed->mouse, MOUSE_LEFT))
	{
		if (placefirst)
			cur->line.start = snap;
		else
		{
			list_push(&ed->world.wall_list, cur, sizeof(t_wall));
			((t_wall *)list_findlast(ed->world.wall_list))->next = cur;
			cur->prev = ((t_wall *)list_findlast(ed->world.wall_list));
			walls_init(&ed->world, &sdl);
			cur->line.start = snap;
		}
		if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		{
			dat->rtm = rtm_modify;
			return ;
			//applywallmesh(cur);
		}
	}
	else if (mouse_clicked(ed->mouse, MOUSE_RIGHT))
	{
		if (cur->prev != NULL)
		{
			dat->wall = *((t_wall *)list_findlast(ed->world.wall_list));
			list_remove(&ed->world.wall_list, &dat->wall, sizeof(t_wall));
		}
		else
		{
			cur->line.start = vector2_zero();
			cur->prev = NULL;
			cur->next = NULL;
		}
	}
	applywallmesh(cur);
	if (!placefirst)
	{
		if (cur->prev != NULL)
		{
			float angle = vector2_anglebetween(cur->prev->line.end, cur->line.end);
			angle += vector2_anglebetween(cur->prev->line.end, cur->prev->line.start);
			char	*str;
			str = ft_itoa(radtodeg(angle));
			t_point p =
				vector3_to_screenspace(ed->render, (t_vector3){cur->line.start.x, cur->line.start.y, 0.0f}, sdl);
			draw_text_boxed(&sdl, str, p, sdl.screensize);
			free(str);
		}
		render_object(sdl, ed->render, &cur->object);
		render_connect(ed, sdl, *dat);
	}
	render_snapgrid(ed, &sdl, snap, false, false);
}

void	modifymode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	t_wall			*cur;
	t_vector3		rc;
	t_vector2		snap;

	cur = &dat->wall;
	rc = raycast(ed);
	snap = vector2_snap((t_vector2){rc.x, rc.y}, 10);
	render_snapgrid(ed, &sdl, snap, false, false);

	if (instantbutton((t_rectangle){20, 200, 60, 60}, &ed->mouse, sdl, "debug1.png"))
	{
		printf("FLOOR GENERATE!\n");
	}
	render_selected(ed, sdl, dat);
	//walls_init(&ed->world);
	//applywallmesh(cur);
}

void	room_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_roomtooldata	*dat;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	if (dat->rtm == rtm_create)
		createmode(ed, sdl, dat);
	else if (dat->rtm == rtm_modify)
		modifymode(ed, sdl, dat);
}

void	room_tool_update(t_editor *ed)
{
	/*t_roomtooldata	*dat;
	t_wall			*cur;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	cur = &dat->wall;*/

	
	/*if (cur->next == NULL && cur->prev == NULL)
	{

	}*/
}

t_tool	*get_room_tool()
{
	static t_tool	tool
	= {
		room_tool_update, room_tool_draw
	};
	t_roomtooldata		*dat;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_roomtooldata));
		dat = (t_roomtooldata *)tool.tooldata;
		//ft_bzero(&dat->wall, sizeof(t_wall));
		dat->wall.object = *object_plane();
		dat->wall.height = 100.0f;
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
