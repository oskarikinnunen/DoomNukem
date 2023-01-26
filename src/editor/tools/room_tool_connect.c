/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_connect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:52:00 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/26 13:32:40 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"

void    free_roomwalls(t_world *world, t_room *room)
{
	int     i;
	t_wall  *w;

	i = 0;
	while (i < room->wallcount)
	{
		w = &room->walls[i];
		printf("destroying wall %i \n", i);
		destroy_entity(world, w->entity);
		w->entity = NULL;
		i++;
	}
	//free(room->walls);
	room->wallcount = 0;
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

void	room_tool_bucket()
{
	
}
