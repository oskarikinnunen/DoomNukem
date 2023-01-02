/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 11:32:36 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/02 18:55:13 by okinnune         ###   ########.fr       */
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
		if (room->walls[i].disabled)
			room->walls[i].entity->hidden = true;

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

void	split_area(t_room *room, t_floor_area *old, t_floor_area *area, t_editor *ed, t_sdlcontext *sdl)
{
	/*t_wall	*w;
	t_wall	*sw;
	t_wall	*ew;
	int		start;
	int		i;

	printf("room total walls: %i \n", room->wallcount);
	printf("unique walls in area %i \n", area->unique_wallcount);
	area->edgecount = 0;
	while (area->edgecount < area->unique_wallcount)
	{
		area->edge_indices[area->edgecount] = (room->wallcount - area->unique_wallcount) + (area->edgecount);
		area->edgecount++;
	}
	printf("area wall count now: %i \n", area->unique_wallcount);
	sw = &room->walls[area->edge_indices[0]];
	ew = &room->walls[area->edge_indices[area->edgecount - 1]];
	printf("assigned sw as wall %i and ew as wall %i\n", area->edge_indices[0], area->edge_indices[area->edgecount - 1]);
	i = 0;
	w = &room->walls[old->edge_indices[i]];
	while (!vector2_cmp(w->line.start, ew->line.end))
	{
		if (i >= old->edgecount)
			i = 0;
		w = &room->walls[old->edge_indices[i]];
		printf("searching for ew->end\n");
		i++;
	}
	printf("start index is %i \n", i - 1);
	i -= 1;
	start = i;
	while (!vector2_cmp(w->line.end, sw->line.start))
	{
		if (i >= old->edgecount)
			i = 0;
		w = &room->walls[old->edge_indices[i]];
		area->edge_indices[area->edgecount] = old->edge_indices[i];
		area->edgecount++;
		printf("searching for sw->start\n");
		i++;
	}
	i = start;
	t_floor_area temp = *old;
	old->edgecount = 0;
	w = &room->walls[temp.edge_indices[i]];
	while (!vector2_cmp(w->line.start, sw->line.start))
	{
		if (i < 0)
			i = temp.edgecount - 1;
		w = &room->walls[temp.edge_indices[i]];
		old->edge_indices[old->edgecount] = temp.edge_indices[i];
		old->edgecount++;
		i--;
	}
	i = 0;
	while (i < area->edgecount)
	{
		printf("wall [%i] is %i\n", i, area->edge_indices[i]);
		i++;
	}
	area->height += room->walls[area->edge_indices[0]].height;
	room->floor_areas[room->floor_areacount] = *area;
	room->floor_areacount++;
	free_floor(&ed->world, room);
	*/
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
	while (i < room->edgecount)
	{
		room->floor_areas[0].edge_indices[i] = i;
		room->floor_areas[0].edges[i] = &room->edges[i];
		i++;
	}
	room->floor_areas[0].edgecount = room->edgecount;
	room->floor_areas[0].unique_wallcount = 0;
	//TODO: make a custom assign_floor area that takes the input from "floorarea_editmode" or whatevs, 
}

static void construct_edges(t_room *room)
{
	
	room->edges = ft_memalloc(sizeof(t_vector2) * 32);
	int	i = 0;
	while (i < room->wallcount)
	{
		//room->edges[i] = room->walls[i].line.start;
		room->walls[i].edgeline.start_index = i;
		room->walls[i].edgeline.end_index = i + 1;
		if (i == room->wallcount - 1)
			room->walls[i].edgeline.end_index = 0;
		i++;
	}
	room->edgecount = i;
	i = 0;
	/*while (i < room->wallcount)
	{
		room->walls[i].edgeline.start = &room->edges[room->walls[i].edgeline.start_index];
		room->walls[i].edgeline.end = &room->edges[room->walls[i].edgeline.end_index];
		i++;
	}*/
	//room->walls[0].
}

t_room	*add_room_to_world(t_world *world, t_room *room)
{
	int		i;
	char	roomname[64];
	
	t_room	*worldroom;
	t_wall	*w_wall;
	t_wall	*wall;

	ft_bzero(roomname, 64);
	snprintf(roomname, 64, "area(%i)", ft_listlen(world->roomlist));
	ft_strcpy(room->name, roomname);
	worldroom = ft_memalloc(sizeof(t_room));
	//worldroom->wallcount = room->wallcount;
	worldroom->walls = ft_memalloc(sizeof(t_wall) * 32); //DON'T ACTUALLY COPY ANY WALLS, JUST RECREATE THEM FROM EDGES
	worldroom->edges = room->edges;
	worldroom->edgecount = room->edgecount;
	if (room->ceiling_height == 0)
		room->ceiling_height = 100;
	ft_strcpy(worldroom->name, room->name);
	worldroom->height = room->height;
	
	list_push(&world->roomlist, worldroom, sizeof(t_room));
	free(worldroom);
	worldroom = list_findlast(world->roomlist);
	
	//construct_edges(worldroom);
	assign_default_floor_area(worldroom);
	init_roomwalls(world, worldroom);
	//init_roomwalls(world, worldroom);
	return (worldroom);
	//free(room->walls);
	//free(room);
	//exit(0);
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
		printf("SAME START AND END");
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
 //TODO: call default_wall_occlusion_settings(&room->walls[i], world);

static void	createmode(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_wall			*cur;
	t_vector2		*edge;
	t_vector3		rc;
	t_vector2		snap;
	t_autogui		*gui;

	cur = &dat->wall;
	rc = raycast_DEPRECATED(ed);
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


	cur->edgeline.end = &snap;
	bool placelast = false;
	if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1 && dat->room->wallcount > 1)
	{
		placelast = true;
		cur->edgeline.end = &dat->room->edges[0];
	}
		
	if (cur->edgeline.start != NULL && cur->edgeline.end != NULL)
	{
		applywallmesh(cur, dat->room, &ed->world);
		render_entity(sdl, &sdl->render, cur->entity);
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		if (!placelast)
		{
			edge = &dat->room->edges[dat->room->edgecount];
			dat->room->edgecount++;
			*edge = snap;
		}
		if (cur->edgeline.start == NULL)
			cur->edgeline.start = edge;
		else
		{
			ft_strcpy(cur->texname, "grid_d.png");
			if (!placelast)
				cur->edgeline.end = edge;
			dat->room->walls[dat->room->wallcount] = *cur;
			dat->room->walls[dat->room->wallcount].entity = ft_memalloc(sizeof(t_entity));
			ft_memcpy(dat->room->walls[dat->room->wallcount].entity, cur->entity, sizeof(t_entity));
			dat->room->wallcount++;
			init_roomwalls_shallow(&ed->world, dat->room);
			cur->edgeline.start = edge;
			if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
			{
				dat->rtm = rtm_modify;
				add_room_to_world(&ed->world, dat->room);
				dat->room = list_findlast(ed->world.roomlist);
				return ;
			}
		}
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		if (dat->room->wallcount >= 1)
		{
			dat->room->edgecount--;
			dat->wall.edgeline.start = &dat->room->edges[dat->room->edgecount - 1];
			dat->room->wallcount--;
			//dat->room->edgecount--;
			init_roomwalls_shallow(&ed->world, dat->room);
			applywallmesh(&dat->wall, dat->room, &ed->world);
		}
		else
		{
			free(dat->room);
			dat->room = NULL;
			ft_bzero(&dat->wall.edgeline, sizeof(t_edgeline));
			dat->rtm = rtm_none;
			return;
		}
	}
	renderroom(ed, sdl, dat->room);
	highlight_room(ed, sdl, *dat->room, CLR_RED);
	render_snapgrid(ed, sdl, snap, false, false);
}

static void modifywallheights(t_room *room, int scrolldelta, t_world *world)
{
	int	i;

	i = 0;
	room->ceiling_height += scrolldelta * 10;
	room->ceiling_height = ft_clamp(room->ceiling_height, 40, 300);
	init_roomwalls(world, room);
}

bool	isroomlegal(t_world *world, t_room *room) //TODO: too many of the same functions ,plz fix
{
	int		i;
	int		ii;
	t_list	*l;
	t_room	*other;

	i = 0;
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

static void init_worldwalls(t_world *world)
{
	t_list	*l;

	l = world->roomlist;
	while (l != NULL)
	{
		free_floor(world, l->content);
		init_roomwalls(world, l->content);
		l = l->next;
	}
}

bool	edge_is_legal(t_vector2 *edge, t_room *room)
{
	int			i;
	t_vector2	*other;

	i = 0;
	while (i < room->edgecount)
	{
		other = &room->edges[i];
		if (other != edge && vector2_sqr_dist(*edge, *other) < 1000.0f)
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
	while (i < room->wallcount)
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

int	find_edge_match_index(t_vector2 start, t_room *room)
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

void	applyedgedrag(t_vector2 *edge, t_vector2 snap, t_room *room, t_world *world)
{
	int				i;
	t_vector2		temp;
	t_vector2		*test;
	t_room			*cur;
	t_list			*l;
	bool			legal;
	t_vector2		orig;

	orig = *edge;
	if (vector2_cmp(*edge, snap))
		return ;
	legal = true;
	l = world->roomlist;
	while (l != NULL)
	{
		cur = l->content;
		if (edge_exists(*edge, cur))
		{
			test = find_edge_match(*edge, cur);
			temp = *test;
			*test = snap;
			if (!edge_is_legal(test, cur) || !isroomlegal(world, cur))
				legal = false;
			*test = temp;
		}
		l = l->next;
	}

	if (legal)
	{
		l = world->roomlist;
		while (l != NULL)
		{
			cur = l->content;
			if (edge_exists(orig, cur))
			{
				printf("exists in room %s \n", cur->name);
				i = find_edge_match_index(orig, cur);
				cur->edges[i] = snap;
				//init_roomwalls(world, cur);
				//free_floor(world, cur);
			}
			l = l->next;
		}

		l = world->roomlist;
		while (l != NULL)
		{
			cur = l->content;
			/*if (edge_exists(snap, cur))
			{
				//init_roomwalls(world, cur);
				//free_floor(world, cur);
			}*/
			l = l->next;
		}
	}
	/*
	*edge = snap;
	if (edge_is_legal(edge, room) && isroomlegal(world, room))
	{
		init_roomwalls(world, room);
		free_floor(world, room);
	}
	else
		*edge = temp;*/
}

void	highlight_room_corners(t_editor *ed, t_sdlcontext *sdl, t_room *room)
{
	int	i;

	i = 0;
	sdl->render.gizmocolor = AMBER_0;
	while (i < room->wallcount)
	{
		//render_gizmo(*sdl, sdl->render, vector2_to_vector3(room->walls[i].line.start), 5);
		i++;
	}
}

static int next_index(int cur, t_room *room)
{
	if (cur == room->edgecount - 1)
		return (0);
	return (cur + 1);
}

static void	split_wall(t_wall *wall, t_room **room, t_world *world)
{
	t_vector2	*new_edges;
	t_wall		*new_walls;
	//t_Wall		*cur;
	
	/*t_vector2	*start_edge;
	t_vector2	*end_edge;*/
	int	ni;
	int	i;


	new_walls = ft_memalloc(sizeof(t_wall) * 32);
	i = 0;
	while (i < (*room)->wallcount)
	{
		//cur = room->walls[i];
		if (&(*room)->walls[i] == wall)
		{
			//printf("splitting wall %i -> %i")
			int copybefore = ft_clamp(i, 0, 32);
			ft_memcpy(new_walls, (*room)->walls, sizeof(t_wall) * copybefore);
			printf("copied %i walls before new split wall \n", copybefore);
			ni = i;
			//new_walls[i].connection = false;
			new_walls[i].height = (*room)->walls[i].height;
			ft_strcpy(new_walls[i].texname, (*room)->walls[i].texname);
			int copyafter = ft_clamp((*room)->wallcount - i, 0, 32);
			ft_memcpy(new_walls + i + 1, (*room)->walls + i, sizeof(t_wall) * copyafter);
			printf("copied %i walls after new split wall \n", copyafter);
			break ;
		}
		i++;
	}
	//room->walls = new_walls;
	new_edges = ft_memalloc(sizeof(t_vector2) * 32);
	i = 0;
	while (i < (*room)->edgecount)
	{
		int	nxt = next_index(i, *room);
		if (&(*room)->edges[i] == (*room)->walls[ni].edgeline.start
			&& &(*room)->edges[nxt] == (*room)->walls[ni].edgeline.end)
		{
			printf("%i edge is wallmatch \n", i);
			ft_memcpy(new_edges, (*room)->edges, sizeof(t_vector2) * ft_clamp(i + 1, 0, 32));
			printf("copied %i edges b4 \n", i + 1);
			printf("new edges' index is %i \n", i + 1);
			new_edges[i + 1] = vector2_lerp((*room)->edges[i], (*room)->edges[nxt], 0.5f);
			ft_memcpy(new_edges + i + 2, (*room)->edges + i + 1, sizeof(t_vector2) * ((*room)->edgecount - i - 1));
			printf("copied %i edges after \n", (*room)->edgecount - i - 1);
			break ;
		}
		i++;
	}
	free_roomwalls(world, *room);
	i = 0;
	(*room)->edgecount++;
	(*room)->wallcount++;
	(*room)->edges = new_edges;
	(*room)->walls = new_walls;
	set_nullentities(&(*room)->walls, (*room)->wallcount);
	init_roomwalls(world, *room);
	assign_default_floor_area(*room);
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
	gui_labeled_bool_edit("Hide wall", &dat->ed_wall->entity->hidden, gui);
	if (gui_labeled_float_slider("UV offset x:", &dat->ed_wall->uv_offset.x, 1.0f, gui))
		applywallmesh(dat->ed_wall, dat->room, &ed->world);
	if (gui_labeled_float_slider("UV offset y:", &dat->ed_wall->uv_offset.y, 1.0f, gui))
		applywallmesh(dat->ed_wall, dat->room, &ed->world);
	if (gui_shortcut_button("Split wall", 'X', gui))
	{
		split_wall(dat->ed_wall, &dat->room, &ed->world);
		dat->ed_wall = NULL;
		//dat->rtm = rtm_none;
		return ;
	}
	gui_end(gui);
	/*dat->ed_wall->height += ed->hid.mouse.scroll_delta * 10;
	dat->ed_wall->height = ft_clamp(dat->ed_wall->height, 10, 200);*/
	if (ed->hid.mouse.scroll_delta != 0)
		init_roomwalls(&ed->world, dat->room);
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->ed_wall = NULL;
}

t_vector2	*closest_edge(t_room *room, t_vector3 rc)
{
	int			i;
	t_vector2	*edge;
	t_vector3	edge_3d;
	float		prev_dist;
	t_vector2	*looked;

	i = 0;
	looked = NULL;
	prev_dist = 1001.0f;
	while (i < room->edgecount)
	{
		edge = &room->edges[i];
		edge_3d = (t_vector3){edge->x, edge->y, room->height};
		if (vector3_sqr_dist(rc, edge_3d) < 1000.0f && vector3_sqr_dist(rc, edge_3d) < prev_dist)
		{
			looked = edge;
			prev_dist = vector3_sqr_dist(rc, edge_3d);
		}
		i++;
	}
	return (looked);
}

t_vector2	*lookedat_edge(t_room *room, t_vector2 snap)
{
	int			i;
	t_vector2	*edge;
	float		prev_dist;
	t_vector2	*looked;

	i = 0;
	looked = NULL;
	prev_dist = 1001.0f;
	while (i < room->edgecount)
	{
		edge = &room->edges[i];
		if (vector2_sqr_dist(snap, *edge) < 1000.0f && vector2_sqr_dist(snap, *edge) < prev_dist)
			looked = edge;
		i++;
	}
	return (looked);
}


void	recalculate_joined_rooms(t_editor *ed, t_room *room)
{
	t_list	*l;
	t_room	*other;
	int		i;

	l = ed->world.roomlist;

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
					init_roomwalls(&ed->world, other);
					make_areas(ed, ed->world.sdl, other);
					break ;
				}
				i++;
			}
		}
		l = l->next;
	}
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
			init_roomwalls(&ed->world, r);
			make_areas(ed, ed->world.sdl, r);
		}
		l = l->next;
	}
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
	//rc = raycast_DEPRECATED(ed); //TODO: replace with new raycast
	if (dat->raycastinfo.hit_entity != NULL)
	{
		rc = dat->raycastinfo.hit_pos;
		rc.x = vector2_snap((t_vector2){rc.x, rc.y}, 10).x;
		rc.y = vector2_snap((t_vector2){rc.x, rc.y}, 10).y;
	}
		
	
	gui_start(gui);
	gui_starthorizontal(gui);
	gui_label("Modifying: ", gui);
	gui_label(dat->room->name, gui);
	gui_endhorizontal(gui);
	gui_labeled_bool("Legal: ", isroomlegal(&ed->world, dat->room), gui);
	gui_labeled_int("Edges: ", dat->room->edgecount, gui);
	gui_labeled_int("Walls: ", dat->room->wallcount, gui);
	if (gui_shortcut_button("Force floor", 'F', gui))
	{
		dat->room->floorcount = 0;
		make_areas(ed, &sdl, dat->room);
	}

	/*int i = 0;
	char *ind_str;
	while (i < dat->room->edgecount)
	{
		t_vector3	ws;
		t_vector2	start;

		start = dat->room->edges[i];
		ws = (t_vector3){start.x, start.y, 0.0f};
		t_point ss = vector3_to_screenspace(ws, sdl);
		ind_str = ft_itoa(i);
		print_text(&sdl, ind_str, ss);
		sdl.render.gizmocolor = CLR_BLUE;
		render_gizmo(sdl, sdl.render, ws, 4);
		free (ind_str);
		i++;
	}*/
	
	if (gui_shortcut_button("Delete", KEYS_DELETEMASK, gui))
	{
		remove_room(&ed->world, dat->room);
		//list_remove(&ed->world.roomlist, dat->room, sizeof(t_room));
		//dat->wall.line.start = vector2_zero();
		//dat->wall.line.end = vector2_zero();
		dat->room = NULL;
		gui_end(gui);
		return ;
	}
	if (gui_shortcut_button("Split floor", 'Z', gui))
	{
		dat->rtm = rtm_split;
		/*dat->wall.line.start = vector2_zero();
		dat->splitwallcount = 0;
		dat->area = ft_memalloc(sizeof(t_floor_area));*/
		return ;
	}
	//int	prev = dat->floor_debugvalue;
	//gui_labeled_int_slider("FLOORDEBUG:", &dat->floor_debugvalue, 1, gui);
	//dat->floor_debugvalue = ft_clamp(dat->floor_debugvalue, 2, 5);
	gui_end(gui);
	snap = vector2_snap((t_vector2){rc.x, rc.y}, 10);
	look_wall = selectedwall(ed, sdl, dat->room);
	
	if (ed->hid.mouse.held == MOUSE_LEFT && dat->held_edge == NULL
		&& closest_edge(dat->room, rc) != NULL)
		dat->held_edge = closest_edge(dat->room, rc);
	if (closest_edge(dat->room, rc) != NULL)
		render_gizmo(sdl, sdl.render,
			vector3_add(
				vector2_to_vector3(*closest_edge(dat->room, rc)),
					(t_vector3){.z = dat->room->height}), 10);
	if (dat->held_edge != NULL/* && ed->hid.mouse.held == MOUSE_LEFT*/)
		applyedgedrag(dat->held_edge, (t_vector2){rc.x, rc.y}, dat->room, &ed->world);

	if (ed->hid.mouse.held == 0 && dat->held_edge != NULL)
	{
		recalculate_rooms(ed, dat->held_edge);
		dat->held_edge = NULL;
	}
		

	else if (dat->room->floorcount == 0)
	{
		make_areas(ed, &sdl, dat->room);
		//makefloor_room(ed, &sdl, dat->room, 2);
	}
		
	if (dat->held_edge == NULL && mouse_clicked(ed->hid.mouse, MOUSE_LEFT) && look_wall != NULL)
	{
		//force_mouseunlock(&ed->hid);
		dat->ed_wall = look_wall;
	}
	highlight_room(ed, &sdl, *dat->room, CLR_GREEN);
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		dat->room = NULL;
		dat->rtm = rtm_none;
	}
	if (ed->hid.mouse.scroll_delta != 0)
	{
		if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		{
			dat->room->height += ed->hid.mouse.scroll_delta * 10;
			dat->room->height = ft_clamp(dat->room->height, 0, 200);
			init_roomwalls(&ed->world, dat->room);
			make_areas(ed, &sdl, dat->room);
			recalculate_joined_rooms(ed, dat->room);
		}
		else
		{
			modifywallheights(dat->room, ed->hid.mouse.scroll_delta, &ed->world);
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
		dat->modroom_gui.minimum_size.x = 130;
		dat->modroom_gui.rect.size.x = 130;
	}
	if (dat->paint_gui.sdl == NULL)
	{
		dat->paint_gui = init_gui(sdl, &ed->hid, &ed->player, (t_point){20, 80}, "Paint rooms");
		//dat->paint_gui.dock = &dat->maingui;
		dat->modroom_gui.minimum_size.x = 130;
		dat->modroom_gui.minimum_size.y = 160;
		dat->modroom_gui.rect.size.x = 130;
		dat->modroom_gui.rect.size.y = 160;
	}
}

void	update_maingui(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui	*gui;
	t_vector3	look;

	look = raycast_DEPRECATED(ed);
	t_point	p;

	p.x = look.x;
	p.y = look.y;

	gui = &dat->maingui;
	gui_start(gui);
	//gui_labeled_point("DEBUG:", p, gui);
	if (gui_shortcut_button("New room", 'N', gui))
	{
		dat->room = ft_memalloc(sizeof(t_room));
		dat->room->walls = ft_memalloc(sizeof(t_wall) * 32);
		dat->room->edges = ft_memalloc(sizeof(t_vector2) * 32);
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
	t_room	*orig;
	t_vector2	*og_start;
	int			og_start_i;
	t_vector2	*og_end;
	int			og_end_i;


	t_vector2	*edge_og_new;
	int			ei;
	int	i;
	int	orig_wallcount;

	orig = data->room;
	
	room->wallcount = 0;
	edge_og_new = ft_memalloc(sizeof(t_vector2) * 32);
	ei = 0;
	og_start = find_edge_match(room->edges[0], orig);
	og_end = find_edge_match(room->edges[room->edgecount - 1], orig);
	og_start_i = find_edge_match_index(*og_start, orig);
	og_end_i = find_edge_match_index(*og_end, orig);
	if (og_start != NULL && og_end != NULL)
	{
		printf("found area start and end in other room \n");
	}
	/*if (og_start_i < og_end_i)
	{
		//target = 0;
		i = room->edgecount - 1;
		printf("FLIPPED CASE! only flip room->edges\n");
		exit(0);
	}*/
	i = 0;
	while (i < room->edgecount)
	{
		edge_og_new[ei++] = room->edges[i];
		printf("connect %i -> %i \n", i, i + 1);
		//room->walls[i].connection = true;
		//room->wallcount = i;
		i++;
		//printf("%i \n", i);
	}
	//edge_og_new[ei++] = room->edges[i];
	//orig_wallcount = room->wallcount;

	i = find_edge_match_index(room->edges[room->edgecount - 1], orig);
	int t_start = i;
	int target = find_edge_match_index(room->edges[0], orig);
	printf("adding edges from orig, range %i -> %i \n", i, target);
	room->height = orig->height + 20;
	while (i != target)
	{
		printf("adding %i from orig \n", i);
		if (!edge_exists(orig->edges[i], room))
			room->edges[room->edgecount++] = orig->edges[i];
		i++;
		if (i > orig->edgecount - 1)
			i = 0;
	}
	room->wallcount = room->edgecount;
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
	//Make walls for both, hide the walls for the one that is higher
	//make edgeconnections between these two rooms
	orig->edges = edge_og_new;
	orig->edgecount = ei;
	orig->wallcount = ei;
	i = 0;
	/*while (i < orig->wallcount)
	{
		orig->walls[i].height = 100;
		i++;
	}*/
	
	free_floor(&ed->world, orig);
	assign_default_floor_area(orig);
	init_roomwalls(&ed->world, orig);

	room->height = orig->height + 20;
	room = add_room_to_world(&ed->world, room);
	//init_roomwalls(&ed->world, room);
	room->ceiling_height = orig->ceiling_height - 20;

	init_roomwalls(&ed->world, orig);
	init_roomwalls(&ed->world, room);
	//free_floor(&ed->world, room);
	//assign_default_floor_area(room);
	//init_roomwalls(&ed->world, room);
	//make_areas(ed, ed->world.sdl, room);

	i = 0;
	

	data->room = room;
	data->rtm = rtm_modify;
}
void	room_tool_split(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui			*gui;
	t_vector3			snap;
	t_wall				*cur;
	t_vector2			v2snap;
	t_vector2			*edge;
	//static t_vector2	*new_edges;
	static	t_room		*new_room;
	//uint32_t			new_edgecount;

	if (new_room == NULL)
	{
		new_room = ft_memalloc(sizeof(t_room));
		new_room->walls = ft_memalloc(sizeof(t_wall) * 32);
		new_room->edges = ft_memalloc(sizeof(t_vector2) * 32);
		printf("allocated split room\n");
		//new_room->walls = ft_memalloc(sizeof(t_wall) * 32);
	}
	snap = raycast_DEPRECATED(ed);
	v2snap = vector2_snap((t_vector2){snap.x, snap.y}, 10);
	edge = lookedat_edge(dat->room, v2snap);
	if (edge != NULL)
	{
		v2snap = *edge;
		sdl->render.gizmocolor = CLR_RED;
		if (new_room->edgecount > 0)
			sdl->render.gizmocolor = CLR_GREEN;
		render_gizmo(*sdl, sdl->render, vector2_to_vector3(*edge), 10);
	}
	/*else
	{*/
		sdl->render.gizmocolor = AMBER_4;
		render_gizmo(*sdl, sdl->render, vector2_to_vector3(v2snap), 7);
	//}
	sdl->render.gizmocolor = AMBER_2;
	if (new_room->edgecount > 0)
		render_ray(sdl, vector2_to_vector3(new_room->edges[new_room->edgecount - 1]), vector2_to_vector3(v2snap));

	int	i = 0;
	
	while (i < new_room->edgecount)
	{
		if (i < new_room->edgecount - 1)
		{
			render_ray(sdl, vector2_to_vector3(new_room->edges[i]), vector2_to_vector3(new_room->edges[i + 1]));
		}
		render_gizmo(*sdl, sdl->render, vector2_to_vector3(new_room->edges[i]), 6);
		i++;
	}

	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
		if (new_room->edgecount > 0 || (new_room->edgecount == 0 && edge != NULL))
		{
			
			new_room->edges[new_room->edgecount] = v2snap;
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

void	room_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_roomtooldata	*dat;
	t_line			line;
	t_ray			ray;

	dat = (t_roomtooldata *)ed->tool->tooldata;
	ray.origin = ed->player.transform.position;
	ray.dir = ed->player.lookdir;
	raycast_new(ray, &dat->raycastinfo, &ed->world); //TODO: filter so this only hits rigid geometry


	t_list	*l;

	l = ed->world.roomlist;
	while (l != NULL)
	{
		highlight_room(ed, sdl, *(t_room *)l->content, AMBER_1);
		l = l->next;
	}
	if (dat->room == NULL && dat->rtm == rtm_none)
	{
		t_room	*hover;
		hover = selectedroom(ed, *sdl);
		if (hover != NULL)
		{
			highlight_room(ed, sdl, *hover, AMBER_3);
			print_text_boxed(sdl, hover->name, point_div(sdl->screensize, 2));
			if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
			{
				dat->room = hover;
				dat->rtm = rtm_modify;
				ed->hid.mouse.click_unhandled = false;
			}
		}
	}
	if (dat->room != NULL && dat->rtm == rtm_create)
	{
		createmode(ed, sdl, dat);
	}
	update_maingui(ed, sdl, dat);
	//TODO: make modifymode, paintmode and splitmode independent of dat->room
	//			require room selection for "split/add geometry" mode
	//TODO: show keybind for paintmode in maingui


	if (dat->rtm == rtm_modify && dat->room != NULL)
		modifymode(ed, *sdl, dat);
	if (dat->rtm == rtm_split && dat->room != NULL)
		room_tool_split(ed, sdl, dat);
	if (dat->rtm == rtm_paint/* && dat->room != NULL*/)
		room_tool_paint(ed, sdl, dat);
	if (dat->rtm == rtm_connect)
		room_tool_connect(ed, sdl, dat);
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
