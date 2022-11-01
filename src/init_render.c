/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:59:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/01 14:16:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_render init_render(t_sdlcontext sdl)
{
	t_render	render;

	bzero(&render, sizeof(t_render));
	render.matworld = matrix_makeidentity();
	render.matproj = matrix_makeprojection(90.0f, (float)sdl.window_h / (float)sdl.window_w, 2.0f, 1000.0f);
	render.calc_triangles = malloc(sizeof(t_triangle) * 10000);
	render.draw_triangles = malloc(sizeof(t_triangle) * 10000);
	render.q = malloc(sizeof(t_quaternion) * 10000); //TODO: should be multiplied by the largest obj vertex count
	render.debug_img = get_image_by_name(sdl, "debug1.png");
	return(render);
}
