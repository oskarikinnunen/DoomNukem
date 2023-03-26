/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 21:09:32 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 15:38:03 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"

void	draw_skybox_image(t_world *world, t_img *skybox)
{
	t_point		skypos1;
	t_point		skypos2;
	t_vector2	rot;

	if (world->sdl->render.occlusion.screen_full)
		return ;
	rot.x = fmodf(world->player->transform.rotation.x / (2 * PI), 1.0f);
	rot.y = fmodf(world->player->transform.rotation.y / PI, 1.0f);
	skybox = get_image_by_name(*world->sdl, "skybox.tga");
	if (rot.x < 0)
	{
		skypos1 = point_add(point_zero(), (t_point){(rot.x * skybox->size.x) + \
		world->sdl->screensize.x, (rot.y * skybox->size.y)});
		skypos2 = point_add(point_zero(), (t_point){(rot.x * skybox->size.x) - \
		(skybox->size.x - world->sdl->screensize.x), (rot.y * skybox->size.y)});
	}
	else
	{
		skypos1 = point_add(point_zero(), (t_point){rot.x * skybox->size.x, \
			(rot.y * skybox->size.y)});
		skypos2 = point_add(point_zero(), (t_point){(rot.x * skybox->size.x) \
			- skybox->size.x, (rot.y * skybox->size.y)});
	}
	draw_image(*world->sdl, skypos1, *skybox, skybox->size);
	draw_image(*world->sdl, skypos2, *skybox, skybox->size);
}
