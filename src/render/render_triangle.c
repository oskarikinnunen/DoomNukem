/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_triangle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:16:50 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 15:49:53 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "bresenham.h"

static void unlit_render(t_sdlcontext *sdl, t_render *render)
{
	int index;

	index = 0;
	if (render->img->transparency)
	{
		while (index < render->screenspace_ptri_count)
		{
			render_triangle_transparent(sdl, render, index);
			index++;
		}
	}
	else
	{
		while (index < render->screenspace_ptri_count)
		{
			render_triangle_unlit(sdl, render, index);
			index++;
		}
	}
}

void render_buffer_triangles(t_sdlcontext *sdl, t_render *render)
{
	int index;

	index = 0;
	if (render->lightmode == lm_unlit || !sdl->lighting_toggled)
		unlit_render(sdl, render);
	else if (render->lightmode == lm_lit)
	{
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
		drawline(*sdl, vector2_to_point(t1.p[0]), vector2_to_point(t1.p[1]), render->gizmocolor);
		drawline(*sdl, vector2_to_point(t1.p[1]), vector2_to_point(t1.p[2]), render->gizmocolor);
		drawline(*sdl, vector2_to_point(t1.p[2]), vector2_to_point(t1.p[0]), render->gizmocolor);
		index++;
	}
}

void render_buffer(t_sdlcontext *sdl, t_render *render)
{
	if (!render->wireframe && (render->map.texture != NULL || render->img != NULL) && !sdl->global_wireframe)
		render_buffer_triangles(sdl, render);
	if (render->wireframe/* || sdl->global_wireframe*/)
		render_buffer_triangle_wireframes(sdl, render);
	/*if (render->img == NULL)
		render_solid_triangle(sdl, render);*/
	render->rs.triangle_count += render->screenspace_ptri_count;
}
