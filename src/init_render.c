/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/01 16:31:58 by okinnune         ###   ########.fr       */
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
	render.debug_img = get_image_by_name(sdl, "");
	/*temp testing render loop with .bot .wall .item*/
	t_entity	temp;

	bzero(&temp, sizeof(t_entity));
	temp.obj = &sdl.objects[1];
	temp.transform.scale = vector3_one();
	list_push(&render.listbot, &temp, sizeof(t_bot));
	temp.obj = &sdl.objects[0];
	list_push(&render.listitem, &temp, sizeof(t_item));
	return(render);
}
