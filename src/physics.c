/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 13:52:50 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 23:16:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "shapes.h"
#include "objects.h"

/*t_vector3	raycast_result(t_vector3 origin, t_vector3 direction, t_list *entitylist)
{
	
}*/

void draw_colliders(t_physics p, t_sdlcontext sdl, t_render render)
{
	t_vector3	indexer;
	t_entity	ent;

	indexer = vector3_zero();
	while (indexer.z < 10.0f)
	{
		indexer.x = 0;
		while (indexer.x < 100.0f)
		{
			indexer.y = 0;
			while (indexer.y < 100.0f)
			{
				//printf("ind %f %f %f \n", indexer.x, indexer.y, indexer.z);
				if (p.cube[(int)indexer.x][(int)indexer.y][(int)indexer.z] == 1)
				{
					ent.obj = &sdl.objects[1];
					ent.transform.location = vector3_mul(indexer, 10.0f);
					ent.transform.scale = (t_vector3){1.0f, 5.0f, 0.1f};
					render_object(sdl, render, &ent);
				}
				indexer.y++;
			}
			indexer.x++;
		}
		indexer.z++;
	}
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
			vertex_ws = vector3_mul_vector3(ent.obj->vertices[v_i], vector3_mul(ent.transform.scale, 0.1f));
			vertex_ws = vector3_add(vertex_ws, ent.transform.location);
			vertex_ws.z += 6.0f;
			vertex_ws = vector3_mul(vertex_ws, 0.1f);
			printf_vec(vertex_ws);
			p->cube[(int)vertex_ws.x][(int)vertex_ws.y][(int)vertex_ws.z] = 1;
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