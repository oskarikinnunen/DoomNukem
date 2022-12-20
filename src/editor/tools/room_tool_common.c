/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_common.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:53:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/20 08:57:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void highlight_entity(t_editor *ed, t_sdlcontext sdl, t_entity *entity, uint32_t color)
{
	ed->render.wireframe = true;
	ed->render.gizmocolor = color;
	render_entity(sdl, &ed->render, entity);
	ed->render.wireframe = false;
}

void highlight_room(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color)
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

void highlight_roomwalls(t_editor *ed, t_sdlcontext sdl, t_room room, uint32_t color)
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
	ed->render.wireframe = false;
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

t_room *selectedroom(t_editor *ed, t_sdlcontext sdl)
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