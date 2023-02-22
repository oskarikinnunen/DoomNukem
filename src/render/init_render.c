/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/16 17:46:37 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

t_render	init_render(t_sdlcontext sdl)
{
	t_render	render;
	t_vector3	vtarget;
	t_mat4x4	matcamera;

	bzero(&render, sizeof(t_render));
	bzero(&render.camera, sizeof(t_camera));
	render.camera.lookdir = (t_vector3){0};
	render.camera.position = (t_vector3){0};
	render.camera.matproj = matrix_makeprojection(90.0f, (float)(sdl.window_h * sdl.resolution_scaling) / (float)(sdl.window_w * sdl.resolution_scaling), 2.0f, 1000.0f);
	render.screenspace_ptris = malloc(sizeof(t_point_triangle) * 10000);
	render.q = malloc(sizeof(t_quaternion) * 10000); //TODO: should be multiplied by the largest obj vertex count
	render.debug_img = get_image_by_name(sdl, "");
	vtarget = vector3_add(render.camera.position, render.camera.lookdir);
	matcamera = matrix_lookat(render.camera.position, vtarget, (t_vector3){0.0f, 0.0f, 1.0f});
	render.camera.matview = matrix_quickinverse(matcamera);
	render.occlusion.occlusion = false;
	render.occlusion.occluder_box = false;
	render.occlusion.draw_occlusion = false;
	return(render);
}

//make this function reutrn matview and take position/lookdir
void	render_start(t_camera *camera)
{
	t_vector3	vtarget;
	t_mat4x4	matcamera;

	vtarget = vector3_add(camera->position, camera->lookdir);
	matcamera = matrix_lookat(camera->position, vtarget, (t_vector3){0.0f, 0.0f, 1.0f});
	camera->matview = matrix_quickinverse(matcamera);
}

void update_render(t_render *render, t_player *player)
{
	render->camera.lookdir = player->lookdir;
	render->camera.position = player->headposition;
}

void	free_render(t_render render)
{
	if (render.screenspace_ptris != NULL)
		free(render.screenspace_ptris);
	if (render.q != NULL)
		free(render.q);
}