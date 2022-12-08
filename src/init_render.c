/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/07 14:52:00 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

t_render	init_render(t_sdlcontext sdl, struct s_world *world)
{
	t_render	render;

	bzero(&render, sizeof(t_render));
	render.matworld = matrix_makeidentity();
	render.matproj = matrix_makeprojection(90.0f, (float)(sdl.window_h * sdl.resolution_scaling) / (float)(sdl.window_w * sdl.resolution_scaling), 2.0f, 1000.0f);
	render.occ_calc_tris = malloc(sizeof(t_triangle) * 10000);
	render.occ_draw_tris = malloc(sizeof(t_triangle) * 10000);
	render.worldspace_ptris = malloc(sizeof(t_point_triangle) * 10000);
	render.screenspace_ptris = malloc(sizeof(t_point_triangle) * 10000);
	render.q = malloc(sizeof(t_quaternion) * 10000); //TODO: should be multiplied by the largest obj vertex count
	render.debug_img = get_image_by_name(sdl, "");
	render.vtarget = vector3_add(render.position, render.lookdir);
	render.matcamera = matrix_lookat(render.position, render.vtarget, (t_vector3){0, 0, 1});
	render.matview = matrix_quickinverse(render.matcamera);
	render.occlusion.occlusion = false;
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

void update_render(t_render *render, t_player *player)
{
	render->lookdir = player->lookdir;
	render->position = player->transform.position;
}

void	free_render(t_render render)
{
	free(render.occ_calc_tris);
	free(render.occ_draw_tris);
	free(render.q);
}