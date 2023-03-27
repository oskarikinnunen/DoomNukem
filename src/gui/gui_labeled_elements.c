/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_labeled_elements.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:40:13 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:15:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

void	gui_labeled_point(char *str, t_point point, t_autogui *gui)
{
	gui_start_horizontal(gui);
	gui_label(str, gui);
	gui_int(point.x, gui);
	gui_int(point.y, gui);
	gui_end_horizontal(gui);
}

void	gui_labeled_int(char *str, int i, t_autogui *gui)
{
	gui_start_horizontal(gui);
	gui_label(str, gui);
	gui_int(i, gui);
	gui_end_horizontal(gui);
}

void	gui_labeled_bool(char *str, bool b, t_autogui *gui)
{
	gui_start_horizontal(gui);
	gui_label(str, gui);
	if (b)
		gui_label("True", gui);
	else
		gui_label("False", gui);
	gui_end_horizontal(gui);
}

//TODO: 'modified' is always false
bool	gui_labeled_bool_edit(char *str, bool *b, t_autogui *gui)
{
	bool	modified;

	modified = false;
	gui_start_horizontal(gui);
	gui_label(str, gui);
	if (gui_highlighted_button_if("True", gui, *b))
		*b = true;
	if (gui_highlighted_button_if("False", gui, !*b))
		*b = false;
	gui_end_horizontal(gui);
	return (modified);
}
