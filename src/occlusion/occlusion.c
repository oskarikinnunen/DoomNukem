/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:40:03 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/25 18:59:30 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int32_t		get_id(t_world *world)
{
	t_list		*l;
	t_entity	*ent;
	t_wall		wall;
	int32_t		id;
	int			i;
	
	id = 0;
	l = world->roomlist;
	while (l != NULL)
	{
		t_room room = *(t_room *)l->content;
		i = 0;
		while (i < room.wallcount)
		{
			id++;
			i++;
		}
		i = 0;
		while (i < room.floorcount)
		{
			id++;
			i++;
		}
		l = l->next;
	}
	l = world->entitylist;
	while (l != NULL)
	{
		id++;
		l = l->next;
	}
	id++;
	return(id);
}

static void update_room_occlusion(t_sdlcontext sdl, t_render *render, t_room *room)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		update_occlusion_culling(sdl, render, &room->walls[i].entity);
		i++;
	}
}

void update_occlusion(t_sdlcontext sdl, t_render *render)
{
	t_list		*l;
	t_entity	*ent;
	t_wall		wall;
	int			i;
	
	l = render->world->roomlist;
	while (l != NULL)
	{
		update_room_occlusion(sdl, render, (t_room *)l->content);
		l = l->next;
	}
	l = render->world->entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		update_occlusion_culling(sdl, render, ent);
		
		l = l->next;
	}
	//update_peripheral_culling(sdl, render, entity);//if false
	//update_occlusion_culling(sdl, render, entity);
}

bool is_entity_culled(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	if (is_entity_frustrum_culled(sdl, render, entity) == false)
	{
		if (is_entity_peripheral_culled(sdl, render, entity) == false)
		{
			return (is_entity_occlusion_culled(sdl, render, entity));
		}
	}
	return (true);
}
