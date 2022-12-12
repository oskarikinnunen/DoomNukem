/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/12 17:21:04 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

t_render	init_render(t_sdlcontext sdl, struct s_world *world)
{
	t_render	render;

	bzero(&render, sizeof(t_render));
	render.camera.lookdir = (t_vector3){-0.630105, 0.650868, -0.423484};
	render.camera.position = (t_vector3){1208.355713, 912.929382, 154.534698};
	render.camera.matworld = matrix_makeidentity();
	render.camera.matproj = matrix_makeprojection(90.0f, (float)(sdl.window_h * sdl.resolution_scaling) / (float)(sdl.window_w * sdl.resolution_scaling), 2.0f, 1000.0f);
	render.occ_calc_tris = malloc(sizeof(t_triangle) * 10000);
	render.occ_draw_tris = malloc(sizeof(t_triangle) * 10000);
	render.worldspace_ptris = malloc(sizeof(t_point_triangle) * 10000);
	render.screenspace_ptris = malloc(sizeof(t_point_triangle) * 10000);
	render.q = malloc(sizeof(t_quaternion) * 10000); //TODO: should be multiplied by the largest obj vertex count
	render.debug_img = get_image_by_name(sdl, "");
	render.camera.vtarget = vector3_add(render.camera.position, render.camera.lookdir);
	render.camera.matcamera = matrix_lookat(render.camera.position, render.camera.vtarget, (t_vector3){0.0f, 0.0f, 1.0f});
	render.camera.matview = matrix_quickinverse(render.camera.matcamera);
	render.occlusion.occlusion = false;
	render.occlusion.cull_box = false;
	render.occlusion.occluder_box = false;
	bake_lighting(&render, world);
	return(render);
}

void	render_start(t_render *render)
{
	render->camera.vtarget = vector3_add(render->camera.position, render->camera.lookdir);
	render->camera.matcamera = matrix_lookat(render->camera.position, render->camera.vtarget, (t_vector3){0.0f, 0.0f, 1.0f});
	render->camera.matview = matrix_quickinverse(render->camera.matcamera);
}

void update_render(t_render *render, t_player *player)
{
	render->camera.lookdir = player->lookdir;
	render->camera.position = player->transform.position;
}

void	free_render(t_render render)
{
	free(render.occ_calc_tris);
	free(render.occ_draw_tris);
	free(render.q);
}