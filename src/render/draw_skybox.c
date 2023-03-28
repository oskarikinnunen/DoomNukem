/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 21:09:32 by raho              #+#    #+#             */
/*   Updated: 2023/03/28 09:38:39 by kfum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"

void	draw_skybox_image(t_world *world)
{
	t_img		*skybox;
	t_point		skypos1;
	t_point		skypos2;
	t_vector2	rot;

	if (world->sdl->render.occlusion.screen_full)
		return ;
	rot.x = ft_absf(fmodf(world->player->transform.rotation.x / (2 * PI), \
		1.0f));
	rot.y = fmodf(world->player->transform.rotation.y / PI, 1.0f);
	skybox = get_image_by_name(*world->sdl, "skybox.tga");
	skypos1 = point_add(point_zero(), (t_point){rot.x * skybox->size.x, \
		(rot.y * skybox->size.y)});
	skypos2 = point_add(point_zero(), (t_point){(rot.x * skybox->size.x) \
		- skybox->size.x, (rot.y * skybox->size.y)});
	draw_image(*world->sdl, skypos1, *skybox, skybox->size);
	draw_image(*world->sdl, skypos2, *skybox, skybox->size);
}
