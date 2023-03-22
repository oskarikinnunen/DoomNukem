/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_connect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:52:00 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 14:53:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"

void	free_roomwalls(t_world *world, t_area *room)
{
	int     i;
	t_wall  *w;

	i = 0;
	while (i < room->wallcount)
	{
		w = &room->walls[i];
		if (w->entity != NULL)
		{
			destroy_entity(world, w->entity);
			w->entity = NULL;
		}
		i++;
	}
	//free(room->walls);
	//room->wallcount = 0;
}
