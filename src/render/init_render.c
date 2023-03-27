/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 20:46:13 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

t_render	init_render(t_sdlcontext sdl)
{
	t_render	render;
	t_vector3	vtarget;
	t_mat4x4	matcamera;

	ft_bzero(&render, sizeof(t_render));
	ft_bzero(&render.camera, sizeof(t_camera));
	render.camera.aspect_ratio = (float)(sdl.window_h) / (float)(sdl.window_w);
	render.camera.mat_proj = matrix_make_projection(90.0f, \
								render.camera.aspect_ratio, 2.0f, 1000.0f);
	render.screenspace_ptris = \
			prot_memalloc(sizeof(t_screen_triangle) * 10000);
	render.q = prot_memalloc(sizeof(t_quaternion) * 10000);
	render.debug_img = get_image_by_name(sdl, "");
	render.occlusion.occlusion = true;
	render.occlusion.occluder_box = false;
	render.occlusion.draw_occlusion = false;
	return (render);
}

void	calculate_matview(t_camera *camera)
{
	t_vector3	vtarget;

	vtarget = vector3_add(camera->position, camera->look_dir);
	camera->mat_view = matrix_lookat(camera->position, \
										vtarget, (t_vector3){0.0f, 0.0f, 1.0f});
}

void	render_start_new(t_sdlcontext *sdl, t_player *player)
{
	float		fov_rad;
	t_render	*render;
	t_vector3	vtarget;

	render = &sdl->render;
	render->camera.look_dir = player->look_dir;
	render->camera.position = player->head_position;
	fov_rad = fov_deg_to_fov_rad(player->fov);
	render->camera.mat_proj.m[0][0] = render->camera.aspect_ratio * fov_rad;
	render->camera.mat_proj.m[1][1] = fov_rad;
	calculate_matview(&render->camera);
	ft_bzero(sdl->surface->pixels,
		sizeof(uint32_t) * sdl->window_h * sdl->window_w);
	ft_bzero(sdl->zbuffer, sizeof(float) * sdl->window_h * sdl->window_w);
	ft_bzero(&render->rs, sizeof(t_render_statistics));
}

void	free_render(t_render render)
{
	if (render.screenspace_ptris != NULL)
		free(render.screenspace_ptris);
	free(render.q);
}
