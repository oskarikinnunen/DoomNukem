/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_connect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 08:52:00 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 21:56:01 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"

void	free_roomwalls(t_world *world, t_area *room)
{
	int		i;
	t_wall	*w;

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
}
