/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_tool_rooms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 03:20:37 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/09 15:27:56 by okinnune         ###   ########.fr       */
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


static float anglebetween(t_vector2 first, t_vector2 second)
{
	second = vector2_sub(second, first);
	float	dot = first.x * second.x + first.y * second.y;
	float	det = first.x * second.y - first.y * second.y;
	return (atan2(dot, det));
}

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

void	floorcalc_debugdraw(t_editor *ed, t_sdlcontext *sdl, t_floorcalc fc, int tri_i)
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
	ws = (t_vector3){fc.edges[fc.faces[i].v_indices[0]].x,
					fc.edges[fc.faces[i].v_indices[0]].y, 0.0f};
	ws_to = (t_vector3){fc.edges[fc.faces[i].v_indices[1]].x,
					fc.edges[fc.faces[i].v_indices[1]].y, 0.0f};
	render_ray(*sdl, ed->render, ws, ws_to);
	ws = (t_vector3){fc.edges[fc.faces[i].v_indices[1]].x,
					fc.edges[fc.faces[i].v_indices[1]].y, 0.0f};
	ws_to = (t_vector3){fc.edges[fc.faces[i].v_indices[2]].x,
					fc.edges[fc.faces[i].v_indices[2]].y, 0.0f};
	render_ray(*sdl, ed->render, ws, ws_to);
	/*CREATED LINE */
	t_vector2 first = fc.edges[fc.faces[i].v_indices[2]];
	t_vector2 center = fc.edges[fc.faces[i].v_indices[1]];
	t_vector2 second = fc.edges[fc.faces[i].v_indices[0]];
	char *stra = ft_itoa(radtodeg(anglebetween(second, first)) + 90);
	draw_text_boxed(sdl, stra, point_div(sdl->screensize, 2), sdl->screensize);
	free(stra);
	ed->render.gizmocolor = CLR_BLUE;
	ws = (t_vector3){fc.edges[fc.faces[i].v_indices[2]].x,
					fc.edges[fc.faces[i].v_indices[2]].y, 0.0f};
	ws_to = (t_vector3){fc.edges[fc.faces[i].v_indices[0]].x,
					fc.edges[fc.faces[i].v_indices[0]].y, 0.0f};
	render_ray(*sdl, ed->render, ws, ws_to);

	t_vector2 *tr = transformed_around(second, RAD90 + ft_degtorad(106.0f), fc.edges, fc.edgecount);
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
	temp1 = vector2_add(tr[fc.faces[tri_i].v_indices[2]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
	temp2 = vector2_add(tr[fc.faces[tri_i].v_indices[0]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
	drawline(*sdl, vector2_to_point(temp1), vector2_to_point(temp2), CLR_BLUE);
	temp1 = vector2_add(tr[fc.faces[tri_i].v_indices[0]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
	temp2 = vector2_add(tr[fc.faces[tri_i].v_indices[1]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
	drawline(*sdl, vector2_to_point(temp1), vector2_to_point(temp2), CLR_PRPL);
	temp1 = vector2_add(tr[fc.faces[tri_i].v_indices[1]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
	temp2 = vector2_add(tr[fc.faces[tri_i].v_indices[2]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
	drawline(*sdl, vector2_to_point(temp1), vector2_to_point(temp2), CLR_PRPL);
	temp1 = vector2_add(tr[fc.faces[tri_i].v_indices[1]],(t_vector2){sdl->window_w / 2, sdl->window_h / 2});
	drawcircle(*sdl, vector2_to_point(temp1), 5, CLR_BLUE);
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

/*static	bool is_ear(int *valid, int n_i, int i, int p_i, int count)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		
	}
}*/

static	void removevalid (int *valid, int count, int ri)
{
	int	i;

	i = 0;
	printf("removing %i \n", ri);
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

void	triangulate(t_floorcalc *fc)
{
	int	valid[MAXSELECTED];
	int	validcount;
	
	int i = 0;
	int	n_i;
	int	p_i;
	fc->facecount = 0;
	i = 0;
	while (i < fc->edgecount)
	{
		valid[i] = i;
		i++;
	}
	validcount = fc->edgecount;
	i = 0;
	printvalid(valid, validcount);
	printf("\nMAKING NEW FACES: \n");
	while (validcount > 2) //while validcount > 3?
	{
		fc->faces[fc->facecount].v_indices[0] = valid[validcount - 1];
		fc->faces[fc->facecount].v_indices[1] = valid[0];
		fc->faces[fc->facecount].v_indices[2] = valid[1];
		/*
		t_vector2 first = fc->edges[fc->faces[i].v_indices[0]];
		t_vector2 second = fc->edges[fc->faces[i].v_indices[1]];
		anglebetween transform
		*/
		
		printf("connect %i %i %i \n", valid[validcount - 1], valid[0], valid[1]);
		removevalid(valid, validcount--, 0);
		//validcount--;
		printvalid(valid, validcount);
		fc->facecount++;
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
			if (isunique(fc, dat->selected[i]->line.start))
				fc.edges[fc.edgecount++] = dat->selected[i]->line.start;
			else if (isunique(fc, dat->selected[i]->line.end))
				fc.edges[fc.edgecount++] = dat->selected[i]->line.end;
		}
		i++;
	}
	triangulate(&fc);
	return (fc);
}