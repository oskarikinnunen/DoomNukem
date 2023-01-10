/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_triangle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2023/01/10 09:33:12 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

void render_buffer_triangles(t_sdlcontext *sdl, t_render *render)
{
	int index;

	index = 0;
	if (render->lightmode == lm_unlit || !sdl->lighting_toggled)
	{
		index = 0;
		while (index < render->screenspace_ptri_count)
		{
			render_triangle_unlit(sdl, render, index);
			index++;
		}
	}
	else if (render->lightmode == lm_lit)
	{
		index = 0;
		while (index < render->screenspace_ptri_count)
		{
			render_triangle_lit(sdl, render, index);
			index++;
		}
	}
}

void render_buffer_triangle_wireframes(t_sdlcontext *sdl, t_render *render)
{
	int	index;

	index = 0;
	while (index < render->screenspace_ptri_count)
	{
		t_point_triangle t1 = wf_tri(render->screenspace_ptris[index], sdl->resolution_scaling);
		drawline(*sdl, t1.p[0], t1.p[1], render->gizmocolor);
		drawline(*sdl, t1.p[1], t1.p[2], render->gizmocolor);
		drawline(*sdl, t1.p[2], t1.p[0], render->gizmocolor);
		index++;
	}
}

//TODO: Gona be deprecated one year
void render_solid_triangle(t_sdlcontext *sdl, t_render *render)
{
	int	index;

	index = 0;
	while (index < render->screenspace_ptri_count)
	{
		t_img	img;

		img.data = (uint32_t [1]) {render->screenspace_ptris[index].clr};
		img.size.x = 1;
		img.size.y = 1;
		img.length = 1;
		render->map.data = img.data;
		render->map.img_size = img.size;
		//render_triangle_wrap(sdl, render, index);
		render->img = NULL;
		index++;
	}
}

void render_buffer(t_sdlcontext *sdl, t_render *render)
{
	if (!render->wireframe && (render->map.data != NULL || render->img != NULL))
		render_buffer_triangles(sdl, render); //thread start
	if (render->wireframe)
		render_buffer_triangle_wireframes(sdl, render);
	//if (render->img == NULL)
	//	render_solid_triangle(sdl, render);
	render->rs.triangle_count += render->screenspace_ptri_count;
}
