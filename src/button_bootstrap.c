/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_bootstrap.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 18:31:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/27 18:54:13 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

void	buttoncreator(t_editor *ed, t_sdlcontext sdl)
{
	static int			state;
	static t_guibutton	button;

	if (ed->mouse.click_unhandled)
	{
		printf("buttoncreator got click event \n");
		ed->mouse.click_unhandled = false;
		if (state == 0)
		{
			button.rect.position = ed->mouse.pos;
		}
		else
		{
			button.rect.size = point_sub(button.rect.position, ed->mouse.pos);
		}
		state = !state;
	}
	drawrectangle(sdl, button.rect, CLR_GREEN);
}
