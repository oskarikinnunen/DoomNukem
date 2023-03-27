/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renders_world_triangles_to_screen_triangles        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 15:59:55 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/26 20:29:32 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

inline static void	clip_viewspace_triangle_to_buffer(
	t_sdlcontext sdl, t_render *render, t_world_triangle world_tri)
{
	t_screen_triangle	screen_triangle_buffer[32];
	t_world_triangle	clipped[2];
	int					nclippedtriangles;
	int					i;
	t_point				start;

	nclippedtriangles = clip_triangle_plane((t_vector3){0.0f, 0.0f, 0.1f}, \
	(t_vector3){0.0f, 0.0f, 1.0f}, world_tri, clipped);
	i = 0;
	while (i < nclippedtriangles)
	{
		screen_triangle_buffer[0] = world_triangle_to_screen_triangle(
				render->camera.mat_proj, clipped[i], sdl);
		start = clip_screen_triangle_against_screen_edge(
				screen_triangle_buffer, render->screen_edge);
		ft_memcpy(&render->screenspace_ptris[render->screenspace_ptri_count], \
		&screen_triangle_buffer[start.x], \
		sizeof(t_screen_triangle) * (start.y - start.x));
		render->screenspace_ptri_count += start.y - start.x;
		i++;
	}
}

void	render_world_triangle_buffer_to_screen_triangle(
	t_render *render, t_sdlcontext sdl)
{
	t_world_triangle	world_tri;
	int					i;

	if (!render->world_triangles)
		return ;
	i = render->start_index;
	while (i <= render->end_index)
	{
		world_tri = render->world_triangles[i];
		if (!is_triangle_backface(world_tri, render))
		{
			world_tri = triangle_to_viewspace(
					world_tri, render->camera.mat_view);
			clip_viewspace_triangle_to_buffer(sdl, render, world_tri);
		}
		i++;
	}
}
