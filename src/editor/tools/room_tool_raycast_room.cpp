/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_raycast_room.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 17:08:01 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 20:46:13 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"
#include "raycast.h"

bool	area_has_wall_entity(t_entity *entity, t_area *area)
{
	int	i;

	i = 0;
	while (i < area->wall_count)
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
	while (i < area->floor_count)
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
	while (i < area->ceiling_count)
	{
		if (area->ceilings[i].entity == entity)
			return (true);
		i++;
	}
	return (false);
}

t_area	*get_raycast_room(t_raycast_info info, t_world *world)
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

void	room_tool_raycast(t_editor *ed, t_room_tool_data *dat)
{
	t_ray			ray;

	ray.origin = ed->player.head_position;
	ray.dir = ed->player.look_dir;
	ft_bzero(&dat->raycast_info, sizeof(t_raycast_info));
	raycast(ray, &dat->raycast_info, &ed->world);
}
