/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/18 19:53:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"
#include "inputhelp.h"

static void highlight_object(t_editor *ed, t_sdlcontext sdl, t_entity *entity, uint32_t color)
{
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	render_entity(sdl, ed->render, entity);
	ed->render.wireframe = false;
}

static void highlight_room(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color)
{
	int	i;

	i = 0;
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	while (i < room.wallcount)
	{
		render_entity(sdl, ed->render, &room.walls[i].entity);
		i++;
	}
	i = 0;
	while (i < room.floorcount)
	{
		render_entity(sdl, ed->render, &room.floors[i].entity);
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

static void ptr_removelast(void *ptr, uint32_t *len)
{
	/*free(ptr + (*len - 1));
	*len = *len - 1;*/
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

t_meshtri	*selectedfloor(t_editor *ed, t_sdlcontext sdl, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->floorcount)
	{
		if (object_lookedat(ed, sdl, room->floors[i].entity.obj))
			return (&room->floors[i]);
		i++;
	}
	return (NULL);
}

t_wall	*selectedwall(t_editor *ed, t_sdlcontext sdl, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if (object_lookedat(ed, sdl, room->walls[i].entity.obj))
			return (&room->walls[i]);
		i++;
	}
	return (NULL);
}

static t_room *selectedroom(t_editor *ed, t_sdlcontext sdl)
{
	t_list	*l;
	t_room	*r;

	l = ed->world.roomlist;
	while (l != NULL)
	{
		r = (t_room *)l->content;
		if (selectedwall(ed, sdl, r) != NULL)
			return (r);
		if (selectedfloor(ed, sdl, r) != NULL)
			return (r);
		l = l->next;
	}
	return (NULL);
}

static bool illegalwall(t_wall *wall, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if (linelineintersect(wall->line, room->walls[i].line))
			return (true);
		i++;
	}
	return (false);
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
	if (dat->room->wallcount == 0)
		draw_text_boxed(&sdl, "place room starting point", (t_point) {20, 200}, sdl.screensize);
	cur->line.end = snap;
	if (dat->room->wallcount == 0)
		cur->line.start = snap;
	if (cur->entity.obj == NULL)
		cur->entity.obj = object_plane(&sdl);
	if ((ed->keystate >> KEYS_SHIFTMASK) & 1 && dat->room->wallcount > 0)
		cur->line.end = dat->room->walls[0].line.start;
	if (mouse_clicked(ed->mouse, MOUSE_LEFT) && !illegalwall(cur, dat->room))
	{
		ptr_add((void **)(&dat->room->walls), &dat->room->wallcount, sizeof(t_wall), cur);
		init_roomwalls(dat->room, &sdl);
		printf("wallcount in room %i \n", dat->room->wallcount);
		cur->line.start = snap;
		if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		{
			dat->rtm = rtm_modify;
			snprintf(dat->room->name, 32, "room%i", ft_listlen(ed->world.roomlist));
			printf("room name: %s \n", dat->room->name);
			return ;
		}
	}
	else if (mouse_clicked(ed->mouse, MOUSE_RIGHT) && dat->room->wallcount >= 1)
	{
		printf("wall count was %i \n", dat->room->wallcount);
		dat->wall.line.start = dat->room->walls[dat->room->wallcount - 1].line.start;
		dat->room->wallcount--;
	}
	applywallmesh(cur);
	draw_text_boxed(&sdl, vector_string(rc), (t_point){20, sdl.window_h - 40}, sdl.screensize);
	render_entity(sdl, ed->render, &cur->entity);
	if (illegalwall(cur, dat->room))
	{
		ed->render.wireframe = true;
		ed->render.gizmocolor = CLR_RED;
		render_entity(sdl, ed->render, &cur->entity);
		ed->render.wireframe = false;
	}
	renderroom(ed, sdl, *dat->room);
	render_snapgrid(ed, &sdl, snap, false, false);
}

static void modifywallheights(t_room *room, int scrolldelta)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		room->walls[i].height += scrolldelta * 10;
		room->walls[i].height = ft_clamp(room->walls[i].height, 40, 300);
		applywallmesh(&room->walls[i]);
		i++;
	}
}

void	applydrag(t_vector2 snap, t_room *room)
{
	int			i;
	t_wall	*prev;

	i = 0;
	while (i < room->wallcount)
	{
		if (vector2_sqr_magnitude(vector2_sub(snap, room->walls[i].line.start)) < 1000)
		{
			if (i == 0)
				prev = &room->walls[room->wallcount - 1];
			else
				prev = &room->walls[i - 1];
			prev->line.end = snap;
			room->walls[i].line.start = snap;
			applywallmesh(prev);
			applywallmesh(&room->walls[i]);
		}
		i++;
	}
}

t_wall	*dragroomcorner(t_vector2 snap, t_room *room)
{
	int			i;

	i = 0;
	while (i < room->wallcount)
	{
		if (vector2_sqr_magnitude(vector2_sub(snap, room->walls[i].line.start)) < 1000)
		{
			return (&room->walls[i]);
			/*room->walls[i - 1].line.end = snap;
			room->walls[i].line.start = snap;
			applywallmesh(&room->walls[i - 1]);
			applywallmesh(&room->walls[i]);*/
		}
		i++;
	}
	return (NULL);
}

/*TODO: write floor function that frees previous floor and generates new
write door create function (click wall and select door width / position)
*/

void	modifymode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	t_vector3		rc;
	t_wall			*wall;
	t_vector2		snap;
	char			text[64] = { };

	rc = raycast(ed);
	snprintf(text, 64, "modifying room '%s'", dat->room->name);
	draw_text_boxed(&sdl, text, (t_point){sdl.window_w / 2, 40}, sdl.screensize);
	snap = vector2_snap((t_vector2){rc.x, rc.y}, 10);
	render_snapgrid(ed, &sdl, snap, false, false);
	highlight_room(ed, sdl, *dat->room, CLR_GREEN);
	wall = dragroomcorner(snap, dat->room);
	if (wall != NULL)
	{
		if (ed->mouse.held == MOUSE_LEFT)
			applydrag(snap, dat->room);
		highlight_object(ed, sdl, &wall->entity, CLR_PRPL);
	}
	if (dat->room->floorcount == 0)
	{
		makefloor_room(ed, &sdl, dat->room);
		list_push(&ed->world.roomlist, dat->room, sizeof(t_room));
		free(dat->room);
		dat->room = list_findlast(ed->world.roomlist);
	}
	if (mouse_clicked(ed->mouse, MOUSE_RIGHT))
	{
		dat->wall.line.start = vector2_zero();
		dat->wall.line.end = vector2_zero();
		dat->room = NULL;
		dat->rtm = rtm_create;
	}
	if (ed->mouse.scroll_delta != 0)
		modifywallheights(dat->room, ed->mouse.scroll_delta);
	if (instantbutton((t_rectangle){20, 300, 80, 40}, &ed->mouse, sdl, "minus.png"))
	{
		list_remove(&ed->world.roomlist, dat->room, sizeof(t_room));
		dat->room = NULL;
	}
}

void	room_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_roomtooldata	*dat;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	if (dat->room == NULL)
	{
		drawcircle(sdl, point_div(sdl.screensize, 2), 2, CLR_BLUE);
		t_room	*hover;
		hover = selectedroom(ed, sdl);
		if (hover != NULL)
		{
			highlight_room(ed, sdl, *hover, CLR_BLUE);
			if (mouse_clicked(ed->mouse, MOUSE_LEFT))
			{
				dat->room = hover;
				dat->rtm = rtm_modify;
			}
		}
	} else if (dat->rtm == rtm_create)
		createmode(ed, sdl, dat);
	if (instantbutton((t_rectangle){20, 200, 80, 40}, &ed->mouse, sdl, "plus.png"))
	{
		dat->room = ft_memalloc(sizeof(t_room));
		dat->rtm = rtm_create;
	}
	if (dat->rtm == rtm_modify && dat->room != NULL)
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
