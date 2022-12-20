/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 13:56:06 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/12 14:23:57 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "editor_tools.h"
#include "tools/roomtool.h"

/*static void highlight_room(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color)
{
	int	i;

	i = 0;
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	while (i < room.wallcount)
	{
		render_entity(sdl, &ed->render, room.walls[i].entity);
		i++;
	}
	i = 0;
	while (i < room.floorcount)
	{
		render_entity(sdl, &ed->render, room.floors[i].entity);
		i++;
	}
	ed->render.wireframe = false;
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
		//if (selectedfloor(ed, sdl, r) != NULL)
		//	return (r);
		l = l->next;
	}
	return (NULL);
}

void	update_maingui(t_editor *ed, t_sdlcontext *sdl, t_roomtoolndata *dat)
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
		dat->room = ft_memalloc(sizeof(t_room2));
		ft_bzero(roomname, 64);
		snprintf(roomname, 64, "room(%i)", ft_listlen(ed->world.roomlist));
		ft_strcpy(dat->room->name, roomname);
		dat->room->walls = ft_memalloc(sizeof(t_wall2) * 32);
		dat->rtm = rtm_create;
		force_mouselock(&ed->hid);
	}
	gui_end(gui);
}


void	room_tooln_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_roomtoolndata	*dat;

	dat = ed->tool->tooldata;
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

void	room_tooln_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_roomtoolndata	*dat;

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

t_tool	*get_room_tool()
{
	static t_tool	tool
	= {
		.update = room_tooln_update,
		.init	= room_tooln_init
	};
	t_roomtoolndata		*dat;

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
	return (&tool);
}*/