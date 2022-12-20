/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_connect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:52:00 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/20 10:37:54 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"


t_vector2   vector2_flipxy(t_vector2 vec)
{
    return ((t_vector2) {-vec.y, vec.x});
}

void    free_roomwalls(t_world *world, t_room *room)
{
    int     i;
    t_wall  *w;

    i = 0;
    while (i < room->wallcount)
    {
        w = &room->walls[i];
        destroy_entity(world, w->entity);
        i++;
    }
    free(room->walls);
}

void    set_nullentities(t_wall **ptr, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        (*ptr)[i].entity = NULL;
        i++;
    }
}

t_wall  *insertdoorway(t_world *world, t_wall *current, t_room *room)
{
	int	i;
	t_wall	*newptr;
    t_wall  new[3];
    t_vector2   norm;

	i = 0;
	newptr = ft_memalloc((room->wallcount + 2) * sizeof(t_wall));
    ft_bzero(new, sizeof(t_wall [3]));
    
    norm = vector2_flipxy(vector2_sub(current->line.end, current->line.start));
    norm = vector2_mul(vector2_normalise(norm), 10.0f);
    new[0].line = current->line;
    new[0].line.end = vector2_add(new[0].line.start, norm);
    new[0].height = current->height;
    new[2].height = current->height;
    new[2].line.end = current->line.end;
    new[2].line.start = vector2_add(current->line.end, norm);
    
    new[1].line.start = new[0].line.end;
    new[1].line.end = new[2].line.start;
    new[1].height = current->height;
    new[1].connection = true;
    new[1].disabled = true;

	while (i < room->wallcount)
	{
		if (current == &room->walls[i])
		{
			ft_memcpy(newptr, room->walls, sizeof(t_wall) * i);
			printf("copied %i walls before doorwalls \n", i);
			ft_memcpy(newptr + i, new, sizeof(t_wall) * 3);
			ft_memcpy(newptr + i + 3, room->walls + i + 1, sizeof(t_wall) * (room->wallcount - i - 1));
			printf("copied %i walls after doorwalls \n", (room->wallcount - i - 1));
			free_roomwalls(world, room);
            set_nullentities(&newptr, room->wallcount + 2);
			room->walls = newptr;
			room->wallcount = room->wallcount + 2;
			return &room->walls[i + 1];
		}
		i++;
	}
	free(newptr);
    return (NULL);
}

void    adjust_door_depth(t_wall *wall, t_room *room, t_world *world, int depth)
{
    int         i;
    t_vector2   norm;
    t_line      *left;
    t_line      *right;

    depth = -depth;
    i = 0;
    while (i < room->wallcount)
    {
        if (wall == &room->walls[i])
            break;
        i++;
    }
    left = &room->walls[i - 1].line;
    right = &room->walls[i + 1].line;
    norm = vector2_flipxy(vector2_sub(left->start, right->end));
    norm = vector2_mul(vector2_normalise(norm), depth);
    left->end = vector2_add(left->start, norm);
    right->start = vector2_add(right->end, norm);
    room->walls[i].line.start = left->end;
    room->walls[i].line.end = right->start;
    applywallmesh(&room->walls[i - 1]);
    applywallmesh(&room->walls[i]);
    applywallmesh(&room->walls[i + 1]);
}

void	room_tool_connect(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui	*gui;
	t_list		*l;
	t_wall		*select;
    static  int cdepth = 10;

	gui = &dat->modroom_gui;
	l = ed->world.roomlist;
	select = NULL;
	while (l != NULL)
	{
		highlight_roomwalls(ed, *sdl, *(t_room *)l->content, AMBER_2);
		if (select == NULL)
        {
            dat->room = (t_room *)l->content;
            select = selectedwall(ed, *sdl, (t_room *)l->content);
        }
			
		l = l->next;
	}
	if (select != NULL)
		highlight_entity(ed, *sdl, select->entity, AMBER_3);
	if (select != NULL && mouse_clicked(ed->hid.mouse, MOUSE_LEFT))
	{
        if (select->connection)
            dat->ed_wall = select;
        else
        {
            dat->ed_wall = insertdoorway(&ed->world, select, dat->room);
            init_roomwalls(&ed->world, dat->room);
        }
        select = NULL;
	}
		
	gui_start(gui);
    if (dat->room != NULL)
        gui_labeled_int("Room walls:", dat->room->wallcount, gui);
	if (dat->ed_wall != NULL)
	{
		highlight_entity(ed, *sdl, dat->ed_wall->entity, AMBER_4);
        if (dat->ed_wall->connection)
        {
            if (gui_labeled_int_slider("Door depth:", &cdepth, 1.0f, gui) || ed->hid.mouse.scroll_delta != 0)
            {
                cdepth += ed->hid.mouse.scroll_delta * 3;
                cdepth = ft_clamp(cdepth, 10, 100);
                adjust_door_depth(dat->ed_wall, dat->room, &ed->world, cdepth);
                //init_roomwalls(&ed->world, dat->room);
            }
            if (gui_shortcut_button("Link new room", 'l', gui))
            {
                dat->room = ft_memalloc(sizeof(t_room));
		        ft_bzero(dat->room->name, 32);
		        snprintf(dat->room->name, 32, "room(%i)", ft_listlen(ed->world.roomlist));
		        dat->room->walls = ft_memalloc(sizeof(t_wall) * 32);
                dat->room->walls[0] = *dat->ed_wall;
                dat->room->walls[0].entity = ft_memalloc(sizeof(t_entity));
                dat->room->walls[0].disabled = true;
                dat->room->wallcount = 1;
                dat->wall.line.start = dat->ed_wall->line.end;
                dat->rtm = rtm_create;
                dat->ed_wall = NULL;
                return ;
            }
        }
	}
	else
		gui_label("Select connection point", gui);
	gui_end(gui);
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
	{
		dat->rtm = rtm_none;
		dat->room = NULL;
	}
		
}