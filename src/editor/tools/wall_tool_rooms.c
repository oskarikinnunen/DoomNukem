/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool_rooms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 03:20:37 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 13:44:05 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
 
#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "objects.h"


/* DRAWDEBUGFLOOR */
/*
	takes index, indexes to that point and draws the attempted triangle to screen

*/

t_vector2	*transformed_around(t_vector2 og, float angle, t_vector2 *t, int count)
{
	static t_vector2	final[MAXSELECTED];
	int					i;
	t_vector2			min;

	min = (t_vector2){4000, 4000};
	ft_memcpy(final, t, sizeof(t_vector2[MAXSELECTED]));
	i = 0;
	while (i < count)
	{
		if (final[i].x < min.x)
			min.x = final[i].x;
		if (final[i].y < min.y)
			min.y = final[i].y;
		i++;
	}
	i = 0;
	float c = cosf(angle);
	float s = sinf(angle);
	while (i < count)
	{
		final[i] = vector2_sub(final[i], og);
		t_vector2 temp = final[i];
		temp.x = (final[i].x * c) - (final[i].y * s);
		temp.y = (final[i].y * c) + (final[i].x * s);
		temp.x *= -1.0f;
		//temp.y *= -1.0f;
		final[i] = temp;
		//final[i] = vector2_sub(final[i], min);
		i++;
	}
	return (final);
}

static bool intersect(t_line line1, t_vector2 *edges, int edgecount)
{
	t_line	line2;
	float	xdiff1;
	float	xdiff2;
	float	ydiff1;
	float	ydiff2;
	int		i;
	float	res;

	i = 0;
	while (i < edgecount)
	{
		line2.start = edges[i];
		if (i == edgecount - 1)
			line2.end = edges[0];
		else
			line2.end = edges[i + 1];
		
		xdiff1 = line1.start.x - line1.end.x;
		xdiff2 = line2.start.x - line2.end.x;
		ydiff1 = line1.start.y - line1.end.y;
		ydiff2 = line2.start.y - line2.end.y;
		res = xdiff1 * ydiff2 - ydiff1 * xdiff2;
		
		if (fabsf(res) > 0.000001f)
		{
			float fa = line1.start.x * line1.end.y - line1.start.y * line1.end.x;
			float fb = line2.start.x * line2.end.y - line2.start.y * line2.end.x;

			float x = (fa * xdiff2 - fb * xdiff1) / res;
			float y = (fa * ydiff2 - fb * ydiff1) / res;
			if ((ft_minf(line1.start.x, line1.end.x) < x
				&& ft_maxf(line1.start.x, line1.end.x) > x
				&& ft_minf(line2.start.x, line2.end.x) < x
				&& ft_maxf(line2.start.x, line2.end.x) > x)
				|| (ft_minf(line1.start.y, line1.end.y) < y
				&& ft_maxf(line1.start.y, line1.end.y) > y
				&& ft_minf(line2.start.y, line2.end.y) < y
				&& ft_maxf(line2.start.y, line2.end.y) > y))
				{
					//printf("\nLINE COLLIDES WITH %i->%i \n", i, i + 1);
					return (true);
				}
		}
		i++;
	}
	return (false);
}

static	bool correctangle_new(t_floorcalc fc, int *valid, int validcount) //TODO: takes 'fc', 'valid' array and 'validcount'
{
	t_vector2	*tr;
	t_vector2	pool[MAXSELECTED];
	float		angle;
	t_vector2	first;
	t_vector2	center;
	t_vector2	second;
	int			i;

	first = fc.edges[valid[validcount - 1]];
	center = fc.edges[valid[0]];
	second = fc.edges[valid[1]];
	angle = vector2_anglebetween(first, second);
	i = 0;
	while (i < validcount) 
	{
		pool[i] = fc.edges[valid[i]];
		i++;
	}
	tr = transformed_around(second, -angle, pool, validcount);
	float lowest = -100000.0f;
	float highest = 100000.0f;
	int		l_i = -2;
	i = 0;
	while (i < validcount)
	{
		if (tr[i].y > lowest)
		{
			lowest = tr[i].y;
			l_i = i;
		}
		if (tr[i].y < highest)
			highest = tr[i].y;
		i++;
	}
	printf("lowest point in shape was %i, value: %f , center y %f HIGHEST: %f\n", valid[l_i], lowest, tr[valid[0]].y, highest);
	if (valid[l_i] == valid[0])
	{
		printf("CENTER WAS LOWEST POINT");
	}
	return (valid[l_i] == valid[0]);
	//return (tr[1].y >= tr[0].y && tr[1].y >= tr[2].y);
}

static	bool correctangle(t_vector2 vs[3]) //TODO: takes 'fc', 'valid' array and 'validcount'
{
	t_vector2	*tr;
	float		angle = vector2_anglebetween(vs[0], vs[2]);
	if (isinf(angle) || isnan(angle))
	{
		printf("angle was nan or inf \n");
		exit(0);
	}
	tr = transformed_around(vs[2], -angle, vs, 3);
	return (tr[1].y >= tr[0].y && tr[1].y >= tr[2].y);
}

static	bool correctangle_debug(t_vector2 vs[3]) //TODO: takes 'fc', 'valid' array and 'validcount'
{
	t_vector2	*tr;
	float		angle = vector2_anglebetween(vs[0], vs[2]);
	if (isinf(angle) || isnan(angle))
	{
		printf("angle was nan or inf \n");
		exit(0);
	}

	tr = transformed_around(vs[2], -angle, vs, 3);
	printf("0y: %f 5y: %f\n6 y: %f\n", tr[0].y, tr[2].y, tr[1].y);
	return (tr[1].y >= tr[0].y && tr[1].y >= tr[2].y);
}

static void indexesdebug(t_sdlcontext *sdl, t_floorcalc fc, int i)
{
	char	*str= ft_itoa(fc.faces[i].v_indices[0]);
	print_text_boxed(sdl, str, (t_point) {300, 240});
	free(str);
	str= ft_itoa(fc.faces[i].v_indices[1]);
	print_text_boxed(sdl, str, (t_point) {350, 240});
	free(str);
	str= ft_itoa(fc.faces[i].v_indices[2]);
	print_text_boxed(sdl, str, (t_point) {400, 240});
	free(str);
}

void	floorcalc_debugdraw(t_editor *ed, t_sdlcontext *sdl, t_floorcalc fc, int tri_i, float rad)
{
	int			i;
	t_vector3	ws;
	t_point		ss;
	t_vector3	ws_to;
	char		*str;

	i = 0;
	/*sprintf(str, "edges: %i", fc.edgecount);
	print_text_boxed(sdl, str, (t_point) {30, 400});*/
	while (i < fc.edgecount)
	{
		ws = (t_vector3){fc.edges[i].x, fc.edges[i].y, 0.0f};
		ss = vector3_to_screenspace(ws, *sdl);
		str = ft_itoa(i);
		print_text_boxed(sdl, str, ss);
		free (str);
		sdl->render.gizmocolor = CLR_RED;
		render_gizmo(*sdl, sdl->render, ws, 10);
		i++;
	}
	i = tri_i;
	sdl->render.gizmocolor = CLR_PRPL;
	
	/*CREATED LINE */
	t_vector2 first = fc.edges[fc.faces[i].v_indices[0]];
	t_vector2 center = fc.edges[fc.faces[i].v_indices[1]];
	t_vector2 second = fc.edges[fc.faces[i].v_indices[2]];
	indexesdebug(sdl, fc, i);

	char *stra;
	float a1 = vector2_anglebetween(first, second);
	float a2 = vector2_anglebetween(first, center);
	t_vector2 t1 = (t_vector2) {2.0f, 2.0f};
	t_vector2 t2 = (t_vector2) {2.0f, 3.0f};
	float a3 = vector2_anglebetween(t1, t2);
	float aw1 = vector2_anglebetween(center, second);
	float angleworld = vector2_anglebetween(vector2_sub(first, first), vector2_sub(second, first)); //'better' one
	//float angleworld = anglebetween(vector2_sub(first, second), vector2_sub(first, center)); //demo one
	stra = ft_itoa(radtodeg(a1)); //DIV with m_pi to get internal triangle angles
	print_text_boxed(sdl, stra, (t_point) {300, 200});
	free(stra);
	stra = ft_itoa(radtodeg(a2)); //Internal angle M_PI - ((a1 / M_PI) + (a3 / M_PI)))
	print_text_boxed(sdl, stra, (t_point) {350, 200});
	free(stra);
	stra = ft_itoa(radtodeg(a3));
	print_text_boxed(sdl, stra, (t_point) {400, 200});
	free(stra);
	stra = ft_itoa(radtodeg(angleworld));
	print_text_boxed(sdl, stra, (t_point) {400, 300});
	free(stra);

	//printf("M_PI MINUS %f, in deg %f\n", 0.0f, radtodeg(M_PI + a3));
	t_vector2 *tr = transformed_around(second, -a1, fc.edges, fc.edgecount);
	i = 0;
	t_vector2 temp1;
	t_vector2 temp2;
	while (i < fc.edgecount)
	{
		temp1 = tr[i];
		temp2 = tr[i + 1];
		if (i == fc.edgecount - 1)
			temp2 = tr[0];
		temp1 = vector2_add(temp1, (t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		temp2 = vector2_add(temp2, (t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		drawline(*sdl, vector2_to_point(temp1), vector2_to_point(temp2), CLR_RED);
		if (i == tri_i)
		{
			//draw the whatevs line
		}
		i++;
	}
	i = 0;
	while (i <= tri_i) {
		temp1 = vector2_add(tr[fc.faces[i].v_indices[2]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		temp2 = vector2_add(tr[fc.faces[i].v_indices[0]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		if (i == tri_i)
			drawline(*sdl, vector2_to_point(temp1), vector2_to_point(temp2), CLR_BLUE);
		else
			drawline(*sdl, vector2_to_point(temp1), vector2_to_point(temp2), CLR_RED);
		temp1 = vector2_add(tr[fc.faces[i].v_indices[1]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		if (i == tri_i)
			drawcircle(*sdl, vector2_to_point(temp1), 5, CLR_BLUE);
		else
			drawcircle(*sdl, vector2_to_point(temp1), 5, CLR_RED);
		i++;
	}

	ft_bzero(sdl->surface->pixels, sizeof(uint32_t) * sdl->window_h * sdl->window_w);
	i = 0;
	while (i < fc.edgecount - 2)
	{
		temp1 = vector2_add(fc.edges[i],(t_vector2){sdl->window_w / 2, 0.0f});
		temp2 = vector2_add(fc.edges[i + 1],(t_vector2){sdl->window_w / 2, 0.0f});
		drawline(*sdl, vector2_to_point(temp1),
			vector2_to_point(temp2), CLR_RED);
		temp1 = vector2_add(fc.normals[i].start,(t_vector2){sdl->window_w / 2, 0.0f});
		temp2 = vector2_add(fc.normals[i].end,(t_vector2){sdl->window_w / 2, 0.0f});
		drawline(*sdl, vector2_to_point(temp1),
			vector2_to_point(temp2), CLR_BLUE);
		i++;
	}
	
}

/*static void gen_tris()
{

}*/

static bool	isunique(t_floorcalc fc, t_vector2 v2)
{
	int		i;
	bool	result;

	i = 0;
	if (fc.edgecount == 0)
		return (true);
	result = true;
	while (i < fc.edgecount)
	{
		if (vector2_cmp(v2, fc.edges[i]))
			result = false;
		i++;
	}
	return (result);
}

static	int select_ni(int *valid, int i, int count)
{
	//rotates around if i < 
	return (0);

}

static	int select_pi(int *valid, int i, int count)
{
	return (0);
}

static	void removevalid (int *valid, int count, int ri)
{
	int	i;

	i = 0;
	//printf("removing ear: %i \n", valid[ri]);
	while (i < count - 1)
	{
		if (i >= ri)
			valid[i] = valid[i + 1];
		i++;
	}
}

static void printvalid(int valid[MAXSELECTED], int count)
{
	int	i;

	printf("CURRENT VALID INDEXES: ");
	i = 0;
	while(i < count)
		printf(" %i,", valid[i++]);
	printf("\n");
}


static	void shiftvalid(int valid[32], int count)
{
	int	temp;
	int	f_temp;
	int	i;

	f_temp = valid[0];
	i = 0;
	while (i < count)
	{
		if (i == count - 1)
			valid[i] = f_temp;
		else
			valid[i] = valid[i + 1];
		i++;
	}
}

static	void shiftvalid_left(int *valid, int count)
{
	int	f_temp;
	int	i;

	f_temp = valid[count - 1];
	i = count;
	while (i >= 0)
	{
		if (i == 0)
			valid[i] = f_temp;
		else
			valid[i] = valid[i - 1];
		i--;
	}
}

bool	isaligned(t_vector2 vs[3])
{
	t_vector2	align1 = vector2_sub(vs[1], vs[2]);
	t_vector2	align2 = vector2_sub(vs[1], vs[0]);
	align1 = vector2_normalise(align1);
	align2 = vector2_normalise(align2);
	align1 = vector2_abs(align1);
	align2 = vector2_abs(align2);
	return (vector2_cmp(align1, align2));
}

void	populatevalid(int	valid[32], int *validcount, t_floorcalc fc)
{
	*validcount = 0;
	while (*validcount < fc.edgecount)
	{
		valid[*validcount] = *validcount;
		*validcount = *validcount + 1;
	}
}

void	populatevalid_l(int	valid[32], int *validcount, t_floorcalc fc)
{
	*validcount = 0;
	while (*validcount < fc.edgecount)
	{
		valid[*validcount] = fc.edgecount - *validcount - 1;
		//printf("pop %i \n", valid[*validcount]);
		*validcount = *validcount + 1;
	}
}

bool	checkroomnormal(t_floorcalc *fc)
{
	int	i;

	i = 0;
	while (i < fc->edgecount - 2)
	{
		t_vector2	normal;
		t_line		nline;
		//normal = vector2_cross(fc.edges[i], fc.edges[i + 1]);
		nline.start = vector2_add(fc->edges[i], vector2_one());
		/*nline.end = vector2_sub(fc->edges[i], fc->edges[i + 1]);
		
		float	temp = nline.end.y;
		nline.end.y = -nline.end.x;
		nline.end.x = -temp;
		nline.end = vector2_mul(vector2_normalise(nline.end), 500.0f);*/
		nline.end = vector2_lerp(fc->edges[i], fc->edges[i + 1], 0.5f);
		/*if (!intersect(nline, fc->edges, fc->edgecount))
			return (true);*/
		fc->normals[i] = nline;
		i++;
	}
	return (false);
}

bool	points_collide(t_floorcalc *fc, t_vector2 tri[3])
{
	int	i;

	t_triangle	t;
	t_point		p;

	t.p[0].v = v2tov3(tri[0]);
	t.p[1].v = v2tov3(tri[1]);
	t.p[2].v = v2tov3(tri[2]);
	i = 0;
	while (i < fc->edgecount)
	{
		if (vector2_cmp(fc->edges[i], tri[0])
			|| vector2_cmp(fc->edges[i], tri[1])
			|| vector2_cmp(fc->edges[i], tri[2]))
		{
			i++;
			continue;
		}
		p = vector2_to_point(fc->edges[i]);
		if (pointtrianglecollision(p, t))
			return (true);
		i++;
	}
	return (false);
	//pointtrianglecollision()
	/*int	i;
	t_line	o0;
	t_line	o1;
	t_line	o2;
	t_line	t0;
	t_line	t1;
	t_line	t2;
	t_line	t0s;
	t_line	t1s;
	t_line	t2s;

	i = 0;
	o0.start = tri[0];
	o0.end = tri[1];
	o1.start = tri[1];
	o1.end = tri[2];
	o2.start = tri[2];
	o2.end = tri[0];


	t0.end = tri[0];
	t1.end = tri[1];
	t2.end = tri[2];
	while (i < fc->edgecount)
	{
		if (vector2_cmp(fc->edges[i], tri[0])
			|| vector2_cmp(fc->edges[i], tri[1])
			|| vector2_cmp(fc->edges[i], tri[2]))
		{
			i++;
			continue;
		}
		t0.start = fc->edges[i];
		t1.start = fc->edges[i];
		t2.start = fc->edges[i];
		t0s = line_shorten(t0);
		t1s = line_shorten(t1);
		t2s = line_shorten(t2);
		if (!linelineintersect(t0s, o0) && !linelineintersect(t0s, o1) && !linelineintersect(t0s, o2)
			&& !linelineintersect(t1s, o0) && !linelineintersect(t1s, o1) && !linelineintersect(t1s, o2)
			&& !linelineintersect(t2s, o0) && !linelineintersect(t2s, o1) && !linelineintersect(t2s, o2))
			{
				printf("edge %f %f\n", fc->edges[i].x, fc->edges[i].y);
				return (true);
			}
			
		i++;
	}
	return (false);*/
}

bool isany(int o, int i1, int i2, int i3)
{
	return (o == i1 || o == i2 || o == i3);
}

bool isvalid_any(int i1, int i2, int i3, int valid[32], int validcount)
{
	return (isany(valid[0], i1, i2, i3)
			&& isany(valid[1], i1, i2, i3)
			&& isany(valid[validcount - 1], i1, i2, i3));
}

t_line	line_shorten(t_line line) //lol
{
	t_vector2	temp;
	t_vector2	delta_start;
	t_vector2	delta_end;
	t_line		final;

	float	dist = vector2_dist(line.start, line.end);
	temp = vector2_sub(line.start, line.end);
	temp = vector2_clamp_magnitude(temp, dist - 1.0f);
	temp = vector2_add(temp, line.end);
	final.start = temp;
	temp = vector2_sub(line.end, line.start);
	temp = vector2_clamp_magnitude(temp, dist - 1.0f);
	temp = vector2_add(temp, line.start);
	final.end = temp;
	return (final);
}

void	triangulate(t_floorcalc *fc, int valid_target)
{
	int	valid[MAXSELECTED];
	int	validcount;
	int	i;
	int	attempts = 0;
	

	validcount = 0;
	populatevalid(valid, &validcount, *fc);
	int	lowest = validcount;
	i = 0;
	fc->facecount = 0;
	//printf("triangulating shape with %i edges\n", fc->edgecount);
	while (validcount > valid_target)
	{
		//printf("try %i %i %i \n", valid[validcount - 1], valid[0], valid[1]);
		t_line line1;
		line1.start = fc->edges[valid[validcount - 1]];
		line1.end = fc->edges[valid[1]];
		t_vector2 first = fc->edges[valid[validcount - 1]];
		t_vector2 center = fc->edges[valid[0]];
		t_vector2 second = fc->edges[valid[1]];
		if (isaligned((t_vector2[3]){first,center,second}))
		{
			center.x = 0.0001f;
			center.y = -0.0001f;
			//printf("removed aligned %i \n", valid[0]);
		}

		if (valid[0] == 2 && valid[1] == 5 && valid[validcount - 1] == 1)
		{
			if (!correctangle((t_vector2[3]){first,center,second}))
				printf("512 IS INCORRECT??\n");
		}
		/*if (!correctangle((t_vector2[3]){first,center,second}))
			printf("incorrect angle between %i %i %i\n", valid[validcount - 1], valid[0], valid[1]);
		*/
		if (!points_collide(fc, (t_vector2[3]){first,center,second})
			&& correctangle((t_vector2[3]){first,center,second})
			&& !intersect(line_shorten(line1), fc->edges, fc->edgecount))
		{
			fc->faces[fc->facecount].v_indices[0] = valid[validcount - 1];
			fc->faces[fc->facecount].v_indices[1] = valid[0];
			fc->faces[fc->facecount].v_indices[2] = valid[1];
			//printf("connect %i %i %i \n", valid[validcount - 1], valid[0], valid[1]);
			removevalid(valid, validcount--, 0);
			//printf("next valid = %i \n", valid[0]);
			if (validcount == valid_target)
			{
				line1.start = fc->edges[valid[validcount - 1]];
				line1.end = fc->edges[valid[1]];
				first = fc->edges[valid[validcount - 1]];
				center = fc->edges[valid[0]];
				second = fc->edges[valid[1]];
			}
			if (validcount <= lowest)
				lowest = validcount;
			fc->facecount++;
		}
		else
		{
			/*if (valid_target == 4)
				printf("ear %i failed \n", valid[0]);*/
			if ((i >= 25000 && i < 50000) || (i >= 75000 && i < 100000))
				shiftvalid(valid, validcount);
			else
				shiftvalid_left(valid, validcount);
		}
		//printvalid(valid, validcount);
		i++;
		if (i == 50000)
		{
			populatevalid_l(valid, &validcount, *fc);
			fc->facecount = 0;
		}
		if (i > 100000)
		{
			fc->facecount = 0;
			doomlog(LOG_WARNING, "Couldn't create floor, too many iterations!");
			return ;
		}
		if (attempts > fc->edgecount * 2)
		{
			fc->facecount = 0;
			return ;
		}
	}
	//printf("made %i faces \n", fc->facecount);
}

void	free_object(t_object *object)
{
	if (object == NULL)
		return ;
	if (object->faces != NULL)
		free(object->faces);
	if (object->materials != NULL)
		free(object->materials);
	if (object->uvs != NULL)
		free(object->uvs);
	if (object->vertices != NULL)
		free(object->vertices);
}

#include "doomnukem.h"

void	free_floor(t_world *world, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->floorcount)
	{
		if (room->floors[i].entity != NULL)
		{
			if (room->floors[i].entity->obj != NULL )
			{
				free_object(room->floors[i].entity->obj);
				room->floors[i].entity->obj = NULL;
			}
			destroy_entity(world, room->floors[i].entity);
			room->floors[i].entity = NULL;
		}
		i++;
	}
	//room->floorcount = 0;
}

void	free_ceilings(t_world *world, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->ceilingcount)
	{
		if (room->ceilings[i].entity != NULL)
		{
			if (room->ceilings[i].entity->obj != NULL )
			{
				free_object(room->ceilings[i].entity->obj);
				room->ceilings[i].entity->obj = NULL;
			}
			destroy_entity(world, room->ceilings[i].entity);
			room->ceilings[i].entity = NULL;
		}
		i++;
	}
}

//void	apply_currentfloortexture(t_world *world, t_room *room, char *texname)

void	_room_triangulate_floors(t_world *world, t_area *room)
{
	t_floorcalc	fc;
	t_wall		*w;
	t_meshtri	*mtri;
	t_object	*obj;
	int			i;

	ft_bzero(&fc, sizeof(fc));
	i = 0;
	//free_floor(world, room);
	while (i < room->edgecount)
	{
		fc.edges[fc.edgecount++] = room->edges[i];
		i++;
	}
	triangulate(&fc, 2);
	if (fc.facecount == 0)
		return ;
	ft_bzero(room->floors, sizeof(room->floors));
	room->floorcount = fc.facecount;
	i = 0;
	while (i < fc.facecount)
	{
		mtri = &room->floors[i];
		obj = object_tri(world->sdl);
		obj->materials->img = get_image_by_name(*world->sdl, room->s_floortex.str);
		mtri->v[0] = v2tov3(fc.edges[fc.faces[i].v_indices[0]]);
		mtri->v[1] = v2tov3(fc.edges[fc.faces[i].v_indices[1]]);
		mtri->v[2] = v2tov3(fc.edges[fc.faces[i].v_indices[2]]);
		mtri->v[0].z += room->height;
		mtri->v[1].z += room->height;
		mtri->v[2].z += room->height;
		mtri->uv[0] = fc.edges[fc.faces[i].v_indices[0]];
		mtri->uv[1] = fc.edges[fc.faces[i].v_indices[1]];
		mtri->uv[2] = fc.edges[fc.faces[i].v_indices[2]];
		mtri->uv[0] = vector2_div(mtri->uv[0], 100.0f);
		mtri->uv[1] = vector2_div(mtri->uv[1], 100.0f);
		mtri->uv[2] = vector2_div(mtri->uv[2], 100.0f);
		applytrimesh(*mtri, obj);
		mtri->entity = spawn_entity(world, obj);
		mtri->entity->rigid = true;
		update_floor_bounds(mtri);
		
		//printf("uv 1: %f %f2: %f %f 3: %f %f\n", mtri->uv[0].x, mtri->uv[0].y, mtri->uv[1].x, mtri->uv[1].y, mtri->uv[2].x, mtri->uv[2].y);
		//create_lightmap_for_entity(mtri->entity, world);
		//create_map_for_entity(mtri->entity, world);
		i++;
	}
}

void	room_makefloor(t_world *world, t_area *room)
{
	//free_floor(world, room);
	_room_triangulate_floors(world, room);
}

void	room_makeceilings(t_world *world, t_area *room)
{
	int			i;
	t_meshtri	*mtri;
	t_object	*obj;

	i = 0;
	while (i < room->floorcount)
	{
		mtri = &room->ceilings[i];
		obj = object_tri(world->sdl);
		obj->materials->img = get_image_by_name(*world->sdl, room->s_ceiltex.str);
		mtri->v[0] = room->floors[i].v[0];
		mtri->v[1] = room->floors[i].v[1];
		mtri->v[2] = room->floors[i].v[2];
		mtri->v[0].z += room->ceiling_height;
		mtri->v[1].z += room->ceiling_height;
		mtri->v[2].z += room->ceiling_height;
		mtri->uv[0] = room->floors[i].uv[0];
		mtri->uv[1] = room->floors[i].uv[1];
		mtri->uv[2] = room->floors[i].uv[2];
		applytrimesh(*mtri, obj);
		mtri->entity = spawn_entity(world, obj);
		mtri->entity->rigid = true;
		update_floor_bounds(mtri);
		i++;
	}
	room->ceilingcount = i;
	
	/*free_floor(world, room);
	room->floors = ft_memalloc(sizeof(t_meshtri) * 1000);
	_room_triangulate_floors(world, room);*/
}
