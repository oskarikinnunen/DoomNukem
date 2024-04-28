/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_connect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:52:00 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 18:49:11 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"

void	free_roomwalls(t_world *world, t_area *room)
{
	int		i;
	t_wall	*w;

	i = 0;
	while (i < room->wall_count)
	{
		w = &room->walls[i];
		if (w->entity != NULL)
		{
			free_object(&w->entity->obj);
			destroy_entity(world, w->entity);
			w->entity = NULL;
		}
		i++;
	}
}
