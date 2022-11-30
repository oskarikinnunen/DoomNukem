/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:40:03 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/30 18:46:32 by vlaine           ###   ########.fr       */
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
		if (render->occlusion.occluder_box == true)
			draw_edges(sdl, render, &room->walls[i].entity, CLR_BLUE);
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

	if (render->occlusion.occlusion == false)
		return;
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
		if (render->occlusion.occluder_box == true)
			draw_edges(sdl, render, ent, CLR_BLUE);
		update_occlusion_culling(sdl, render, ent);
		l = l->next;
	}
}

bool is_entity_culled(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	if (render->occlusion.occlusion == false)
		return(false);
	if (is_entity_frustrum_culled(sdl, render, entity) == false)
	{
		if (is_entity_peripheral_culled(sdl, render, entity) == false)
		{
			if (entity->occlusion.type == oc_cull || entity->occlusion.type == oc_occlude_and_cull)
			{
				if (is_entity_occlusion_culled(sdl, render, entity) == false)
				{
					if (render->occlusion.cull_box == true)
						draw_wireframe(sdl, render, entity, CLR_GREEN);
					return(false);
				}
				else
				{
					if (render->occlusion.cull_box == true)
						draw_wireframe(sdl, render, entity, CLR_RED);
					render->rs.occlusion_cull_amount++;
					return(true);
				}
			}
			else
			{
				return(false);
			}
		}
	}
	return (true);
}
