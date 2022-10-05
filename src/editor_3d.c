/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 13:37:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/05 13:57:03 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	editor3d(t_sdlcontext sdl, t_editor *ed_ptr)
{
	float		f_view[2];
	float		lerp;
	t_editor	ed;	//This is here just cuz i'm lazy

	ed = *ed_ptr;
	lerp = ed.transition.lerp;
	v2mul_to_f2(ed.mousedrag[1].pos, 0.005f, f_view);
	f2mul(f_view, lerp);
	ft_memcpy(ed.grid_fdf.view, f_view, sizeof(float [2]));
	ft_memcpy(ed.walls_fdf.view, f_view, sizeof(float [2]));
	ed_ptr->threedee_zoom += (float)ed.mouse.scroll_delta * 0.01f;
	ed.grid_fdf.zoom = ed.threedee_zoom;
	ed.walls_fdf.zoom = ed.threedee_zoom;
	if (ed.transition.active)
	{
		ed.grid_fdf.zoom = (1.0f - lerp) + (lerp * ed.threedee_zoom);
		ed.walls_fdf.zoom = (1.0f - lerp) + (lerp * ed.threedee_zoom);
	}
	v2mul_to_f2(ed.mousedrag->pos, 1.0f / ed.grid_fdf.zoom, f_view);
	ft_memcpy(ed.grid_fdf.offset, f_view, sizeof(float [2]));
	ft_memcpy(ed.walls_fdf.offset, f_view, sizeof(float [2]));
	fdf_draw_wireframe(&ed.grid_fdf);
	fdf_draw_wireframe(&ed.walls_fdf);
	imgtoscreen(sdl.surface->pixels, &ed.grid_fdf.img);
	imgtoscreen(sdl.surface->pixels, &ed.walls_fdf.img);
}
