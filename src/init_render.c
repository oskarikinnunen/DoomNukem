/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 20:29:05 by vlaine           ###   ########.fr       */
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
	render.obj.faces = malloc(sizeof(t_face) * 10000);
	render.obj.uvs = malloc(sizeof(t_vector2) * 10000);
	render.obj.vertices = malloc(sizeof(t_vector3) * 10000);
	render.debug_img = get_image_by_name(sdl, "");
	/*temp testing render loop with .bot .wall .item*/
	t_entity	temp;
	for (int a = 0; a < 5; a++)
	{
		for (int b = 0; b < 5; b++)
		{
			bzero(&temp, sizeof(t_entity));
			temp.obj = &sdl.objects[1];
			for (int i = 0; i < sdl.objects[1].vertice_count; i++)
			{
				if (temp.bounds.width < vector2_dist((t_vector2){sdl.objects[1].vertices[i].x, sdl.objects[1].vertices[i].y}, vector2_zero()))
					temp.bounds.width = vector2_dist((t_vector2){sdl.objects[1].vertices[i].x, sdl.objects[1].vertices[i].y}, vector2_zero());
				if (temp.bounds.height < sdl.objects[1].vertices[i].z)
					temp.bounds.height = sdl.objects[1].vertices[i].z;
			}
			temp.transform.location = (t_vector3){100 * a, 100 * b, 0};
			temp.transform.scale = vector3_one();
			list_push(&render.listbot, &temp, sizeof(t_bot));
		}
	}

	temp.obj = &sdl.objects[0];
	bzero(&temp.bounds, sizeof(t_bounds));
	for (int i = 0; i < sdl.objects[0].vertice_count; i++)
	{
		if (temp.bounds.width < vector2_dist((t_vector2){sdl.objects[0].vertices[i].x, sdl.objects[0].vertices[i].y}, vector2_zero()))
			temp.bounds.width = vector2_dist((t_vector2){sdl.objects[0].vertices[i].x, sdl.objects[0].vertices[i].y}, vector2_zero());
		if (temp.bounds.height < sdl.objects[0].vertices[i].z)
			temp.bounds.height = sdl.objects[0].vertices[i].z;
	}
	//temp.transform.scale.y = 0.5;
	temp.transform.location.x += 1500;
	list_push(&render.listitem, &temp, sizeof(t_item));
	return(render);
}
