/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_presets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:40:45 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 15:28:41 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

void	gui_preset_scale_and_rotate(t_transform *t, t_autogui *gui)
{
	bool	modified;

	gui_labeled_vector3_slider("Rotation:", &t->rotation, 0.1f, gui);
	gui_labeled_vector3_slider("Scale:", &t->scale, 0.1f, gui);
	modified = gui_labeled_float_slider(\
			"Scale (Locked xyz):", &t->scale.z, 0.1f, gui);
	if (modified)
	{
		t->scale.x = t->scale.z;
		t->scale.y = t->scale.z;
	}
}

void	gui_preset_transform(t_transform *t, t_autogui *gui)
{
	bool	modified;

	gui_labeled_vector3_slider("Position:", &t->position, 1.0f, gui);
	gui_labeled_vector3_slider("Rotation:", &t->rotation, 0.1f, gui);
	gui_labeled_vector3_slider("Scale:", &t->scale, 0.1f, gui);
	modified = gui_labeled_float_slider(\
			"Scale (Locked xyz):", &t->scale.z, 0.1f, gui);
	if (modified)
	{
		t->scale.x = t->scale.z;
		t->scale.y = t->scale.z;
	}
}
