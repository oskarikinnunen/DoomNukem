/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/04 21:51:52 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

t_render init_render(t_sdlcontext sdl)
{
	t_render	render;

	bzero(&render, sizeof(t_render));
	render.matworld = matrix_makeidentity();
	render.matproj = matrix_makeprojection(90.0f, (float)sdl.window_h / (float)sdl.window_w, 2.0f, 1000.0f);
	render.calc_triangles = malloc(sizeof(t_triangle) * 10000);
	render.draw_triangles = malloc(sizeof(t_triangle) * 10000);
	render.q = malloc(sizeof(t_quaternion) * 10000); //TODO: should be multiplied by the largest obj vertex count
	render.occluder = malloc(sizeof(t_entity) * 10000);
	render.debug_img = get_image_by_name(sdl, "");
	render.vtarget = vector3_add(render.position, render.lookdir);
	render.matcamera = matrix_lookat(render.position, render.vtarget, (t_vector3){0, 0, 1});
	render.matview = matrix_quickinverse(render.matcamera);
	/*temp testing render loop with .bot .wall .item*/
	
	
	t_entity	temp;

	bzero(&temp, sizeof(t_entity));
	temp.obj = &sdl.objects[0];
	bzero(&temp.bounds, sizeof(t_bounds));
	for (int i = 0; i < sdl.objects[0].vertice_count; i++)
	{
		if (temp.bounds.width < vector2_dist((t_vector2){sdl.objects[0].vertices[i].x, sdl.objects[0].vertices[i].y}, vector2_zero()))
			temp.bounds.width = vector2_dist((t_vector2){sdl.objects[0].vertices[i].x, sdl.objects[0].vertices[i].y}, vector2_zero());
		if (temp.bounds.height < sdl.objects[0].vertices[i].z)
			temp.bounds.height = sdl.objects[0].vertices[i].z;
	}
	temp.transform.scale = (t_vector3){15, 15, 15};
	//temp.transform.scale.y = 0.5;
	//temp.transform.location.x += 1500;
	list_push(&render.listitem, &temp, sizeof(t_item));
	return(render);
}
