/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/27 15:55:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

void	recalculate_joined_rooms(t_world *world, t_room *room);

static void	init_roomwalls_shallow(t_world *world, t_room *room)
{
	int			i;
	t_entity	*ent;

	i = 0;
	while (i < room->wallcount)
	{
		//room->walls[i].entity = raise_entity(world); //Copy saved entitys important values
		//if (room->walls[i].ceilingwall)
		//	room->walls[i].entity->hidden = true;

		/*room->walls[i].edgeline.start = &room->edges[i];
		if (i != room->wallcount - 1)
			room->walls[i].edgeline.end = &room->edges[i + 1];
		else
			room->walls[i].edgeline.end = &room->edges[0];*/

		room->walls[i].entity->transform.position = vector3_zero();
		room->walls[i].entity->transform.scale = vector3_one();
		room->walls[i].entity->obj = object_plane(world->sdl);
		applywallmesh(&room->walls[i], room, world);
		i++;
	}
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

void	room_assign_unique_name(t_room *room, t_world *world)
{
	int	i;

	//i = 
}

t_room	*world_add_room(t_world *world, t_room *room)
{
	int		i;
	char	roomname[64];
	
	t_room	*worldroom;

	ft_bzero(roomname, 64);
	snprintf(roomname, 64, "area(%i)", ft_listlen(world->roomlist));
	ft_strcpy(room->name, roomname);
	worldroom = ft_memalloc(sizeof(t_room));
	ft_strcpy(worldroom->floortex, room->floortex);
	//worldroom->wallcount = room->wallcount;
	worldroom->walls = ft_memalloc(sizeof(t_wall) * 32); //DON'T ACTUALLY COPY ANY WALLS, JUST RECREATE THEM FROM EDGES
	worldroom->edges = room->edges;
	worldroom->edgecount = room->edgecount;
	worldroom->floor_enabled = room->floor_enabled;
	worldroom->walls_enabled = room->walls_enabled;
	worldroom->ceiling_enabled = room->ceiling_enabled;
	worldroom->loop = room->loop;
	if (room->ceiling_height == 0)
		worldroom->ceiling_height = 100;
	ft_strcpy(worldroom->name, room->name);
	worldroom->height = room->height;
	
	list_push(&world->roomlist, worldroom, sizeof(t_room));
	free(worldroom);
	worldroom = list_findlast(world->roomlist);
	//construct_edges(worldroom);
	
	//room_init(room, world);
	return (worldroom);
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
		sdl->render.gizmocolor = CLR_PRPL;
		if (from.x == to.x || from.y == to.y)
			sdl->render.gizmocolor = CLR_GREEN;
		render_ray(sdl, sdl->render, from, to);
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
		/*sdl->render.wireframe = true;
		sdl->render.gizmocolor = CLR_GREEN;*/
		render_entity(sdl, &sdl->render, room->walls[i].entity);
		//sdl->render.wireframe = false;
		i++;
	}
}

static t_line	edgeline_to_line(t_edgeline edgeline)
{
	return ((t_line){.start = *edgeline.start, .end = *edgeline.end});
}

static bool illegalwall_move(t_wall *wall, t_room *room)
{
	int		i;
	t_wall	*ow;

	i = 0;
	if (vector2_cmp(*wall->edgeline.start, *wall->edgeline.end))
	{
		return (true);
	}
		
	while (i < room->wallcount)
	{
		ow = &room->walls[i];
		if (wall != &room->walls[i] &&
			linelineintersect(
				line_shorten(edgeline_to_line(wall->edgeline)),
				line_shorten(edgeline_to_line(ow->edgeline))
				))
				{
					printf("wall %i intersects!! \n", i);
					return (true);
				}
			
		i++;
	}
	return (false);
}

//#include "syscall.h"

t_vector3	createmode_raycast(t_editor *ed, t_roomtooldata	*dat)
{
	t_vector3		result;
	t_ray			ray;
	t_raycastinfo	info;

	ray.origin = ed->player.headposition;
	ray.dir = ed->player.lookdir;
	if (dat->room->edgecount == 0)
	{	
		if (dat->raycastinfo.hit_entity != NULL)
			result = vector3_snap(dat->raycastinfo.hit_pos, 10);
		else
		{
			raycast_plane(ray, &info, 0.0f);
			result = vector3_snap(info.hit_pos, 10);
		}
	}
	else
	{
		raycast_plane(ray, &info, dat->room->height);
		result = vector3_snap(info.hit_pos, 10);
	}
	return (result);
}

void	room_setpreset_floor(t_room *room)
{
	room->floor_enabled = true;
	room->ceiling_enabled = false;
	room->walls_enabled = false;
	room->loop = true;
}


void	room_setpreset_exterior(t_room *room)
{
	room->floor_enabled = false;
	room->ceiling_enabled = false;
	room->walls_enabled = true;
}

void	room_setpreset_room(t_room *room)
{
	room->floor_enabled = true;
	room->ceiling_enabled = true;
	room->walls_enabled = true;
	room->loop = true;
}

bool	gui_roompresets(t_room *room, t_autogui *gui, t_world *world)
{
	bool	is_floor;
	bool	is_exterior;
	bool	is_room;

	is_floor = room->floor_enabled && !room->ceiling_enabled && !room->walls_enabled && room->loop;
	is_exterior = !room->floor_enabled && !room->ceiling_enabled && room->walls_enabled;
	is_room = room->floor_enabled && room->ceiling_enabled && room->walls_enabled && room->loop;
	if (gui_highlighted_button_if("Floor", gui, is_floor))
	{
		room_setpreset_floor(room);
		return (true);
	}
	if (gui_highlighted_button_if("Exterior", gui, is_exterior))
	{
		room_setpreset_exterior(room);
		return (true);
	}
	if (is_exterior && gui_highlighted_button_if("Loop", gui, room->loop))
	{
		room->loop = !room->loop;
		return (true);
	}
	if (gui_highlighted_button_if("Room", gui, is_room))
	{
		room_setpreset_room(room);
		return (true);
	}
	return (false);
}

static void	createmode(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_vector2		*edge;
	t_vector3		cursor;
	t_autogui		*gui;

	cursor = createmode_raycast(ed, dat);
	if (dat->room->edgecount == 0)
		dat->room->height = cursor.z;
	//closest_edge
	gui = &dat->newroom_gui;
	gui_start(gui);
	if (dat->room->edgecount == 0)
		gui_label("Place first wall", gui);
	else if (dat->room->edgecount > 1)
		gui_label("Shift + click to finish room", gui);
	else
		gui_emptyvertical(20, gui);
	gui_roompresets(dat->room, gui, &ed->world);
	gui_end(gui);

	dat->room->ceiling_height = 50;
	bool placelast = false;
	if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1 && dat->room->edgecount > 1)
	{
		placelast = true;
	}
	if ((ed->hid.keystate >> KEYS_ENTERMASK) & 1)
	{
		dat->rtm = rtm_modify;
		dat->room = world_add_room(&ed->world, dat->room);
		//dat->room = list_findlast(ed->world.roomlist);
		return ;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		if (!placelast)
		{
			edge = &dat->room->edges[dat->room->edgecount];
			dat->room->edgecount++;
			*edge = v3tov2(cursor);
		}
		else
		{
			if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
			{
				dat->rtm = rtm_modify;
				world_add_room(&ed->world, dat->room);
				dat->room = list_findlast(ed->world.roomlist);
				return ;
			}
		}
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		if (dat->room->edgecount >= 1)
		{
			dat->room->edgecount--;
		}
		else
		{
			free(dat->room);
			dat->room = NULL;
			dat->rtm = rtm_none;
			return;
		}
	}
	//renderroom(ed, sdl, dat->room);
	highlight_room(ed, sdl, dat->room, CLR_BLUE);
	if (is_joined(v3tov2(cursor), dat->room, &ed->world))
		render_circle(ed->world.sdl, cursor, 5, CLR_GREEN);
	else
		render_circle(ed->world.sdl, cursor, 5, CLR_BLUE);
	//render_gizmo3d(sdl, cursor, 10, CLR_RED);
	//render_gizmo3d(sdl, (t_vector3){snap.x, snap.y, dat->room->height}, 10, CLR_RED);
}

static void modifywallheights(t_room *room, int scrolldelta, t_world *world)
{
	int	i;

	i = 0;
	room->ceiling_height += scrolldelta * 10;
	room->ceiling_height = ft_clamp(room->ceiling_height, 40, 500);
	room_init(room, world);
	//make_areas(world, room);
}

bool	isroomlegal(t_world *world, t_room *room) //TODO: too many of the same functions ,plz fix
{
	int		i;
	int		ii;
	t_list	*l;
	t_room	*other;

	i = 0;
	if (room->walls_enabled == false)
		return (true);
	while (i < room->wallcount)
	{
		/*if (room->walls[i].edgeline.start == NULL || room->walls[i].edgeline.end == NULL)
			return (true);
		if (room->walls[i].edgeline.start != NULL && vector2_cmp(*room->walls[i].edgeline.start, *room->walls[i].edgeline.end))
			return (false);*/
		if (illegalwall_move(&room->walls[i], room))
			return (false);
		i++;
	}
	return (true);
}

t_list	*rooms_copy(t_list	*l)
{
	t_list	*new;

	new = NULL;
	while (l != NULL)
	{
		list_push(&new, l->content, sizeof(t_room));
		((t_room *)list_findlast(new))->walls = ft_memalloc(sizeof(t_wall) * 32);
		ft_memcpy(((t_room *)list_findlast(new))->walls, ((t_room *)l->content)->walls, sizeof(t_wall) * 32);
		l = l->next;
	}
	return (new);
}

bool	edge_is_legal(t_vector2 *edge, t_room *room)
{
	int			i;
	t_vector2	*other;

	i = 0;
	while (i < room->edgecount)
	{
		other = &room->edges[i];
		if (other != edge && vector2_dist(*edge, *other) < 10.0f)
			return (false);
		i++;
	}
	return (true);
}

typedef struct s_edgereturn
{
	t_vector2	*edge;
	t_room		*room;
}	t_edgereturn;

t_edgereturn	get_other_edge(t_vector2 *edge, t_room *room, t_world *world)
{
	t_list			*l;
	int				i;
	t_room			*other;
	t_edgereturn	er;

	ft_bzero(&er, sizeof(er));
	l = world->roomlist;
	while (l != NULL)
	{
		other = l->content;
		if (other != room)
		{
			i = 0;
			while (i < other->edgecount)
			{
				if (vector2_cmp(*edge, other->edges[i]))
				{
					er.edge = &other->edges[i];
					er.room = other;
					return (er);
				}
					
				i++;
			}
		}
		l = l->next;
	}
	return (er);
}

bool	is_connected(t_vector2 *edge, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	{
		
		if (room->walls[i].connection && (
			room->walls[i].edgeline.start == edge || room->walls[i].edgeline.end == edge))
			return (true);
		i++;
	}
	return (false);
}

t_vector2	*find_edge_match(t_vector2 start, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	{
		if (vector2_cmp(start, room->edges[i]))
			return (&room->edges[i]);
		i++;
	}
	return (NULL);
}

int	_room_find_edge_index(t_vector2 start, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	{
		if (vector2_cmp(start, room->edges[i]))
			return (i);
		i++;
	}
	return (-1);
}

void	recalculate_joined_rooms(t_world *world, t_room *room)
{
	t_list	*l;
	t_room	*other;
	int		i;

	l = world->roomlist;

	while (l != NULL)
	{
		other = l->content;
		if (other != room)
		{
			i = 0;
			while (i < room->edgecount)
			{
				if (edge_exists(room->edges[i], other))
				{
					room_init(other, world);
					//make_areas(world, other);
					break ;
				}
				i++;
			}
		}
		l = l->next;
	}
}

static int next_index(int cur, t_room *room)
{
	if (cur == room->edgecount - 1)
		return (0);
	return (cur + 1);
}

static void walleditmode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	char	text[64] = { };
	t_autogui	*gui;

	gui = &dat->modroom_gui;

	//snprintf(text, 64, "modifying selected wall");
	//print_text_boxed(&sdl, text, (t_point){sdl.window_w / 2, 80});
	//render_entity(sdl, &sdl->render, &dat->doorwalls[0]);
	highlight_entity(&sdl, dat->ed_wall->entity, AMBER_4);
	
	gui_start(gui);
	gui_labeled_int("Height: ", dat->ed_wall->height, gui);
	gui_labeled_bool_edit("Hide wall", &dat->ed_wall->entity->hidden, gui);
	if (gui_labeled_float_slider("UV offset x:", &dat->ed_wall->uv_offset.x, 1.0f, gui))
		applywallmesh(dat->ed_wall, dat->room, &ed->world);
	if (gui_labeled_float_slider("UV offset y:", &dat->ed_wall->uv_offset.y, 1.0f, gui))
		applywallmesh(dat->ed_wall, dat->room, &ed->world);
	gui_end(gui);
	/*dat->ed_wall->height += ed->hid.mouse.scroll_delta * 10;
	dat->ed_wall->height = ft_clamp(dat->ed_wall->height, 10, 200);*/
	if (ed->hid.mouse.scroll_delta != 0)
		room_init(dat->room, &ed->world);
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->ed_wall = NULL;
}

/*bool	room_joined(t_room *room1, t_room *room2)
{
	int	i;

	i = 0;
	while (i < room->edgecount)
	if (room1 == room2)
		return (false);
}*/

bool	rooms_have_joined_walls(t_room *room1, t_room *room2)
{
	bool	result;
	int		i;

	i = 0;
	while (i < room1->wallcount)
	{
		if (find_wall(room1->walls[i], room2) != NULL)
			return (true);
		i++;
	}
	return (false);
}

void	recalculate_rooms(t_editor *ed, t_vector2 *edge)
{
	t_list	*l;
	t_room	*r;
	int		i;

	l = ed->world.roomlist;
	while (l != NULL)
	{
		r = l->content;
		if (edge_exists(*edge, r))
		{
			room_init(r, &ed->world);
			//make_areas(&ed->world, r);
		}
		l = l->next;
	}
}

t_vector2	*other_room_edge(t_editor *ed, t_roomtooldata *dat)
{
	t_list		*l;
	t_room		*other;
	t_vector2	*result;

	result = NULL;
	l = ed->world.roomlist;
	while (l != NULL)
	{
		other = l->content;
		if (other != dat->room)
			result = closest_edge(other, dat->raycastinfo.hit_pos);
		l = l->next;
	}
	return (result);
}

static void render_edgegizmo(t_world *world, t_vector2 edge, uint32_t color)
{
	world->sdl->render.gizmocolor = color;
}

t_vector3	edge3d(t_vector2 edge, t_room *room)
{
	return ((t_vector3){edge.x, edge.y, room->height});
}

void	modifymode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	t_vector3		cursor;
	t_wall			*look_wall;
	t_autogui		*gui;
	t_raycastinfo	planecastinfo;
	t_ray			plane_ray;
	char			text[64] = { };

	
	gui = &dat->modroom_gui;
	highlight_room(ed, &sdl, dat->room, AMBER_3);
	if (dat->ed_wall != NULL)
	{
		walleditmode(ed, sdl, dat);
		return ;
	}
	plane_ray.origin = ed->player.headposition;
	plane_ray.dir = ed->player.lookdir;
	if (raycast_plane(plane_ray, &planecastinfo, dat->room->height))
	{
		cursor = planecastinfo.hit_pos;
		cursor.x = vector2_snap((t_vector2){cursor.x, cursor.y}, 10).x;
		cursor.y = vector2_snap((t_vector2){cursor.x, cursor.y}, 10).y;
		//rendergrid(&ed->world, planecastinfo.hit_pos, 10);
	}
	gui_start(gui);
	gui_starthorizontal(gui);
	gui_label("Modifying: ", gui);
	gui_label(dat->room->name, gui);
	gui_endhorizontal(gui);
	gui_labeled_bool("Legal: ", isroomlegal(&ed->world, dat->room), gui);
	gui_labeled_int("Edges: ", dat->room->edgecount, gui);
	gui_labeled_int("Walls: ", dat->room->wallcount, gui);
	//linecirclecollision()
	if (gui_shortcut_button("Force floor", 'F', gui))
		room_makefloor(&ed->world, dat->room);
	/*if (gui_shortcut_button("Copy area", 'C', gui)) //TODO: get working or disable before merge
		dat->rtm = rtm_connect;*/
	if (gui_roompresets(dat->room, gui, &ed->world))
		room_init(dat->room, &ed->world);
	
	if (gui_shortcut_button("Move area", 'M', gui))
	{
		dat->rtm = rtm_move;
		dat->temp_edges = NULL;
		return ;
	}
	if (gui_button("Remove associated entities", gui))
	{
		room_remove_entities(dat->room, &ed->world);
		return ;
	}
	if (gui_shortcut_button("Delete", KEYS_DELETEMASK, gui))
	{
		t_room	temp;
		ft_bzero(&temp, sizeof(t_room));
		//temp.edges = ft_memalloc(sizeof(t_vector2) * 32);
		ft_memcpy(temp.edges, dat->room->edges, sizeof(t_vector2) * dat->room->edgecount);
		temp.edgecount = dat->room->edgecount;
		remove_room(&ed->world, dat->room);
		recalculate_joined_rooms(&ed->world, &temp);
		dat->room = NULL;
		dat->rtm = rtm_none;
		gui_end(gui);
		return ;
	}
	if (dat->room->floor_enabled && gui_shortcut_button("Split floor", 'Z', gui))
	{
		dat->rtm = rtm_split;
		return ;
	}
	gui_end(gui);

	look_wall = selectedwall(ed, sdl, dat->room);
	t_vector2	*hover_edge = closest_edge(dat->room, cursor);
	room_tool_node(cursor, dat, ed);
	/*
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT) && hover_edge != NULL)
		dat->held_edge = hover_edge;
	if (hover_edge != NULL)
	{
		render_gizmo3d(ed->world.sdl, edge3d(*hover_edge, dat->room), 10, AMBER_2);
	}
	if (dat->held_edge != NULL)
	{
		render_gizmo3d(ed->world.sdl, edge3d(*dat->held_edge, dat->room), 11, AMBER_3);
		if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT) && hover_edge != dat->held_edge)
		{
			//if other edge != NULL, connect held_edge
			dat->held_edge = NULL;
			return ;
		}
		else if (ed->hid.mouse.held == MOUSE_LEFT)
		{
			dat->room_should_recalculate = true;
			if (ed->hid.keystate >> KEYS_SHIFTMASK & 1)
				applyedgedrag_solo(dat->held_edge, (t_vector2){rc.x, rc.y}, dat->room, &ed->world);
			else
				applyedgedrag(dat->held_edge, (t_vector2){rc.x, rc.y}, dat->room, &ed->world);
		}
		if (ed->hid.mouse.held == 0 && dat->room_should_recalculate)
		{
			recalculate_rooms(ed, dat->held_edge);
			dat->room_should_recalculate = false;
		}
		if (check_alpha_key(ed->hid.alphakeystate, 'x') && dat->room->edgecount > 3)
		{
			remove_edge(&ed->world, dat->room, dat->held_edge);
			dat->held_edge = NULL;
		}
		
	}*/
	if (dat->held_edge == NULL && hover_edge == NULL)
	{
		if (potentialnode(cursor, dat, ed))
			;
	}
	/*if (ed->hid.mouse.held == 0 && dat->held_edge != NULL)
	{
		recalculate_rooms(ed, dat->held_edge);
		dat->held_edge = NULL;
	}*/
	if (dat->held_edge == NULL && mouse_clicked(ed->hid.mouse, MOUSE_LEFT) && look_wall != NULL)
	{
		//force_mouseunlock(&ed->hid);
		//dat->ed_wall = look_wall;
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		dat->room = NULL;
		dat->rtm = rtm_none;
		return ;
	}
	if (ed->hid.mouse.scroll_delta != 0)
	{
		if ((ed->hid.keystate >> KEYS_LALTMASK) & 1)
		{
			printf("SCROLL %i \n", ed->hid.mouse.scroll_delta * 10);
			dat->room->height += ed->hid.mouse.scroll_delta * 10;
			dat->room->height = ft_clamp(dat->room->height, 0, 1000);
			room_init(dat->room, &ed->world);
			//make_areas(&ed->world, dat->room);
			recalculate_joined_rooms(&ed->world, dat->room);
		}
		else
		{
			modifywallheights(dat->room, ed->hid.mouse.scroll_delta, &ed->world);
			recalculate_joined_rooms(&ed->world, dat->room);
		}
	}
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
		dat->modroom_gui.minimum_size = (t_point) {240, 250};
		dat->modroom_gui.rect.size = dat->modroom_gui.minimum_size;
	}
	if (dat->paint_gui.sdl == NULL)
	{
		dat->paint_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, 80}, "Paint rooms");
	}
	if (dat->edge_gui.sdl == NULL)
	{
		dat->paint_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){60, 140}, "Edge");
		//dat->modroom_gui.minimum_size = (t_point){130, 160};
		//dat->modroom_gui.rect.size = dat->modroom_gui.minimum_size;
	}
}

void	update_maingui(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui	*gui;

	gui = &dat->maingui;
	gui_start(gui);
	//gui_labeled_point("DEBUG:", p, gui);
	if (gui_shortcut_button("New room", 'N', gui))
	{
		dat->room = ft_memalloc(sizeof(t_room));
		dat->room->walls = ft_memalloc(sizeof(t_wall) * 32);
		ft_strcpy(dat->room->floortex, "concrete02.cng");
		ft_bzero(&dat->wall.edgeline, sizeof(t_edgeline));
		dat->rtm = rtm_create;
		force_mouselock(&ed->hid);
	}
	if (gui_shortcut_button("Paint rooms", 'P', gui))
		dat->rtm = rtm_paint;
	/*if (gui_shortcut_button("Connect room", 'C', gui))
	{
	}*/
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



int		movetowards(int cur, int target)
{
	if (cur < target)
		return (cur + 1);
	return (cur - 1);
}

void	finish_partial_room(t_editor *ed, t_room	*room, t_roomtooldata *data)
{
	t_room		*orig;
	t_vector2	*og_start;
	int			og_start_i;
	t_vector2	*og_end;
	int			og_end_i;
	t_vector2	*edge_og_new;
	int			ei;
	int			i;
	int			orig_wallcount;

	orig = data->room;
	room->wallcount = 0;
	edge_og_new = ft_memalloc(sizeof(t_vector2) * 32);
	ei = 0;
	og_start = find_edge_match(room->edges[0], orig);
	og_end = find_edge_match(room->edges[room->edgecount - 1], orig);
	og_start_i = _room_find_edge_index(*og_start, orig);
	og_end_i = _room_find_edge_index(*og_end, orig);
	if (og_start != NULL && og_end != NULL)
	{
		printf("found area start and end in other room \n");
	}
	i = 0;
	while (i < room->edgecount)
	{
		edge_og_new[ei++] = room->edges[i];
		printf("connect %i -> %i \n", i, i + 1);
		i++;
		//printf("%i \n", i);
	}
	i = _room_find_edge_index(room->edges[room->edgecount - 1], orig);
	int t_start = i;
	int target = _room_find_edge_index(room->edges[0], orig);
	printf("adding edges from orig, range %i -> %i \n", i, target);
	while (i != target)
	{
		printf("adding %i from orig \n", i);
		if (!edge_exists(orig->edges[i], room))
			room->edges[room->edgecount++] = orig->edges[i];
		i++;
		if (i > orig->edgecount - 1)
			i = 0;
	}
	i = t_start;
	while (i != target)
	{
		if (!edge_exists(orig->edges[i], room))
			edge_og_new[ei++] = orig->edges[i];
		i--;
		if (i < 0)
			i = orig->edgecount - 1;
		if (i == target)
			break ;
	}
	//TODO: check if both rooms are legal, and have >= 3 edges
	orig->edges = edge_og_new;
	orig->edgecount = ei;
	i = 0;
	room_init(orig, &ed->world);
	room = world_add_room(&ed->world, room);
	room->height = orig->height;
	room->ceiling_height = orig->ceiling_height;
	room->floor_enabled = true;
	room->walls_enabled = orig->walls_enabled;
	room->ceiling_enabled = orig->ceiling_enabled;
	room->loop = orig->loop;
	ft_strcpy(room->floortex, orig->floortex);
	room_init(orig, &ed->world);
	room_init(room, &ed->world);
	//make_areas(&ed->world, orig);
	//make_areas(&ed->world, room);
	i = 0;
	data->room = room;
	data->rtm = rtm_modify;
}

t_room	combined_room(t_room *room1, t_room *room2)
{
	t_room		newroom;
	int			i;
	t_vector2	last_common;


	i = 0;
	ft_bzero(&newroom, sizeof(t_room));
	newroom.edges = ft_memalloc(sizeof(t_vector2) * 32);
	newroom.walls = ft_memalloc(sizeof(t_wall) * 32);
	t_wall	test;
	ft_bzero(&test, sizeof(t_wall));
	while (i < room1->edgecount)
	{
		test.edgeline.start = &room1->edges[i];
		test.edgeline.end = &room1->edges[next_index(i, room1)];
		if (find_wall(test, room2) == NULL)
		{
			newroom.edges[newroom.edgecount++] = room1->edges[i];
		} //IF EXISTS IN BOTH, TRACK
		if (edge_exists(room1->edges[i], room2))
			last_common = room1->edges[i];
		i++;
	}

	int	count = 0;
	i = _room_find_edge_index(last_common, room2);
	while (count < room2->edgecount)
	{
		test.edgeline.start = &room2->edges[i];
		test.edgeline.end = &room2->edges[next_index(i, room2)];
		if (find_wall(test, room1) == NULL && !edge_exists(room2->edges[i], &newroom))
			newroom.edges[newroom.edgecount++] = room2->edges[i];
		i++;
		count++;
		if (i > room2->edgecount - 1)
		{
			i = 0;
		}
	}
	return (newroom);

	//Find common walls
	/*iterate over all the edges
		if the edge is present ONLY in common walls, remove it


	*/
}

t_room	*find_entity_room(t_world *world, t_entity *ent)
{
	t_list	*l;
	t_room	*room;
	int		i;

	l = world->roomlist;
	if (ent == NULL)
		return (NULL);
	while (l != NULL)
	{
		room = l->content;
		i = 0;
		while (i < room->wallcount)
		{
			if (room->walls[i].entity == ent)
				return (room);
			i++;
		}
		i = 0;
		while (i < room->floorcount)
		{
			if (room->floors[i].entity == ent)
				return (room);
			i++;
		}
		l = l->next;
	}
	return (NULL);
}

void	room_tool_combine(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	return ;
	static	t_room	*other;

	other = find_entity_room(&ed->world, dat->raycastinfo.hit_entity);
	if (other != NULL && other != dat->room)
	{
		highlight_room(ed, ed->world.sdl, other, CLR_GREEN);
		if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
		{
			t_room	newroom;
			newroom = combined_room(dat->room, other);
			world_add_room(&ed->world, &newroom);
			t_room	*world_room = list_findlast(ed->world.roomlist);
			world_room->ceiling_height = dat->room->ceiling_height; //TODO: min/max
			world_room->height = dat->room->height;
			room_init(world_room, &ed->world);
			//make_areas(&ed->world, world_room);
			dat->room = NULL;
			dat->rtm = rtm_none;
		}
	}
}

void	room_tool_split(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui			*gui;
	t_vector3			rc;
	t_wall				*cur;
	t_vector2			*edge;
	static	t_room		*new_room;

	if (new_room == NULL)
	{
		new_room = ft_memalloc(sizeof(t_room));
		new_room->walls = ft_memalloc(sizeof(t_wall) * 32);
		new_room->edges = ft_memalloc(sizeof(t_vector2) * 32);
		printf("allocated split room\n");
		//new_room->walls = ft_memalloc(sizeof(t_wall) * 32);
	}
	rc = dat->raycastinfo.hit_pos;
	rc.x = vector2_snap((t_vector2){rc.x, rc.y}, 2).x;
	rc.y = vector2_snap((t_vector2){rc.x, rc.y}, 2).y;
	//snap = raycast_DEPRECATED(ed);
	//rc = planecastinfo.hit_pos;
	
	edge = closest_edge(dat->room, rc);
	if (edge != NULL)
	{
		rc.x = edge->x;
		rc.y = edge->y;
		sdl->render.gizmocolor = CLR_RED;
		if (new_room->edgecount > 0)
			sdl->render.gizmocolor = CLR_GREEN;
		render_gizmo(*sdl, sdl->render, vector3_add(v2tov3(*edge), (t_vector3){.z = dat->room->height}), 10);
	}
	sdl->render.gizmocolor = AMBER_4;
	render_gizmo(*sdl, sdl->render, rc, 7);
	sdl->render.gizmocolor = AMBER_2;
	t_vector3	from;
	t_vector3	to;
	if (new_room->edgecount > 0)
	{

		from = v2tov3(new_room->edges[new_room->edgecount - 1]);
		from.z = dat->room->height;
		to = rc;
		render_ray(sdl, from, to);
	}
		

	int	i = 0;
	
	while (i < new_room->edgecount)
	{
		if (i < new_room->edgecount - 1)
		{
			from = v2tov3(new_room->edges[i]);
			to = v2tov3(new_room->edges[i + 1]);
			from.z = dat->room->height;
			to.z = dat->room->height;
			render_ray(sdl, from, to);
		}
		//render_gizmo(*sdl, sdl->render, vector2_to_vector3(new_room->edges[i]), 6);
		i++;
	}

	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		if (new_room->edgecount > 0 || (new_room->edgecount == 0 && edge != NULL))
		{
			
			new_room->edges[new_room->edgecount] = (t_vector2){rc.x, rc.y};
			new_room->edgecount++;
			if (edge != NULL && new_room->edgecount > 1)
			{
				finish_partial_room(ed, new_room, dat);
				free(new_room);
				new_room = NULL;
				//dat->rtm = rtm_none;
				//dat->room = NULL;
				return ;
			}
			
		}
	}

	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		if (new_room->edgecount > 0)
			new_room->edgecount--;
		else
		{
			free(new_room);
			new_room = NULL;
			dat->rtm = rtm_none;
			dat->room = NULL;
		}
		
	}
}

t_vector3	room_center(t_room *room)
{
	int			i;
	t_vector3	center;

	i = 0;
	center = vector3_zero();
	while (i < room->edgecount)
	{
		center = vector3_add(center, v2tov3(room->edges[i]));
		i++;
	}
	center = vector3_div(center, room->edgecount);
}

t_room	*copy_room(t_room *original)
{
	t_room	*room = ft_memalloc(sizeof(t_room));

	room->edges = (t_vector2 *)ft_memdup(original->edges, sizeof(t_vector2) * 32);
	room->edgecount = original->edgecount;
	room->height = original->height;
	room->ceiling_height = original->ceiling_height;
	//room->walls = ft_memdup(original->edges, sizeof(t_vector2) * 32);
	return (room);
}

void	room_tool_move(t_editor *ed, t_roomtooldata *dat)
{
	t_raycastinfo	planecastinfo;
	t_ray			planeray;
	int				i;
	
	if (dat->temp_edges == NULL)
	{
		dat->temp_edges = (t_vector2 *)ft_memdup(dat->room->edges, sizeof(t_vector2) * 32);
		dat->temp_height = dat->room->height;
	}

	planeray.origin = ed->player.transform.position;
	planeray.dir = ed->player.lookdir;

	raycast_plane(planeray, &planecastinfo, dat->room->height);

	highlight_room(ed, ed->world.sdl, dat->room, AMBER_3);
	//rendergrid(&ed->world, planecastinfo.hit_pos, 30);
	i = 0;
	t_vector3	diff = vector3_sub(planecastinfo.hit_pos, room_center(dat->room));
	render_gizmo3d(ed->world.sdl, planecastinfo.hit_pos, 10, CLR_RED);
	if (ed->hid.mouse.scroll_delta != 0)
	{
		dat->room->height += ed->hid.mouse.scroll_delta * 10;
		dat->room->height = ft_clamp(dat->room->height, 0, 400);
	}
	while (i < dat->room->edgecount)
	{
		dat->room->edges[i] = vector2_add(dat->room->edges[i], v3tov2(diff));
		dat->room->edges[i] = vector2_snap(dat->room->edges[i], 10);
		i++;
	}
	t_autogui *gui = &dat->modroom_gui;
	gui_start(gui);
	if (gui_shortcut_button("Copy room", 'C', gui))
	{
		t_room	*room = copy_room(dat->room);
		t_room *newr = world_add_room(&ed->world, room);
		dat->room = newr;
		return ;
	}
	gui_end(gui);
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		room_init(dat->room, &ed->world);
		//make_areas(&ed->world, dat->room);
		dat->rtm = rtm_modify;
	}

	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		ft_memcpy(dat->room->edges, dat->temp_edges, sizeof(t_vector2) * 32);
		free(dat->temp_edges);
		dat->temp_edges = NULL;
		dat->room->height = dat->temp_height;
		dat->rtm = rtm_modify;
	}
}

t_room	*get_raycast_room(t_raycastinfo info, t_world *world)
{
	t_list	*l;
	t_room	*room;
	int		i;
	if (info.hit_entity == NULL)
		return (NULL);
	l = world->roomlist;
	while (l != NULL)
	{
		room = (t_room *)l->content;
		i = 0;
		while (i < room->floorcount)
		{
			if (room->floors[i].entity == info.hit_entity)
				return (room);
			i++;
		}
		i = 0;
		while (i < room->wallcount)
		{
			if (room->walls[i].entity == info.hit_entity)
				return (room);
			i++;
		}
		l = l->next;
	}
	return (NULL);
}

void	room_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_roomtooldata	*dat;
	t_line			line;
	t_ray			ray;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	ray.origin = ed->player.headposition;
	ray.dir = ed->player.lookdir;
	ft_bzero(&dat->raycastinfo, sizeof(t_raycastinfo));
	raycast_new(ray, &dat->raycastinfo, &ed->world);
	/*if (!raycast_new(ray, &dat->raycastinfo, &ed->world) && dat->rtm == rtm_create)
	{
		raycast_plane(ray, &dat->raycastinfo, -0.1f);
	}*/
	t_list	*l;

	l = ed->world.roomlist;
	while (l != NULL)
	{
		highlight_room(ed, sdl, (t_room *)l->content, AMBER_1);
		l = l->next;
	}
	if (dat->rtm == rtm_none)
	{
		t_room	*hover;
		hover = get_raycast_room(dat->raycastinfo, &ed->world);
		if (hover != dat->room && hover != NULL) //AND SHIFT NOT PRESSED
		{
			highlight_room(ed, sdl, hover, AMBER_2);
			print_text_boxed(sdl, hover->name, point_div(sdl->screensize, 2));
			if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
			{
				dat->held_edge = NULL;
				dat->room = hover;
				dat->rtm = rtm_modify;
				ed->hid.mouse.click_unhandled = false;
			}
		}
	}
	if (dat->room != NULL && dat->rtm == rtm_create)
		createmode(ed, sdl, dat);
	update_maingui(ed, sdl, dat);

	if (dat->rtm == rtm_move && dat->room != NULL)
	{
		room_tool_move(ed, dat);
	}
	if (dat->rtm == rtm_modify && dat->room != NULL)
		modifymode(ed, *sdl, dat);
	if (dat->rtm == rtm_split && dat->room != NULL)
		room_tool_split(ed, sdl, dat);
	if (dat->rtm == rtm_paint/* && dat->room != NULL*/)
		room_tool_paint(ed, sdl, dat);
	if (dat->rtm == rtm_connect && dat->room != NULL)
		room_tool_combine(ed, sdl, dat);
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
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
