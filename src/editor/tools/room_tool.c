/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/15 17:35:51 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"
#include "inputhelp.h"

static void highlight_room(t_editor *ed, t_sdlcontext sdl, t_room room)
{
	int	i;

	i = 0;
	ed->render.wireframe = true;
	ed->render.gizmocolor = CLR_GREEN;
	while (i < room.wallcount)
	{
		render_entity(sdl, ed->render, &room.walls[i].entity);
		i++;
	}
	ed->render.wireframe = false;
}

static void render_connect(t_editor *ed, t_sdlcontext sdl, t_roomtooldata dat)
{
	t_wall	*first;
	t_wall	*last;
	t_wall	*w;
	
	w = &dat.wall;
	/*while (w->prev != NULL)
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
	}*/
}

static void ptr_add(void **ptr, uint32_t *len, size_t size, void *add)
{
	void	*newptr;

	newptr = ft_memalloc((*len + 1) * size);
	if (newptr == NULL)
		error_log(EC_MALLOC);
	if (*ptr != NULL)
	{
		ft_memcpy(newptr, *ptr, *len * size);
		free (*ptr);
	}
	printf("newptr loc %lu catting mem to %lu \n", newptr, newptr + ((*len) * size));
	ft_memcpy(newptr + ((*len) * size), add, size);
	*len = *len + 1;
	*ptr = newptr;
}

static void renderroom(t_editor *ed, t_sdlcontext sdl, t_room room)
{
	int	i;

	i = 0;
	while (i < room.wallcount)
	{
		render_entity(sdl, ed->render, &room.walls[i].entity);
		i++;
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
	/*bool placefirst = (cur->prev == NULL && cur->next == NULL
		&& vector2_cmp(cur->line.start, vector2_zero()));*/
	if (dat->room.wallcount == 0)
		draw_text_boxed(&sdl, "place room starting point", (t_point) {20, 200}, sdl.screensize);
	cur->line.end = snap;
	if (dat->room.wallcount == 0)
		cur->line.start = snap;
	if (cur->entity.obj == NULL)
		cur->entity.obj = object_plane(&sdl);
	if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		cur->line.end = dat->room.walls[0].line.start;
	if (mouse_clicked(ed->mouse, MOUSE_LEFT))
	{
		
		ptr_add((void **)(&dat->room.walls), &dat->room.wallcount, sizeof(t_wall), cur);
		init_roomwalls(&dat->room, &sdl);
		printf("wallcount in room %i \n", dat->room.wallcount);
		cur->line.start = snap;
		if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		{
			dat->rtm = rtm_modify;
			snprintf(dat->room.name, 32, "room%i", ft_listlen(ed->world.roomlist));
			printf("room name: %s \n", dat->room.name);
			//sprintf(dat->room.name, "room_%i", )
			return ;
			//applywallmesh(cur);
		}
	}
	else if (mouse_clicked(ed->mouse, MOUSE_RIGHT))
	{
		/*if (cur->prev != NULL)
		{
			dat->wall = *((t_wall *)list_findlast(ed->world.wall_list));
			list_remove(&ed->world.wall_list, &dat->wall, sizeof(t_wall));
		}
		else
		{
			cur->line.start = vector2_zero();
			cur->prev = NULL;
			cur->next = NULL;
		}*/
	}
	applywallmesh(cur);
	if (dat->room.wallcount != 0)
	{
		/*if (cur->prev != NULL)
		{
			float angle = vector2_anglebetween(cur->prev->line.end, cur->line.end);
			angle += vector2_anglebetween(cur->prev->line.end, cur->prev->line.start);
			char	*str;
			str = ft_itoa(radtodeg(angle));
			t_point p =
				vector3_to_screenspace(ed->render, (t_vector3){cur->line.start.x, cur->line.start.y, 0.0f}, sdl);
			draw_text_boxed(&sdl, str, p, sdl.screensize);
			free(str);
		}*/
		//ed->render.wireframe = true;
		
		//ed->render.wireframe = false;
		//render_connect(ed, sdl, *dat);
	}
	render_entity(sdl, ed->render, &cur->entity);
	renderroom(ed, sdl, dat->room);
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

	highlight_room(ed, sdl, dat->room);
	if (dat->room.floorcount == 0)
	{
		makefloor_room(ed, &sdl, &dat->room);
		list_push(&ed->world.roomlist, &dat->room, sizeof(t_room));
	}
	if (mouse_clicked(ed->mouse, MOUSE_RIGHT))
	{
		dat->wall.line.start = vector2_zero();
		dat->wall.line.end = vector2_zero();
		//applywallmesh(&dat->wall);
		dat->rtm = rtm_create;
	}
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
		dat->wall.entity.obj = NULL;
		dat->wall.entity.transform.scale = vector3_one();
		dat->wall.entity.transform.location = vector3_zero();
		dat->wall.height = 100.0f;
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
