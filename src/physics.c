/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 13:52:50 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/03 18:28:51 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "shapes.h"
#include "objects.h"

/*t_entity	*raycast_result(t_vector3 origin, t_vector3 direction, t_list *entitylist)
{
	
}*/

/*static	t_object	*object_cube()
{
	static t_object	cube;

	if (cube.vertice_count != 0)
		return &cube;
	cube.vertice_count = 8;
	cube.vertices = ft_memalloc(8 * sizeof(t_vector3));
	if (cube.vertices == NULL)
		error_log(EC_MALLOC);
	cube.vertices[0] = vector3_zero();
	cube.vertices[1] = vector3_zero();
}*/

t_entity *entity_collides(t_physics p, t_entity ent)
{
	t_entity	*result;
	int			e_i;
	int			v_i;
	t_vector3	vertex_ws;

	e_i = 0;
	v_i = 0;
	result = NULL;
	while (v_i < ent.obj->vertice_count)
	{
		vertex_ws = vector3_mul_vector3(ent.obj->vertices[v_i], ent.transform.scale);
		vertex_ws = vector3_add(vertex_ws, ent.transform.location);
		vertex_ws = vector3_mul(vertex_ws, 0.1f);
		vertex_ws.x = ft_clampf(vertex_ws.x, 0.0f, 99.0f);
		vertex_ws.y = ft_clampf(vertex_ws.y, 0.0f, 99.0f);
		if (vertex_ws.z > 0.0f && p.cube[(int)vertex_ws.x][(int)vertex_ws.y][(int)vertex_ws.z] != 0)
		{
			result = p.entities[p.cube[(int)vertex_ws.x][(int)vertex_ws.y][(int)vertex_ws.z] - 1];
			break ;
		}
		v_i++;
	}
	return (result);
}

void draw_colliders(t_physics p, t_sdlcontext sdl, t_render render)
{
	t_vector3	indexer;
	t_entity	ent;

	indexer = vector3_zero();
	ent.obj = get_object_by_name(sdl, "cube");
	ent.transform.scale = (t_vector3){1.0f, 1.0f, 1.0f};
	render.wireframe = true;
	while (indexer.z < 10.0f)
	{
		indexer.x = 0;
		while (indexer.x < 100.0f)
		{
			indexer.y = 0;
			while (indexer.y < 100.0f)
			{
				ent.transform.location = vector3_mul(indexer, 10.0f);
				if (p.cube[(int)indexer.x][(int)indexer.y][(int)indexer.z] != 0) {
					render.gizmocolor = CLR_PRPL;
					render_gizmo(sdl, render, &ent);
				}
				if (indexer.y == 0 || indexer.y == 99.0f || indexer.x == 0 || indexer.x == 99.0f || indexer.z == 0) {
					render.gizmocolor = CLR_GREEN;
					render_gizmo(sdl, render, &ent);
				}
				indexer.y++;
			}
			indexer.x++;
		}
		indexer.z++;
	}
	render.wireframe = false;
}

void calculate_colliders(t_physics *p)
{
	t_entity	ent;
	int			e_i;
	int			v_i;
	t_vector3	vertex_ws;

	e_i = 0;
	ft_bzero(p->cube, sizeof(p->cube));
	while (p->entities[e_i] != NULL)
	{
		ent = *p->entities[e_i];
		v_i = 0;
		while (v_i < ent.obj->vertice_count)
		{
			vertex_ws = vector3_mul_vector3(ent.obj->vertices[v_i], ent.transform.scale);
			vertex_ws = vector3_add(vertex_ws, ent.transform.location);
			vertex_ws = vector3_mul(vertex_ws, 0.1f);
			if (vertex_ws.z > 0.0f)
				p->cube[(int)vertex_ws.x][(int)vertex_ws.y][(int)vertex_ws.z] = e_i + 1;
			v_i++;
		}
		e_i++;
	}
}

bool	pointrectanglecollision(t_point p, t_rectangle rect) //dunno if this should be in physics
{
	return (p.x > rect.position.x && p.x < rect.position.x + rect.size.x
		&& p.y > rect.position.y && p.y < rect.position.y + rect.size.y);
}

bool	pointcirclecollision(t_vector2 p, t_vector2 cp, float r)
{
	return (vector2_dist(p, cp) <= r);
}

//TODO: no worky, look at https://www.jeffreythompson.org/collision-detection/line-circle.php and fix
bool	linecirclecollision(t_line line, t_vector2 cp, float r) //CONTINUE FIXING HERE
{
	return	false;
	/*float	start[2];
	float	end[2];
	float	dist;
	float	dot;
	
	f2tov2(start, line.start);
	f2tov2(end, line.end);
	f2mul(start, TILESIZE); //Lines are in their grid positions so multiply by tilesize to get world coordinates
	f2mul(end, TILESIZE);
	dist = f2dist(start, end);
	dot = (((cp[X]-start[X])*(end[X]-start[X])) + ((cp[Y]-start[Y])*(end[Y]-start[Y])) ) / pow(dist,2);
	float x_dist = (start[X] + (dot * (end[X]-start[X]))) - cp[X];
	float y_dist = (start[Y] + (dot * (end[Y]-start[Y]))) - cp[Y];
	dist = sqrt((x_dist * x_dist) + (y_dist * y_dist));
	return (dist <= r);*/
}