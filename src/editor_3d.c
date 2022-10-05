/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 13:37:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/05 13:43:12 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	editor3d(t_sdlcontext sdl, t_editor *ed_ptr)
{
	float		v[2];
	float		animlerp;
	t_editor	ed;

	ed = *ed_ptr; //This is here just cuz i'm lazy
	animlerp = ((float)ed.transition.frame / (float)ed.transition.lastframe);
	v2mul_to_f2(ed.mousedrag[1].pos, 0.005f, v);
	v[X] = v[X] * animlerp;
	v[Y] = v[Y] * animlerp;
	ft_memcpy(ed.grid_fdf.view, v, sizeof(float [2]));
	ft_memcpy(ed.walls_fdf.view, v, sizeof(float [2]));
	ed_ptr->threedee_zoom += (float)ed.mouse.scroll_delta * 0.01f;
	ed.grid_fdf.zoom = ed.threedee_zoom;
	ed.walls_fdf.zoom = ed.threedee_zoom;
	if (ed.transition.active)
	{
		ed.grid_fdf.zoom = (1.0f - animlerp) + (animlerp * ed.threedee_zoom);
		ed.walls_fdf.zoom = (1.0f - animlerp) + (animlerp * ed.threedee_zoom);
	}
	v2mul_to_f2(ed.mousedrag->pos, 1.0f / ed.grid_fdf.zoom, v);
	ft_memcpy(ed.grid_fdf.offset, v, sizeof(float [2]));
	ft_memcpy(ed.walls_fdf.offset, v, sizeof(float [2]));
	fdf_draw_wireframe(&ed.grid_fdf);
	fdf_draw_wireframe(&ed.walls_fdf);
	imgtoscreen(sdl.surface->pixels, &ed.grid_fdf.img);
	imgtoscreen(sdl.surface->pixels, &ed.walls_fdf.img);
}
