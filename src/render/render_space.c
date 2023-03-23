/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 15:36:10 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/23 21:47:20 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool is_triangle_backface(t_world_triangle tritransformed, t_render *render)
{
	t_vector3 normal;
	t_vector3 vcameraray;

	normal = normal_calc_quaternion(tritransformed.p);
	vcameraray = vector3_sub(tritransformed.p[0].v, render->camera.position);
	if (vector3_dot(normal, vcameraray) < 0.0f || 1)
		return (false);
	else
		return (true);
}

static void clip_and_render_triangles(t_sdlcontext *sdl, t_render *render)
{
	render_world_triangle_buffer_to_screen_triangle(render, *sdl);
	render_screen_triangles_buffer(sdl, render);
	render->screenspace_ptri_count = 0;
	render->map.texture = NULL;
	render->img = NULL;
}

void render_world_triangles(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	t_world_triangle		world_tri;
	t_object		*obj;
	t_quaternion	temp;
	int				i;
	int				index;

	if (entity->world_triangles == NULL || entity->obj == NULL)
	{
		//doomlog(LOG_WARNING, "entity->worldtriangles == null in render_quaternions\n!	printing entity->object_name.str and entity->obj->name");
		//doomlog(LOG_WARNING, entity->object_name.str);
		//doomlog(LOG_WARNING, entity->obj->name);
		return;
	}
	obj = entity->obj;
	render->world_triangles = entity->world_triangles;
	i = 0;
	render->start_index = 0;
	while (i < obj->face_count)
	{
		index = obj->faces[i].materialindex;
		if (i + 1 == obj->face_count || index != obj->faces[i + 1].materialindex)
		{
			render->lightmode = lm_unlit;
			render->img = obj->materials[index].img;
			if (entity->map != NULL && sdl->lighting_toggled)
			{
				render->map = entity->map[index];
				//render->map.img_size = render->img->size;
				//render->map.size = point_sub(render->map.size, (t_point){1, 1});
				render->lightmode = lm_lit;
			}
			render->end_index = i;
			clip_and_render_triangles(sdl, render);
			render->start_index = i + 1;
		}
		i++;
	}
	render->world_triangles = NULL; // should be unnecessary
	render->rs.render_count++;
}
