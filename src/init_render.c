/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/30 17:46:05 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

t_render	init_render(t_sdlcontext sdl, struct s_world *world)
{
	t_render	render;

	bzero(&render, sizeof(t_render));
	render.matworld = matrix_makeidentity();
	render.matproj = matrix_makeprojection(90.0f, (float)sdl.window_h / (float)sdl.window_w, 2.0f, 1000.0f);
	render.calc_triangles = malloc(sizeof(t_triangle) * 10000);
	render.draw_triangles = malloc(sizeof(t_triangle) * 10000);
	render.q = malloc(sizeof(t_quaternion) * 10000); //TODO: should be multiplied by the largest obj vertex count
	render.debug_img = get_image_by_name(sdl, "");
	render.vtarget = vector3_add(render.position, render.lookdir);
	render.matcamera = matrix_lookat(render.position, render.vtarget, (t_vector3){0, 0, 1});
	render.matview = matrix_quickinverse(render.matcamera);
	render.world = world;
	render.occlusion.occlusion = true;
	render.occlusion.cull_box = false;
	render.occlusion.occluder_box = false;
	/*temp testing render loop with .bot .wall .item*/
	return(render);
}

void	render_start(t_render *render)
{
	render->vtarget = vector3_add(render->position, render->lookdir);
	render->matcamera = matrix_lookat(render->position, render->vtarget, (t_vector3){0, 0, 1});
	render->matview = matrix_quickinverse(render->matcamera);
}

void	free_render(t_render render)
{
	free(render.calc_triangles);
	free(render.draw_triangles);
	free(render.q);
}