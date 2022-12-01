/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugconsole.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:22:27 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/30 18:04:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	debugconsole_addmessage(t_debugconsole *console, char *str)
{
	console->messages[console->messagecount++] = ft_strdup(str);
	if (console->hidden)
		start_anim(&console->show_anim, anim_forwards);
}

void	update_debugconsole(t_debugconsole *console, t_sdlcontext *sdl, uint32_t delta)
{
	int	i;
	t_point	size = (t_point) {400, 16 * 12};
	t_point	origin;
	char	*str;

	if (console->hidden && !console->show_anim.active)
		return ;
		
	origin = point_sub(sdl->screensize, size);
	draw_rectangle_filled(*sdl, (t_rectangle){origin.x, origin.y, size.x, size.y}, CLR_DARKGRAY);
	draw_text_boxed(sdl, "DEBUGCONSOLE", point_add(origin, (t_point){5, 5}), sdl->screensize);
	if (console->show_anim.active)
	{
		int	sec = console->show_anim.lastframe / console->show_anim.framerate;
		str = ft_itoa(sec - ((console->show_anim.frame) / console->show_anim.framerate));
		draw_text_boxed(sdl, str, point_add(origin, (t_point){size.x - 20, 5}), sdl->screensize);
		free(str);
		update_anim(&console->show_anim, delta);
	}
	i = 0;
	while (i < console->messagecount)
	{
		draw_text_boxed(sdl, console->messages[i], point_add(origin, (t_point){5, ((i + 1) * 16) + 5}), sdl->screensize);
		//drawline(*sdl, point_add(origin, (t_point){0, i * 16}), point_add(origin, (t_point){size.x, i * 16}), CLR_DARKGRAY);
		i++;
	}
}