/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_raycast_room.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 17:08:01 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 14:39:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

bool	area_has_wall_entity(t_entity *entity, t_area *area)
{
	int	i;

	i = 0;
	while (i < area->wallcount)
	{
		if (area->walls[i].entity == entity)
			return (true);
		i++;
	}
	return (false);
}

bool	area_has_floor_entity(t_entity *entity, t_area *area)
{
	int	i;

	i = 0;
	while (i < area->floorcount)
	{
		if (area->floors[i].entity == entity)
			return (true);
		i++;
	}
	return (false);
}

bool	area_has_ceiling_entity(t_entity *entity, t_area *area)
{
	int	i;

	i = 0;
	while (i < area->ceilingcount)
	{
		if (area->ceilings[i].entity == entity)
			return (true);
		i++;
	}
	return (false);
}

t_area	*get_raycast_room(t_raycastinfo info, t_world *world)
{
	t_list	*l;
	t_area	*room;

	if (info.hit_entity == NULL)
		return (NULL);
	l = world->arealist;
	while (l != NULL)
	{
		room = (t_area *)l->content;
		if (area_has_ceiling_entity(info.hit_entity, room)
			||area_has_floor_entity(info.hit_entity, room)
			|| area_has_wall_entity(info.hit_entity, room))
		{
			return (room);
		}
		l = l->next;
	}
	return (NULL);
}

void	room_tool_raycast(t_editor *ed, t_roomtooldata *dat)
{
	t_ray			ray;

	ray.origin = ed->player.headposition;
	ray.dir = ed->player.lookdir;
	ft_bzero(&dat->raycastinfo, sizeof(t_raycastinfo));
	raycast_new(ray, &dat->raycastinfo, &ed->world);
}