/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_save_amap.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 19:59:44 by raho              #+#    #+#             */
/*   Updated: 2023/03/22 20:57:25 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_io.h"
#include "doomnukem.h"

void	world_save_amap(char *level, t_world world)
{
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"saving .amap to", level, NULL});
	world_remove_all_room_entities(&world);
	world_sanitize_all_room_pointers(&world);
	save_chunk(level, "AREA", world.arealist);
}
