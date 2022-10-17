/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_buttons.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 14:35:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/17 20:20:35 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shapes.h"
#include "doomnukem.h"

t_rectangle	*get_editor_buttons()
{
	static t_rectangle buttons[3] =
	{
		{WINDOW_W - 50, 10, 40, 40},
		{WINDOW_W - 50, 60, 40, 40},
		{WINDOW_W - 50, 110, 40, 40}
	};

	return	(buttons);
}

void	draw_editor_buttons(t_sdlcontext sdl)
{
	t_rectangle	*buttons;
	int			i;

	buttons = get_editor_buttons();
	i = 0;
	while (i < 3)
	{
		drawrectangle(sdl.surface->pixels, buttons[i], CLR_GRAY);
		i++;
	}
}