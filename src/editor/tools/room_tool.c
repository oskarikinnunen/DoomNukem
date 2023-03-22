/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 14:53:44 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

void	recalculate_joined_rooms(t_world *world, t_area *room);
void	room_assign_unique_name(t_area *room, t_world *world)
{
	t_list	*l;
	
}


//TODO: room_free
t_area	*world_add_room(t_world *world, t_area *room)
{
	int		i;
	char	roomname[64];
	
	t_area	*worldroom;
	ft_bzero(roomname, 64);
	snprintf(roomname, 64, "area(%i)", ft_listlen(world->arealist));
	ft_strcpy(room->name, roomname);
	worldroom = ft_memalloc(sizeof(t_area));
	ft_strcpy(worldroom->s_floortex.str, room->s_floortex.str);
	ft_strcpy(worldroom->s_ceiltex.str, room->s_ceiltex.str);
	ft_memcpy(worldroom->edges, room->edges, sizeof(worldroom->edges));
	worldroom->edgecount = room->edgecount;
	worldroom->floor_enabled = room->floor_enabled;
	worldroom->walls_enabled = room->walls_enabled;
	worldroom->ceiling_enabled = room->ceiling_enabled;
	worldroom->loop = room->loop;
	if (room->ceiling_height == 0)
		worldroom->ceiling_height = 100;
	ft_strcpy(worldroom->name, room->name);
	worldroom->height = room->height;
	list_push(&world->arealist, worldroom, sizeof(t_area));
	free(worldroom);
	worldroom = list_findlast(world->arealist);
	return (worldroom);
}

void	world_remove_room(t_world *world, t_area *room)
{
	room_remove_entities(room, world);
	list_remove(&world->arealist, room, sizeof(t_area));
}

static t_line	edgeline_to_line(t_edgeline edgeline)
{
	return ((t_line){.start = *edgeline.start, .end = *edgeline.end});
}

static bool	illegalwall_move(t_wall *wall, t_area *room)
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
		return (wall != &room->walls[i]
			&& linelineintersect(
				line_shorten(edgeline_to_line(wall->edgeline)),
				line_shorten(edgeline_to_line(ow->edgeline))
			));
		i++;
	}
	return (false);
}

void	room_setpreset_floor(t_area *room)
{
	room->floor_enabled = true;
	room->ceiling_enabled = false;
	room->walls_enabled = false;
	room->loop = true;
}

void	room_setpreset_exterior(t_area *room)
{
	room->floor_enabled = false;
	room->ceiling_enabled = false;
	room->walls_enabled = true;
}

void	room_setpreset_room(t_area *room)
{
	room->floor_enabled = true;
	room->ceiling_enabled = true;
	room->walls_enabled = true;
	room->loop = true;
}

t_areastatus	room_get_areastatus(t_area area)
{
	t_areastatus	status;

	status.is_floor \
		= area.floor_enabled && !area.ceiling_enabled
		&& !area.walls_enabled && area.loop;
	status.is_exterior \
		= !area.floor_enabled && !area.ceiling_enabled
		&& area.walls_enabled;
	status.is_room \
		= area.floor_enabled && area.ceiling_enabled
		&& area.walls_enabled && area.loop;
	return (status);
}

bool	gui_roompresets(t_area *room, t_autogui *gui, t_world *world)
{
	t_areastatus	status;

	status = room_get_areastatus(*room);
	if (gui_highlighted_button_if("Floor", gui, status.is_floor))
	{
		room_setpreset_floor(room);
		return (true);
	}
	if (gui_highlighted_button_if("Exterior", gui, status.is_exterior))
	{
		room_setpreset_exterior(room);
		return (true);
	}
	if (status.is_exterior
		&& gui_highlighted_button_if("Loop", gui, room->loop))
	{
		room->loop = !room->loop;
		return (true);
	}
	if (gui_highlighted_button_if("Room", gui, status.is_room))
	{
		room_setpreset_room(room);
		return (true);
	}
	return (false);
}

static void	modifywallheights(t_area *room, int scrolldelta, t_world *world)
{
	int	i;

	i = 0;
	room->ceiling_height += scrolldelta * 10;
	room->ceiling_height = ft_clamp(room->ceiling_height, 0, 2500);
	room_init(room, world);
}

bool	isroomlegal(t_world *world, t_area *room)
{
	int		i;
	int		ii;
	t_list	*l;
	t_area	*other;

	i = 0;
	if (room->walls_enabled == false)
		return (true);
	while (i < room->wallcount)
	{
		if (illegalwall_move(&room->walls[i], room))
			return (false);
		i++;
	}
	return (true);
}

bool	edge_is_legal(t_vector2 *edge, t_area *room)
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

t_vector2	*find_edge_match(t_vector2 start, t_area *room)
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

int	_room_find_edge_index(t_vector2 start, t_area *room)
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

void	recalculate_joined_rooms(t_world *world, t_area *room)
{
	t_list	*l;
	t_area	*other;
	int		i;

	l = world->arealist;
	while (l != NULL)
	{
		other = l->content;
		if (other != room)
		{
			i = 0;
			while (i < room->edgecount)
			{
				if (edge_exists(room->edges[i], other))
					room_init(other, world);
				i++;
			}
		}
		l = l->next;
	}
}

static void	room_tool_remove_room(t_editor *ed, t_roomtooldata *dat)
{
	t_area	temp;

	ft_bzero(&temp, sizeof(t_area));
	ft_memcpy(temp.edges, dat->room->edges,
		sizeof(t_vector2) * dat->room->edgecount);
	temp.edgecount = dat->room->edgecount;
	world_remove_room(&ed->world, dat->room);
	recalculate_joined_rooms(&ed->world, &temp);
	dat->room = NULL;
	dat->rtm = rtm_none;
}

static void	modifymode_modifyheights(t_editor *ed, t_roomtooldata *dat)
{
	if (ed->hid.mouse.scroll_delta != 0)
	{
		if ((ed->hid.keystate >> KEYS_LALTMASK) & 1)
		{
			dat->room->height += ed->hid.mouse.scroll_delta * 10;
			dat->room->height = ft_clamp(dat->room->height, 0, 1000);
			room_init(dat->room, &ed->world);
			recalculate_joined_rooms(&ed->world, dat->room);
		}
		else
		{
			modifywallheights(dat->room,
				ed->hid.mouse.scroll_delta, &ed->world);
			recalculate_joined_rooms(&ed->world, dat->room);
		}
	}
}

static void	modifymode_gui(t_editor *ed, t_roomtooldata *dat)
{
	t_autogui	*gui;

	if (dat->room == NULL)
		return ;
	gui = &dat->modroom_gui;
	gui_start(gui);
	gui_starthorizontal(gui);
	gui_label("Modifying: ", gui);
	gui_label(dat->room->name, gui);
	gui_endhorizontal(gui);
	//gui_labeled_bool("Legal: ", isroomlegal(&ed->world, dat->room), gui);
	gui_labeled_int("Edges: ", dat->room->edgecount, gui);
	gui_labeled_int("Walls: ", dat->room->wallcount, gui);
	if (gui_roompresets(dat->room, gui, &ed->world))
		room_init(dat->room, &ed->world);
	if (gui_shortcut_button("Delete", KEYS_DELETEMASK, gui))
	{
		room_tool_remove_room(ed, dat);
		gui_end(gui);
		return ;
	}
	gui_end(gui);
}

static t_vector3	modifymode_get_cursor(t_editor *ed, t_roomtooldata *dat)
{
	t_vector3		cursor;
	t_raycastinfo	planecastinfo;
	t_ray			plane_ray;

	//highlight_room(ed, ed->world.sdl, dat->room, AMBER_3);
	cursor = vector3_zero();
	ft_bzero(&planecastinfo, sizeof(planecastinfo));
	plane_ray.origin = ed->player.headposition;
	plane_ray.dir = ed->player.lookdir;
	if (raycast_plane(plane_ray, &planecastinfo, dat->room->height))
	{
		cursor = planecastinfo.hit_pos;
		cursor.x = vector2_snap((t_vector2){cursor.x, cursor.y}, 10).x;
		cursor.y = vector2_snap((t_vector2){cursor.x, cursor.y}, 10).y;
	}
	return (cursor);
}

void	modifymode(t_editor *ed, t_sdlcontext sdl, t_roomtooldata *dat)
{
	t_vector3		cursor;

	highlight_room(ed, &sdl, dat->room, AMBER_3);
	cursor = modifymode_get_cursor(ed, dat);
	modifymode_gui(ed, dat);
	if (dat->room == NULL)
		return ;
	room_tool_node(cursor, dat, ed);
	if (dat->held_edge == NULL)
		potentialnode(cursor, dat, ed);
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		dat->room = NULL;
		dat->rtm = rtm_none;
		return ;
	}
	modifymode_modifyheights(ed, dat);
}

void	update_maingui(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui	*gui;

	gui = &dat->maingui;
	gui_start(gui);
	if (gui_shortcut_button("New room", 'N', gui))
	{
		dat->room = ft_memalloc(sizeof(t_area));
		ft_strcpy(dat->room->s_floortex.str, "concrete02.cng");
		ft_strcpy(dat->room->s_ceiltex.str, "concrete02.cng");
		dat->rtm = rtm_create;
		room_setpreset_room(dat->room);
		force_mouselock(&ed->hid);
	}
	if (gui_shortcut_button("Paint rooms", 'P', gui))
		dat->rtm = rtm_paint;
	gui_end(gui);
}

void	room_tool_update_nonemode(t_editor *ed,
		t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_area	*hover;

	hover = NULL;
	hover = get_raycast_room(dat->raycastinfo, &ed->world);
	if (hover != dat->room && hover != NULL)
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

void	room_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_roomtooldata	*dat;
	t_ray			ray;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	
	room_tool_raycast(ed, dat);
	if (dat->rtm == rtm_none)
		room_tool_update_nonemode(ed, sdl, dat);
	if (dat->room != NULL && dat->rtm == rtm_create)
		createmode(ed, sdl, dat);
	update_maingui(ed, sdl, dat);
	if (dat->rtm == rtm_modify && dat->room != NULL)
	{
		modifymode(ed, *sdl, dat);
	}
		
	if (dat->rtm == rtm_paint)
		room_tool_paint(ed, sdl, dat);
}
