/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool_rooms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 03:20:37 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/11 13:39:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "inputhelp.h"
#include "tools/walltool.h"
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
		
		if (fabsf(res) > 0.01f)
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
					printf("\nLINE COLLIDES WITH %i->%i \n", i, i + 1);
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

	tr = transformed_around(vs[2], -angle, vs, 3);
	return (tr[1].y >= tr[0].y && tr[1].y >= tr[2].y);
}

static void indexesdebug(t_sdlcontext *sdl, t_floorcalc fc, int i)
{
	char	*str= ft_itoa(fc.faces[i].v_indices[0]);
	draw_text_boxed(sdl, str, (t_point) {300, 240}, sdl->screensize);
	free(str);
	str= ft_itoa(fc.faces[i].v_indices[1]);
	draw_text_boxed(sdl, str, (t_point) {350, 240}, sdl->screensize);
	free(str);
	str= ft_itoa(fc.faces[i].v_indices[2]);
	draw_text_boxed(sdl, str, (t_point) {400, 240}, sdl->screensize);
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
	draw_text_boxed(sdl, str, (t_point) {30, 400}, sdl->screensize);*/
	while (i < fc.edgecount)
	{
		ws = (t_vector3){fc.edges[i].x, fc.edges[i].y, 0.0f};
		ss = vector3_to_screenspace(ed->render, ws, *sdl);
		str = ft_itoa(i);
		draw_text_boxed(sdl, str, ss, sdl->screensize);
		free (str);
		ed->render.gizmocolor = CLR_RED;
		render_gizmo(*sdl, ed->render, ws, 10);
		i++;
	}
	i = tri_i;
	ed->render.gizmocolor = CLR_PRPL;
	/*ws = (t_vector3){fc.edges[fc.faces[i].v_indices[0]].x,
					fc.edges[fc.faces[i].v_indices[0]].y, 0.0f};
	ws_to = (t_vector3){fc.edges[fc.faces[i].v_indices[1]].x,
					fc.edges[fc.faces[i].v_indices[1]].y, 0.0f};
	render_ray(*sdl, ed->render, ws, ws_to);
	ws = (t_vector3){fc.edges[fc.faces[i].v_indices[1]].x,
					fc.edges[fc.faces[i].v_indices[1]].y, 0.0f};
	ws_to = (t_vector3){fc.edges[fc.faces[i].v_indices[2]].x,
					fc.edges[fc.faces[i].v_indices[2]].y, 0.0f};
	render_ray(*sdl, ed->render, ws, ws_to);*/
	
	/*CREATED LINE */
	t_vector2 first = fc.edges[fc.faces[i].v_indices[0]];
	t_vector2 center = fc.edges[fc.faces[i].v_indices[1]];
	t_vector2 second = fc.edges[fc.faces[i].v_indices[2]];
	indexesdebug(sdl, fc, i);
	
	/*ws = (t_vector3){fc.edges[fc.faces[i].v_indices[2]].x,
					fc.edges[fc.faces[i].v_indices[2]].y, 0.0f};
	ws_to = (t_vector3){fc.edges[fc.faces[i].v_indices[0]].x,
					fc.edges[fc.faces[i].v_indices[0]].y, 0.0f};
	render_ray(*sdl, ed->render, ws, ws_to);*/

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
	draw_text_boxed(sdl, stra, (t_point) {300, 200}, sdl->screensize);
	free(stra);
	stra = ft_itoa(radtodeg(a2)); //Internal angle M_PI - ((a1 / M_PI) + (a3 / M_PI)))
	draw_text_boxed(sdl, stra, (t_point) {350, 200}, sdl->screensize);
	free(stra);
	stra = ft_itoa(radtodeg(a3));
	draw_text_boxed(sdl, stra, (t_point) {400, 200}, sdl->screensize);
	free(stra);
	stra = ft_itoa(radtodeg(angleworld));
	draw_text_boxed(sdl, stra, (t_point) {400, 300}, sdl->screensize);
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
/*		temp1 = vector2_add(tr[fc.faces[i].v_indices[0]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		temp2 = vector2_add(tr[fc.faces[i].v_indices[1]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		drawline(*sdl, vector2_to_point(temp1), vector2_to_point(temp2), CLR_PRPL);
		temp1 = vector2_add(tr[fc.faces[i].v_indices[1]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		temp2 = vector2_add(tr[fc.faces[i].v_indices[2]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		drawline(*sdl, vector2_to_point(temp1), vector2_to_point(temp2), CLR_PRPL);*/
		temp1 = vector2_add(tr[fc.faces[i].v_indices[1]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
		if (i == tri_i)
			drawcircle(*sdl, vector2_to_point(temp1), 5, CLR_BLUE);
		else
			drawcircle(*sdl, vector2_to_point(temp1), 5, CLR_RED);
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
	printf("removing ear: %i \n", valid[ri]);
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


static	void shiftvalid(int *valid, int count)
{
	int	temp;
	int	f_temp;
	int	i;

	f_temp = valid[0];
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

void	triangulate(t_floorcalc *fc)
{
	int	valid[MAXSELECTED];
	int	validcount;
	int	i;

	validcount = 0;
	while (validcount < fc->edgecount)
	{
		valid[validcount] = validcount;
		validcount++;
	}
	i = 0;
	fc->facecount = 0;
	printf("\nMAKING NEW FACES: \n");
	while (validcount > 2)
	{
		printf("try %i %i %i \n", valid[validcount - 1], valid[0], valid[1]);
		t_line line1;
		line1.start = fc->edges[valid[validcount - 1]];
		line1.end = fc->edges[valid[1]];
		t_vector2 first = fc->edges[valid[validcount - 1]];
		t_vector2 center = fc->edges[valid[0]];
		t_vector2 second = fc->edges[valid[1]];
		if (correctangle((t_vector2[3]){first,center,second}) && !intersect(line1, fc->edges, fc->edgecount))
		{
			fc->faces[fc->facecount].v_indices[0] = valid[validcount - 1];
			fc->faces[fc->facecount].v_indices[1] = valid[0];
			fc->faces[fc->facecount].v_indices[2] = valid[1];
			printf("connect %i %i %i \n", valid[validcount - 1], valid[0], valid[1]);
			removevalid(valid, validcount--, 0);
			fc->facecount++;
		}
		else
			shiftvalid(valid, validcount);
		printvalid(valid, validcount);
		i++;
		if (i > 300)
			break ;
	}
	printf("made %i faces \n", fc->facecount);
}

t_floorcalc	generate_floor(t_walltooldata *dat)
{
	t_floorcalc	fc;
	int			i;

	i = 0;
	fc.edgecount = 0;
	ft_bzero(&fc, sizeof(fc));
	while (i < MAXSELECTED)
	{
		if (dat->selected[i] != NULL)
		{
			if (i == 0)
			{
				fc.edges[fc.edgecount++] = dat->selected[i]->line.start;
				fc.edges[fc.edgecount++] = dat->selected[i]->line.end;
			}
			else
			{
				if (isunique(fc, dat->selected[i]->line.start))
					fc.edges[fc.edgecount++] = dat->selected[i]->line.start;
				else if (isunique(fc, dat->selected[i]->line.end))
					fc.edges[fc.edgecount++] = dat->selected[i]->line.end;
			}
			
		}
		i++;
	}
	triangulate(&fc);
	return (fc);
}