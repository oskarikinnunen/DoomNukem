/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/21 18:50:19 by vlaine           ###   ########.fr       */
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

static void	init_splitwalls(t_world *world, t_roomtooldata *dat)
{
	int			i;
	t_wall		*w;

	i = 0;
	while (i < dat->splitwallcount)
	{
		w = &dat->splitwalls[i];
		//room->walls[i].entity = raise_entity(world); //Copy saved entitys important values
		w->entity->transform.position = vector3_zero();
		w->entity->transform.scale = vector3_one();
		w->entity->obj = object_plane(world->sdl);
		printf("set splitwal obj %i \n", i);
		applywallmesh(w);
		i++;
	}
}


static void highlight_object(t_editor *ed, t_sdlcontext sdl, t_entity *entity, uint32_t color)
{
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	render_entity(&sdl, &ed->render, entity);
	ed->render.wireframe = false;
}

void remove_room(t_world *world, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		free_object(room->walls[i].entity->obj);
		destroy_entity(world, room->walls[i].entity);
		i++;
	}
	free_floor(world, room);
	list_remove(&world->roomlist, room, sizeof(t_room));
}

void	split_area(t_room *room, t_floor_area *old, t_floor_area *area, t_editor *ed, t_sdlcontext *sdl)
{
	t_wall	*w;
	t_wall	*sw;
	t_wall	*ew;
	int		start;
	int		i;

	printf("room total walls: %i \n", room->wallcount);
	printf("unique walls in area %i \n", area->unique_wallcount);
	area->wallcount = 0;
	while (area->wallcount < area->unique_wallcount)
	{
		area->wall_indices[area->wallcount] = (room->wallcount - area->unique_wallcount) + (area->wallcount);
		area->wallcount++;
	}
	printf("area wall count now: %i \n", area->unique_wallcount);
	sw = &room->walls[area->wall_indices[0]];
	ew = &room->walls[area->wall_indices[area->wallcount - 1]];
	printf("assigned sw as wall %i and ew as wall %i\n", area->wall_indices[0], area->wall_indices[area->wallcount - 1]);
	i = 0;
	w = &room->walls[old->wall_indices[i]];
	while (!vector2_cmp(w->line.start, ew->line.end))
	{
		if (i >= old->wallcount)
			i = 0;
		w = &room->walls[old->wall_indices[i]];
		printf("searching for ew->end\n");
		i++;
	}
	printf("start index is %i \n", i - 1);
	i -= 1;
	start = i;
	while (!vector2_cmp(w->line.end, sw->line.start))
	{
		if (i >= old->wallcount)
			i = 0;
		w = &room->walls[old->wall_indices[i]];
		area->wall_indices[area->wallcount] = old->wall_indices[i];
		area->wallcount++;
		printf("searching for sw->start\n");
		i++;
	}
	i = start;
	t_floor_area temp = *old;
	old->wallcount = 0;
	w = &room->walls[temp.wall_indices[i]];
	while (!vector2_cmp(w->line.start, sw->line.start))
	{
		if (i < 0)
			i = temp.wallcount - 1;
		w = &room->walls[temp.wall_indices[i]];
		old->wall_indices[old->wallcount] = temp.wall_indices[i];
		old->wallcount++;
		i--;
	}
	i = 0;
	while (i < area->wallcount)
	{
		printf("wall [%i] is %i\n", i, area->wall_indices[i]);
		i++;
	}
	area->height += room->walls[area->wall_indices[0]].height;
	room->floor_areas[room->floor_areacount] = *area;
	room->floor_areacount++;
	free_floor(&ed->world, room);
	
	/*if (area->wall_indices[area->unique_wallcount] > area->wall_indices[area->wallcount - 1])
	{
		printf("FIRST > LAST (gt)\n");
	} else {
		printf("FIRST < LAST (lt)\n");
	}*/
}

void	assign_default_floor_area(t_room *room)
{
	int	i;
	//int	w_i;

	room->floor_areas = ft_memalloc(sizeof(t_floor_area) * 10);
	room->floor_areacount = 1;

	i = 0;
	while (i < room->wallcount)
	{
		room->floor_areas[0].wall_indices[i] = i;
		i++;
	}
	room->floor_areas[0].wallcount = room->wallcount;
	room->floor_areas[0].unique_wallcount = room->wallcount;
	//TODO: make a custom assign_floor area that takes the input from "floorarea_editmode" or whatevs, 
}

void add_room_to_world(t_world *world, t_room *room)
{
	int		i;
	t_room	*worldroom;
	t_wall	*w_wall;
	t_wall	*wall;

	worldroom = ft_memalloc(sizeof(t_room));
	worldroom->wallcount = room->wallcount;
	worldroom->walls = ft_memalloc(sizeof(t_wall) * worldroom->wallcount * 2);
	//TODO: memcpy from room to worldroom and overwrite?
	i = 0;
	while (i < room->wallcount)
	{
		w_wall = &worldroom->walls[i];
		wall = &room->walls[i];
		ft_memcpy(w_wall, wall, sizeof(t_wall));
		w_wall->entity = spawn_entity(world);
		w_wall->entity->uneditable = true;
		free(wall->entity);
		i++;
	}
	ft_strcpy(worldroom->name, room->name);
	list_push(&world->roomlist, worldroom, sizeof(t_room));
	free(worldroom);
	worldroom = list_findlast(world->roomlist);
	assign_default_floor_area(worldroom);
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
		render_entity(&sdl, &ed->render, room.walls[i].entity);
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
		render_entity(sdl, &ed->render, room->walls[i].entity);
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
		if (ft_memcmp(&wall->line, &room->walls[i].line, sizeof(t_line)) &&
			linelineintersect(line_shorten(wall->line), line_shorten(room->walls[i].line)))
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
		render_entity(sdl, &ed->render, cur->entity);
	if (illegalwall(cur, dat->room))
	{
		/*ed->render.wireframe = true;
		ed->render.gizmocolor = CLR_RED;*/
		render_entity(sdl, &ed->render, cur->entity);
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
	free_floor(world, room);
	while (i < room->wallcount)
	{
		if (vector2_sqr_magnitude(vector2_sub(snap, room->walls[i].line.start)) < 1000)
		{
			/*if (i == 0)
				prev = &room->walls[room->wallcount - 1];
			else
				prev = &room->walls[i - 1];
			test[0] = *prev;
			test[0].line.end = snap;
			test[1] = room->walls[i];
			test[1].line.start = snap;
			if (!illegalwall_move(&test[0], room) && !illegalwall_move(&test[1], room))
			{
				prev->line.end = snap;
				room->walls[i].line.start = snap;
				applywallmesh(prev);
				applywallmesh(&room->walls[i]);
				free_floor(world, room);
				assign_default_floor_area(room);
			}
			break ;*/
			room->walls[i].line.start = snap;
			applywallmesh(&room->walls[i]);
			
		}
		else if (vector2_sqr_magnitude(vector2_sub(snap, room->walls[i].line.end)) < 1000)
		{
			room->walls[i].line.end = snap;
			applywallmesh(&room->walls[i]);
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

void	highlight_room_corners(t_editor *ed, t_sdlcontext *sdl, t_room *room)
{
	int	i;

	i = 0;
	ed->render.gizmocolor = AMBER_0;
	while (i < room->wallcount)
	{
		render_gizmo(*sdl, ed->render, vector2_to_vector3(room->walls[i].line.start), 5);
		i++;
	}
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
	t_autogui	*gui;

	gui = &dat->modroom_gui;

	//snprintf(text, 64, "modifying selected wall");
	//print_text_boxed(&sdl, text, (t_point){sdl.window_w / 2, 80});
	//render_entity(sdl, &ed->render, &dat->doorwalls[0]);
	highlight_object(ed, sdl, dat->ed_wall->entity, CLR_GREEN);
	
	gui_start(gui);
	if (gui_button("Triple", gui))
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
	gui_labeled_bool_edit("Hide wall", &dat->ed_wall->entity->hidden, gui);
	gui_end(gui);
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

	int i = 0;
	char *ind_str;
	while (i < dat->room->wallcount)
	{
		t_vector3	ws;
		t_vector2	start;

		start = dat->room->walls[i].line.start;
		ws = (t_vector3){start.x, start.y, 0.0f};
		t_point ss = vector3_to_screenspace(ed->render, ws, sdl);
		ind_str = ft_itoa(i);
		print_text_boxed(&sdl, ind_str, point_add(ss, (t_point){20, 20}));
		free (ind_str);
		i++;
	}
	if (gui_shortcut_button("Delete", KEYS_DELETEMASK, gui))
	{
		remove_room(&ed->world, dat->room);
		//list_remove(&ed->world.roomlist, dat->room, sizeof(t_room));
		dat->wall.line.start = vector2_zero();
		dat->wall.line.end = vector2_zero();
		dat->room = NULL;
		gui_end(gui);
		return ;
	}
	if (gui_shortcut_button("Paint", 'P', gui))
	{
		dat->rtm = rtm_paint;
		return ;
	}
	/*if (gui_shortcut_button("Split floor", 'Z', gui))
	{
		dat->rtm = rtm_split;
		dat->wall.line.start = vector2_zero();
		dat->splitwallcount = 0;
		dat->area = ft_memalloc(sizeof(t_floor_area));
		return ;
	}*/
	//int	prev = dat->floor_debugvalue;
	//gui_labeled_int_slider("FLOORDEBUG:", &dat->floor_debugvalue, 1, gui);
	//dat->floor_debugvalue = ft_clamp(dat->floor_debugvalue, 2, 5);
	gui_end(gui);
	snap = vector2_snap((t_vector2){rc.x, rc.y}, 10);
	highlight_room(ed, sdl, *dat->room, AMBER_2);
	look_wall = selectedwall(ed, sdl, dat->room);
	if (looking_atcorner(ed, sdl, snap, dat->room) && ed->hid.mouse.held == MOUSE_LEFT)
		applydrag(snap, dat->room, &ed->world);
	else if (dat->room->floorcount == 0)
	{
		make_areas(ed, &sdl, dat->room);
		//makefloor_room(ed, &sdl, dat->room, 2);
	}
		
	if (!looking_atcorner(ed, sdl, snap, dat->room) && mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		//force_mouseunlock(&ed->hid);
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
	t_vector3	look;

	look = raycast(ed);
	t_point	p;

	p.x = look.x;
	p.y = look.y;

	gui = &dat->maingui;
	gui_start(gui);
	//gui_labeled_point("DEBUG:", p, gui);
	if (gui_shortcut_button("New room", 'N', gui))
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

void	draw_t_line(t_sdlcontext *sdl, t_line line, uint32_t color)
{
	t_point p1;
	t_point p2;

	p1 = vector2_to_point(line.start);
	p2 = vector2_to_point(line.end);
	drawline(*sdl, p1, p2, color);
}

t_vector2	closest_corner(t_vector2	v2, t_room *room)
{
	int	i;
	t_vector2	result;
	float		dist;

	i = 0;
	//result = v2
	result = room->walls[0].line.start;
	dist = 100000.0f;
	while (i < room->wallcount)
	{
		if (dist > vector2_sqr_dist(v2, room->walls[i].line.start))
		{
			dist = vector2_sqr_dist(v2, room->walls[i].line.start);
			result = room->walls[i].line.start;
		}
			
		i++;
	}
	return (result);
}

void	room_tool_paint(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui	*gui;
	static	t_img	*tex;
	t_wall		*w;
	static		int	img_index;
	static		int	prev_img_index;
	static		uint32_t	prev_changetime;
	int			i;

	gui = &dat->modroom_gui;
	int	delt;
	delt = 0;
	if (ed->hid.mouse.scroll_delta > 0)
		delt = 1;
	else if (ed->hid.mouse.scroll_delta < 0)
		delt = -1;
	img_index += delt;
	img_index = ft_clamp(img_index, 0, sdl->imagecount - 1);
	tex = &sdl->images[img_index];
	
	if (img_index != prev_img_index)
	{
		prev_changetime = ed->world.clock.prev_time;
		printf("CHANGED \n");
	}
	
	highlight_room(ed, *sdl, *dat->room, AMBER_2);

	t_point middle = point_div(sdl->screensize, 2);
	middle = point_sub(middle, (t_point){16,16});
	middle = point_add(middle, (t_point){0,32});
	
	t_rectangle mrect;
	mrect.position = middle;
	mrect.size = (t_point){32,32};
	//draw_image(*sdl, point_div(sdl->screensize, 2), *tex, (t_point){64,64});
	

	int from = 0;
	int	to = 1;
	if (ed->clock.prev_time < prev_changetime + 1000)
	{
		i = -5;
		while (i < 6)
		{
			t_point cur;

			cur = point_add(middle, point_mul((t_point){32, 0}, i));
			cur.y = middle.y + (cos(i / 5.0f) * 164);
			if (img_index + i >= 0 && img_index + i < sdl->imagecount)
			{
				draw_image(*sdl, cur, sdl->images[img_index + i], (t_point){32,32});
			}
			i++;
		}
	}
	i = from;
	while (i < to)
	{
		t_point cur;

		cur = point_add(middle, point_mul((t_point){32, 0}, i));
		cur.y = middle.y + (cos(i / 5.0f) * 164);
		t_rectangle rect;
		rect.position = cur;
		rect.size = (t_point){32,32};
		if (img_index + i >= 0 && img_index + i < sdl->imagecount)
		{
			draw_image(*sdl, cur, sdl->images[img_index + i], (t_point){32,32});
			if (i == 0)
				drawrectangle(*sdl, rect, AMBER_4);
		}
		i++;
	}
	//drawrectangle(*sdl, mrect, AMBER_4);
		
	
	prev_img_index = img_index;
	if (tex != NULL && ed->hid.mouse.held == MOUSE_LEFT)
	{
		i = 0;
		while (i < dat->room->wallcount)
		{
			t_wall *w;

			w = &dat->room->walls[i];
			if (entity_lookedat(ed, *sdl, w->entity))
			{
				if (w->entity->obj->materials->img != tex)
				{
					w->entity->obj->materials->img = tex;
					bake_lighting(&ed->render, &ed->world);
				}
				return ;
			}
			i++;
		}
		i = 0;
		while (i < dat->room->floorcount)
		{
			t_meshtri *f;

			f = &dat->room->floors[i];
			if (entity_lookedat(ed, *sdl, f->entity))
			{
				if (f->entity->obj->materials->img != tex)
				{
					f->entity->obj->materials->img = tex;
					bake_lighting(&ed->render, &ed->world);
				}
				return ;
			}
			i++;
		}
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->rtm = rtm_modify;
}

void	room_tool_split(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui	*gui;
	t_vector3	snap;
	t_wall		*cur;
	t_vector2	v2snap;

	snap = raycast(ed);
	v2snap = vector2_snap((t_vector2){snap.x, snap.y}, 10);
	cur = &dat->wall;
	cur->height = dat->room->walls[0].height / 2;
	gui = &dat->modroom_gui;
	
	if ((dat->splitwallcount == 0  && vector2_cmp(cur->line.start, vector2_zero()))
		|| (ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		v2snap = closest_corner(v2snap, dat->room);

	ed->render.gizmocolor = AMBER_4;
	render_gizmo(*sdl, ed->render, vector2_to_vector3(v2snap), 10);
	/*
	have local wall * for split, after confirming the split is correct, append it to the room
	
	
	*/
	cur->line.end = v2snap;
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		if (vector2_cmp(cur->line.start, vector2_zero()))
		{
			cur->line.start = v2snap;
			//Assing this new floorareas wall indice 0 to the one we selected
			//
			return ;
		}
		dat->room->walls[dat->room->wallcount] = *cur;
		dat->room->walls[dat->room->wallcount].entity = spawn_entity(&ed->world);
		//dat->room->walls[dat->room->wallcount].entity->obj = object_plane(sdl);
		dat->room->wallcount++;
		dat->area->unique_wallcount++;
		init_roomwalls(&ed->world, dat->room);
		if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		{
			//assign_floor_area(dat->room, dat->area, ed, sdl);
			split_area(dat->room, &dat->room->floor_areas[0], dat->area, ed, sdl);
			dat->rtm = rtm_modify;
			/*dat->room = NULL;
			dat->rtm = rtm_create;*/
			return ;
		}
		//init_splitwalls(&ed->world, dat);
		//cur->line.end = v2snap;
		cur->line.start = v2snap;
		return ;
	}
	applywallmesh(cur);
	if (!vector2_cmp(cur->line.start, vector2_zero()))
		render_entity(sdl, &ed->render, cur->entity);
	gui_start(gui);
	gui_starthorizontal(gui);
	gui_label("Modifying: ", gui);
	gui_label(dat->room->name, gui);
	gui_endhorizontal(gui);
	gui_label("Select a point to start a new area from", gui);

	int		i = 0;
	char	str[256];
	while (i < dat->room->floor_areacount)
	{
		ft_bzero(str, 256);
		snprintf(str, 256, "Area %i\n", i);
		gui_label(str, gui);
		i++;
	}
	gui_end(gui);
	highlight_room_corners(ed, sdl, dat->room);
	
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->rtm = rtm_modify;
}

void	room_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_roomtooldata	*dat;
	t_line			line;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	if (dat->room == NULL)
	{
		t_room	*hover;
		hover = selectedroom(ed, *sdl);
		if (hover != NULL)
		{
			highlight_room(ed, *sdl, *hover, AMBER_0);
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
	if (dat->rtm == rtm_split && dat->room != NULL)
		room_tool_split(ed, sdl, dat);

	if (dat->rtm == rtm_paint && dat->room != NULL)
		room_tool_paint(ed, sdl, dat);
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
