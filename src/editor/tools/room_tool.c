/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/07 09:49:06 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"
 
static void	init_roomwalls_shallow(t_world *world, t_room *room)
{
	int			i;
	t_entity	*ent;

	i = 0;
	while (i < room->wallcount)
	{
		//room->walls[i].entity = raise_entity(world); //Copy saved entitys important values
		room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		room->walls[i].entity->obj = object_plane(world->sdl);
		applywallmesh(&room->walls[i]);
		i++;
	}
}


static void highlight_object(t_editor *ed, t_sdlcontext sdl, t_entity *entity, uint32_t color)
{
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	render_entity(sdl, &ed->render, entity);
	ed->render.wireframe = false;
}

void remove_room(t_world *world, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		free_object(room->walls[i].entity->obj);
		erase_entity(world, room->walls[i].entity);
		i++;
	}
	list_remove(&world->roomlist, room, sizeof(t_room));
}

void add_room_to_world(t_world *world, t_room *room)
{
	int		i;
	t_room	*worldroom;
	t_wall	*w_wall;
	t_wall	*wall;

	worldroom = ft_memalloc(sizeof(t_room));
	worldroom->wallcount = room->wallcount;
	worldroom->walls = ft_memalloc(sizeof(t_wall) * worldroom->wallcount);
	//TODO: memcpy from room to worldroom and overwrite?
	i = 0;
	while (i < room->wallcount)
	{
		w_wall = &worldroom->walls[i];
		wall = &room->walls[i];
		ft_memcpy(w_wall, wall, sizeof(t_wall));
		w_wall->entity = raise_entity(world);
		w_wall->entity->uneditable = true;
		free(wall->entity);
		i++;
	}
	ft_strcpy(worldroom->name, room->name);
	list_push(&world->roomlist, worldroom, sizeof(t_room));
	free(worldroom);
	worldroom = list_findlast(world->roomlist);
	init_roomwalls(world, worldroom);
	free(room->walls);
	free(room);
}

static void highlight_room(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color)
{
	int	i;

	i = 0;
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	while (i < room.wallcount)
	{
		//printf("HIGHLIGHTING ROOM \n");
		render_entity(sdl, &ed->render, room.walls[i].entity);
		i++;
	}
	/*i = 0;
	while (i < room.floorcount)
	{
		render_entity(sdl, &ed->render, room.floors[i].entity);
		i++;
	}*/
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

void ptr_add(void **ptr, uint32_t *len, size_t size, void *add)
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
	//printf("newptr loc %lu catting mem to %lu \n", newptr, newptr + ((*len) * size));
	ft_memcpy(newptr + ((*len) * size), add, size);
	*len = *len + 1;
	*ptr = newptr;
}

static void ptr_removelast(void *ptr, uint32_t *len)
{
	/*free(ptr + (*len - 1));
	*len = *len - 1;*/
}

static void renderroom(t_editor *ed, t_sdlcontext *sdl, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		/*ed->render.wireframe = true;
		ed->render.gizmocolor = CLR_GREEN;*/
		render_entity(*sdl, &ed->render, room->walls[i].entity);
		//ed->render.wireframe = false;
		i++;
	}
}

t_meshtri	*selectedfloor(t_editor *ed, t_sdlcontext sdl, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->floorcount)
	{
		if (entity_lookedat(ed, sdl, room->floors[i].entity))
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
		if (entity_lookedat(ed, sdl, room->walls[i].entity))
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
		/*if (selectedfloor(ed, sdl, r) != NULL)
			return (r);*/
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
		if (vector2_cmp(wall->line.end, room->walls[i].line.end))
			return (true);
		i++;
	}
	return (false);
}

static bool illegalwall_move(t_wall *wall, t_room *room)
{
	int	i;

	i = 0;
	if (vector2_cmp(wall->line.start, wall->line.end))
		return (true);
	while (i < room->wallcount)
	{
		if (linelineintersect(wall->line, room->walls[i].line))
			return (true);
		i++;
	}
	return (false);
}
 //TODO: call default_wall_occlusion_settings(&room->walls[i], world);

static void	createmode(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_wall			*cur;
	t_vector3		rc;
	t_vector2		snap;
	t_autogui		*gui;

	cur = &dat->wall;
	rc = raycast(ed);
	snap = vector2_snap((t_vector2){rc.x, rc.y}, 10);
	gui = &dat->newroom_gui;
	gui_start(gui);
	if (dat->room->wallcount == 0)
		gui_label("Place first wall", gui);
	else if (dat->room->wallcount > 1)
		gui_label("Shift + click to finish room", gui);
	else
		gui_emptyvertical(20, gui);
	gui_end(gui);
	cur->line.end = snap;
	if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1 && dat->room->wallcount > 0)
		cur->line.end = dat->room->walls[0].line.start;
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT) && !illegalwall(cur, dat->room))
	{
		if (vector2_cmp(cur->line.start, vector2_zero()))
		{
			cur->line.start = snap;
			return ;
		}
		//ptr_add((void **)(&dat->room->walls), &dat->room->wallcount, sizeof(t_wall), cur);
		dat->room->walls[dat->room->wallcount] = *cur;
		dat->room->walls[dat->room->wallcount].entity = ft_memalloc(sizeof(t_entity));
		ft_memcpy(dat->room->walls[dat->room->wallcount].entity, cur->entity, sizeof(t_entity));
		dat->room->wallcount++;
		init_roomwalls_shallow(&ed->world, dat->room);
		cur->line.start = snap;
		if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		{
			dat->rtm = rtm_modify;
			add_room_to_world(&ed->world, dat->room);
			dat->room = NULL;
			//printf room null
			return ;
		}
	}
	else if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		if (dat->room->wallcount >= 1)
		{
			dat->wall.line.start = dat->room->walls[dat->room->wallcount - 1].line.start;
			dat->room->wallcount--;
		}
		else
		{
			free(dat->room);
			dat->room = NULL;
			ft_bzero(&dat->wall.line, sizeof(t_line));
			return ;
		}
	}
	applywallmesh(cur);
	//print_text_boxed(&sdl, vector_string(rc), (t_point){20, sdl.window_h - 80}, sdl.screensize);
	if (!vector2_cmp(cur->line.start, vector2_zero()))
		render_entity(*sdl, &ed->render, cur->entity);
	if (illegalwall(cur, dat->room))
	{
		/*ed->render.wireframe = true;
		ed->render.gizmocolor = CLR_RED;*/
		render_entity(*sdl, &ed->render, cur->entity);
		//ed->render.wireframe = false;
	}
	renderroom(ed, sdl, dat->room);
	render_snapgrid(ed, sdl, snap, false, false);
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

void	applydrag(t_vector2 snap, t_room *room, t_world *world)
{
	int			i;
	t_wall	*prev;
	t_wall	test[2];

	i = 0;
	while (i < room->wallcount)
	{
		if (vector2_sqr_magnitude(vector2_sub(snap, room->walls[i].line.start)) < 1000)
		{
			if (i == 0)
				prev = &room->walls[room->wallcount - 1];
			else
				prev = &room->walls[i - 1];
			test[0] = *prev;
			test[0].line.end = snap;
			test[1] = room->walls[i];
			test[1].line.start = snap;
			if (!illegalwall_move(&test[0], room) || !illegalwall_move(&test[1], room))
			{
				prev->line.end = snap;
				room->walls[i].line.start = snap;
				applywallmesh(prev);
				applywallmesh(&room->walls[i]);
				free_floor(world, room);
			}
			break ;
		}
		i++;
	}
}

bool	looking_atcorner(t_editor *ed, t_sdlcontext sdl, t_vector2 snap, t_room *room)
{
	int			i;

	i = 0;
	while (i < room->wallcount)
	{
		if (vector2_sqr_magnitude(vector2_sub(snap, room->walls[i].line.start)) < 1000)
		{
			ed->render.gizmocolor = CLR_BLUE;
			render_gizmo(sdl, ed->render, vector2_to_vector3(room->walls[i].line.start), 10);
			render_gizmo(sdl, ed->render, vector2_to_vector3(room->walls[i].line.start), 4);
			return (true);
		}
		i++;
	}
	return (false);
}

static void insertwall(t_wall current, t_room *room, t_wall new[3])
{
	int	i;
	t_wall	*newptr;

	i = 0;
	newptr = ft_memalloc((room->wallcount + 2) * sizeof(t_wall));
	while (i < room->wallcount)
	{
		if (ft_memcmp(&current.line, &room->walls[i].line, sizeof(t_line)) == 0)
		{
			ft_memcpy(newptr, room->walls, sizeof(t_wall) * i);
			printf("copied %i walls before doorwalls \n", i);
			ft_memcpy(newptr + i, new, sizeof(t_wall) * 3);
			ft_memcpy(newptr + i + 3, room->walls + i + 1, sizeof(t_wall) * (room->wallcount - i - 1));
			printf("copied %i walls after doorwalls \n", (room->wallcount - i - 1));
			free(room->walls);
			room->walls = newptr;
			room->wallcount = room->wallcount + 2;
			return ;
		}
		i++;
	}
	printf("no match in insertwall \n");
	free(newptr);
}

static void walleditmode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	char	text[64] = { };

	//snprintf(text, 64, "modifying selected wall");
	//print_text_boxed(&sdl, text, (t_point){sdl.window_w / 2, 80});
	//render_entity(sdl, &ed->render, &dat->doorwalls[0]);
	highlight_object(ed, sdl, dat->ed_wall->entity, CLR_GREEN);
	/*snprintf(text, 64, "make door");
	print_text_boxed(&sdl, text, (t_point){20, 280}, sdl.screensize);*/
	if (instantbutton((t_rectangle){20, 300, 40, 40}, &ed->hid.mouse, sdl, "stop.png"))
	{
		//dat->ed_wall->entity.transform.scale = vector3_zero();
		dat->doorwalls[0].line.start = dat->ed_wall->line.start;
		dat->doorwalls[0].line.end = vector2_lerp(dat->ed_wall->line.start, dat->ed_wall->line.end, 0.33f);
		dat->doorwalls[0].height = dat->ed_wall->height;
		dat->doorwalls[2].line.start = vector2_lerp(dat->ed_wall->line.end, dat->ed_wall->line.start, 0.33f);
		dat->doorwalls[2].line.end = dat->ed_wall->line.end;
		dat->doorwalls[2].height = dat->ed_wall->height;
		dat->doorwalls[1].line.start = dat->doorwalls[0].line.end;
		dat->doorwalls[1].line.end = dat->doorwalls[2].line.start;
		dat->doorwalls[1].height = dat->ed_wall->height;
		applywallmesh(&dat->doorwalls[0]);
		applywallmesh(&dat->doorwalls[1]);
		applywallmesh(&dat->doorwalls[2]);
		insertwall(*dat->ed_wall, dat->room, dat->doorwalls);
		//init_roomwalls(dat->room, &sdl);
		dat->ed_wall = NULL;
		return ;
	}
	if (instantbutton((t_rectangle){20, 345, 40, 40}, &ed->hid.mouse, sdl, "one.png"))
	{
		dat->ed_wall->disabled = !dat->ed_wall->disabled;
		//init_roomwalls(dat->room, &sdl);
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->ed_wall = NULL;
}

void	modifymode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	t_vector3		rc;
	t_wall			*look_wall;
	t_vector2		snap;
	t_autogui		*gui;
	char			text[64] = { };

	gui = &dat->modroom_gui;
	if (dat->ed_wall != NULL)
	{
		walleditmode(ed, sdl, dat);
		return ;
	}
	rc = raycast(ed);
	gui_start(gui);
	gui_starthorizontal(gui);
	gui_label("Modifying: ", gui);
	gui_label(dat->room->name, gui);
	gui_endhorizontal(gui);
	if (gui_button("Delete", gui))
	{
		remove_room(&ed->world, dat->room);
		//list_remove(&ed->world.roomlist, dat->room, sizeof(t_room));
		dat->wall.line.start = vector2_zero();
		dat->wall.line.end = vector2_zero();
		dat->room = NULL;
		gui_end(gui);
		return ;
	}
	gui_end(gui);
	//print_text_boxed(&sdl, text, (t_point){sdl.window_w / 2, 40}, sdl.screensize);
	snap = vector2_snap((t_vector2){rc.x, rc.y}, 10);
	highlight_room(ed, sdl, *dat->room, CLR_GREEN);
	look_wall = selectedwall(ed, sdl, dat->room);
	if (looking_atcorner(ed, sdl, snap, dat->room) && ed->hid.mouse.held == MOUSE_LEFT)
		applydrag(snap, dat->room, &ed->world);
	else if (dat->room->floorcount == 0)
		makefloor_room(ed, &sdl, dat->room);
	if (!looking_atcorner(ed, sdl, snap, dat->room) && mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		force_mouseunlock(&ed->hid);
		dat->ed_wall = look_wall;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		dat->wall.line.start = vector2_zero();
		dat->wall.line.end = vector2_zero();
		dat->room = NULL;
		dat->rtm = rtm_create;
	}
	if (ed->hid.mouse.scroll_delta != 0)
		modifywallheights(dat->room, ed->hid.mouse.scroll_delta);
	/*if (instant_text_button(sdl, &ed->hid.mouse, "remove room", (t_point){20, 300}))
	{
		list_remove(&ed->world.roomlist, dat->room, sizeof(t_room));
		dat->wall.line.start = vector2_zero();
		dat->wall.line.end = vector2_zero();
		dat->room = NULL;
	}*/
}

void	room_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_roomtooldata	*dat;

	dat = ed->tool->tooldata;
	if (dat->wall.entity->obj == NULL)
		dat->wall.entity->obj = object_plane(sdl);
	if (dat->maingui.sdl == NULL)
	{
		dat->maingui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, 80}, "Room tool");
		gui_autosize(&dat->maingui);
		dat->maingui.minimum_size.x = 140;
	}
	if (dat->newroom_gui.sdl == NULL)
	{
		dat->newroom_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, 80}, "New room");
		dat->newroom_gui.dock = &dat->maingui;
		gui_autosize(&dat->newroom_gui);
	}
	if (dat->modroom_gui.sdl == NULL)
	{
		dat->modroom_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, sdl->screensize.y / 2}, "Edit room");
		dat->modroom_gui.minimum_size.x = 130;
		dat->modroom_gui.rect.size.x = 130;
	}
}

void	update_maingui(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui	*gui;
	char		roomname[64];

	gui = &dat->maingui;
	gui_start(gui);
	if (gui_button("Add room", gui))
	{
		dat->room = ft_memalloc(sizeof(t_room));
		ft_bzero(roomname, 64);
		snprintf(roomname, 64, "room(%i)", ft_listlen(ed->world.roomlist));
		ft_strcpy(dat->room->name, roomname);
		dat->room->walls = ft_memalloc(sizeof(t_wall) * 32);
		dat->rtm = rtm_create;
		force_mouselock(&ed->hid);
	}
	gui_end(gui);
}

void	room_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_roomtooldata	*dat;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	if (dat->room == NULL)
	{
		t_room	*hover;
		hover = selectedroom(ed, *sdl);
		if (hover != NULL)
		{
			highlight_room(ed, *sdl, *hover, CLR_BLUE);
			if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
			{
				dat->room = hover;
				dat->rtm = rtm_modify;
				ed->hid.mouse.click_unhandled = false;
			}
		}
	}
	else if (dat->rtm == rtm_create)
	{
		createmode(ed, sdl, dat);
	}
	update_maingui(ed, sdl, dat);
	if (dat->rtm == rtm_modify && dat->room != NULL)
		modifymode(ed, *sdl, dat);
}



t_tool	*get_room_tool()
{
	static t_tool	tool
	= {
		.update = room_tool_update,
		.init	= room_tool_init
	};
	t_roomtooldata		*dat;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_roomtooldata));
		dat = (t_roomtooldata *)tool.tooldata;
		//ft_bzero(&dat->wall, sizeof(t_wall));
		dat->wall.entity = ft_memalloc(sizeof(t_entity));
		dat->wall.entity->obj = NULL;
		dat->wall.entity->transform.scale = vector3_one();
		dat->wall.entity->transform.position = vector3_zero();
		dat->wall.height = 100.0f;
		/*dat->doorwalls[0].entity.transform.scale = vector3_one();
		dat->doorwalls[0].entity.transform.position = vector3_zero();
		dat->doorwalls[1].entity.transform.scale = vector3_one();
		dat->doorwalls[1].entity.transform.position = vector3_zero();
		dat->doorwalls[2].entity.transform.scale = vector3_one();
		dat->doorwalls[2].entity.transform.position = vector3_zero();*/
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
