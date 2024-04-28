/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_player_spawn2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:56:17 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 17:57:42 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Used to edit component values */
void	comp_playerspawn_gui_edit(t_entity *entity,
									t_autogui *gui, t_world *world)
{
	t_playerspawn	*playerspawn;

	playerspawn = entity->component.data;
	gui_label("GUI for playerspawn not implemented", gui);
	if (playerspawn == NULL)
		return ;
}
