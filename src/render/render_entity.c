/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_entity.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/24 20:30:40 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool	is_triangle_backface(t_world_triangle tritransformed, t_render *render)
{
	t_vector3	normal;
	t_vector3	vcameraray;

	normal = normal_calc_quaternion(tritransformed.p);
	vcameraray = vector3_sub(tritransformed.p[0].v, render->camera.position);
	if (vector3_dot(normal, vcameraray) < 0.0f || 1)
		return (false);
	else
		return (true);
}

static void	clip_and_render_triangles(t_sdlcontext *sdl, t_render *render)
{
	render_world_triangle_buffer_to_screen_triangle(render, *sdl);
	render_screen_triangles_buffer(sdl, render);
	render->screenspace_ptri_count = 0;
	render->map.texture = NULL;
	render->img = NULL;
}

static void	set_render_for_entity(
	t_sdlcontext *sdl, t_entity *entity, int index, int i)
{
	sdl->render.lightmode = lm_unlit;
	sdl->render.img = entity->obj->materials[index].img;
	if (entity->map != NULL && sdl->lighting_toggled)
	{
		sdl->render.map = entity->map[index];
		sdl->render.lightmode = lm_lit;
	}
	sdl->render.end_index = i;
	clip_and_render_triangles(sdl, &sdl->render);
	sdl->render.start_index = i + 1;
}

void	render_entitys_world_triangles(
	t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_world_triangle	world_tri;
	t_object			*obj;
	t_quaternion		temp;
	int					i;
	int					index;

	obj = entity->obj;
	if (entity->world_triangles == NULL || obj == NULL || entity->hidden)
		return ;
	render->world_triangles = entity->world_triangles;
	i = 0;
	render->start_index = 0;
	while (i < obj->face_count)
	{
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || \
		index != obj->faces[i + 1].materialindex)
			set_render_for_entity(sdl, entity, index, i);
		i++;
	}
	render->rs.render_count++;
}

void	render_entity(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	render->screenspace_ptri_count = 0;
	/*t_world	w;
	w.sdl = sdl;
	render_entity_worldtriangles(entity, &w);*/
	if ((point_cmp(entity->occlusion.clip.max, point_zero()) && point_cmp(entity->occlusion.clip.min, point_zero()))
		|| !render->occlusion.occlusion)
	{
		render->screen_edge.max.x = (float)(sdl->window_w * sdl->resolution_scaling) - 1.0f;
		render->screen_edge.max.y = (float)(sdl->window_h * sdl->resolution_scaling) - 1.0f;
		render->screen_edge.min = vector2_zero();
	}
	else
	{
		render->screen_edge.min = point_to_vector2(entity->occlusion.clip.min);
		render->screen_edge.max = point_to_vector2(entity->occlusion.clip.max);
	}
	render->screen_edge.max.x = (float)(sdl->window_w * sdl->resolution_scaling) - 1.0f;
	render->screen_edge.max.y = (float)(sdl->window_h * sdl->resolution_scaling) - 1.0f;
	render->screen_edge.min = vector2_zero();
	render_entitys_world_triangles(sdl, render, entity);
}
