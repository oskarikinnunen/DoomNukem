/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_triangulate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:35:35 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 12:23:29 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "collision.h"
#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "objects.h"


bool	points_collide(t_floorcalc *fc, t_vector2 tri[3])
{
	int					i;
	t_world_triangle	t;
	t_point				p;

	t.p[0].v = v2tov3(tri[0]);
	t.p[1].v = v2tov3(tri[1]);
	t.p[2].v = v2tov3(tri[2]);
	i = 0;
	while (i < fc->edge_count)
	{
		if (vector2_cmp(fc->edges[i], tri[0])
			|| vector2_cmp(fc->edges[i], tri[1])
			|| vector2_cmp(fc->edges[i], tri[2]))
		{
			i++;
			continue ;
		}
		p = vector2_to_point(fc->edges[i]);
		if (collision_point_triangle(p, t))
			return (true);
		i++;
	}
	return (false);
}

t_line	line_shorten(t_line line)
{
	t_vector2	temp;
	t_vector2	delta_start;
	t_vector2	delta_end;
	t_line		final;
	float		dist;

	dist = vector2_dist(line.start, line.end);
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

void	free_object(t_object **object)
{
	if (*object == NULL)
		return ;
	if ((*object)->faces != NULL)
		free((*object)->faces);
	if ((*object)->materials != NULL)
		free((*object)->materials);
	if ((*object)->uvs != NULL)
		free((*object)->uvs);
	if ((*object)->vertices != NULL)
		free((*object)->vertices);
	free(*object);
	*object = NULL;	
}

void	free_floor(t_world *world, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->floor_count)
	{
		if (room->floors[i].entity != NULL)
		{
			if (room->floors[i].entity->obj != NULL)
			{
				free_object(&room->floors[i].entity->obj);
				room->floors[i].entity->obj = NULL;
			}
			destroy_entity(world, room->floors[i].entity);
			room->floors[i].entity = NULL;
		}
		i++;
	}
}

void	free_ceilings(t_world *world, t_area *room)
{
	int	i;

	i = 0;
	while (i < room->ceiling_count)
	{
		if (room->ceilings[i].entity != NULL)
		{
			if (room->ceilings[i].entity->obj != NULL)
			{
				free_object(&room->ceilings[i].entity->obj);
				room->ceilings[i].entity->obj = NULL;
			}
			destroy_entity(world, room->ceilings[i].entity);
			room->ceilings[i].entity = NULL;
		}
		i++;
	}
}


void	shiftvalid(int valid[32], int count)
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

void	shiftvalid_left(int *valid, int count)
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
	t_vector2	align1;
	t_vector2	align2;

	align1 = vector2_sub(vs[1], vs[2]);
	align2 = vector2_sub(vs[1], vs[0]);
	align1 = vector2_normalise(align1);
	align2 = vector2_normalise(align2);
	align1 = vector2_abs(align1);
	align2 = vector2_abs(align2);
	return (vector2_cmp(align1, align2));
}

void	populatevalid(int valid[32], int *validcount, t_floorcalc fc)
{
	*validcount = 0;
	while (*validcount < fc.edge_count)
	{
		valid[*validcount] = *validcount;
		*validcount = *validcount + 1;
	}
}

void	populatevalid_l(int valid[32], int *validcount, t_floorcalc fc)
{
	*validcount = 0;
	while (*validcount < fc.edge_count)
	{
		valid[*validcount] = fc.edge_count - *validcount - 1;
		*validcount = *validcount + 1;
	}
}


static t_vector2	rot_vec(t_vector2 v, float cos, float sin)
{
	t_vector2	temp;

	temp = v;
	temp.x = (v.x * cos) - (v.y * sin);
	temp.y = (v.y * cos) + (v.x * sin);
	temp.x *= -1.0f;
	return (temp);
}

t_vector2	*transformed_around(t_vector2 og, float angle, t_vector2 *t)
{
	static t_vector2	final[MAXSELECTED];
	int					i;
	t_vector2			min;

	min = (t_vector2){4000, 4000};
	ft_memcpy(final, t, sizeof(t_vector2[3]));
	i = 0;
	while (i < 3)
	{
		if (final[i].x < min.x)
			min.x = final[i].x;
		if (final[i].y < min.y)
			min.y = final[i].y;
		i++;
	}
	i = 0;
	while (i < 3)
	{
		final[i] = vector2_sub(final[i], og);
		final[i] = rot_vec(final[i], cosf(angle), sinf(angle));
		i++;
	}
	return (final);
}

bool	intersect(t_line line1, t_vector2 *edges, int edge_count)
{
	t_line	line2;
	int		i;
	float	res;

	i = 0;
	while (i < edge_count)
	{
		line2.start = edges[i];
		if (i == edge_count - 1)
			line2.end = edges[0];
		else
			line2.end = edges[i + 1];
		if (collision_line_line_intersect(line1, line_shorten(line2)))
			return (true);
		i++;
	}
	return (false);
}

//Might not NEED to exit but why not, better safe than sorry
//													(or segfault)
bool	correctangle(t_vector2 vs[3])
{
	t_vector2	*tr;
	float		angle;

	angle = vector2_anglebetween(vs[0], vs[2]);
	if (isinf(angle) || isnan(angle))
		doomlog(LOG_FATAL, "Invalid angle in floor/ceiling");
	tr = transformed_around(vs[2], -angle, vs);
	return (tr[1].y >= tr[0].y && tr[1].y >= tr[2].y);
}

void	removevalid(int *valid, int count, int ri)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		if (i >= ri)
			valid[i] = valid[i + 1];
		i++;
	}
}


t_ear	get_ear(int *valid, int validcount, t_floorcalc *fc)
{
	t_ear	ear;

	ear.first = fc->edges[valid[validcount - 1]];
	ear.center = fc->edges[valid[0]];
	ear.second = fc->edges[valid[1]];
	if (isaligned((t_vector2 *)&ear))
	{
		ear.center.x = 0.0001f;
		ear.center.y = -0.0001f;
	}
	return (ear);
}

static void	shift_direction(int *valid, int validcount, int i)
{
	if ((i >= 25000 && i < 50000) || (i >= 75000 && i < 100000))
		shiftvalid(valid, validcount);
	else
		shiftvalid_left(valid, validcount);
}

bool	fc_add_face(t_floorcalc *fc, int *valid, int *validcount)
{
	t_ear	ear;
	t_line	line1;

	line1.start = fc->edges[valid[*validcount - 1]];
	line1.end = fc->edges[valid[1]];
	ear = get_ear(valid, *validcount, fc);
	if (!points_collide(fc, (t_vector2 *)&ear)
		&& correctangle((t_vector2 *)&ear)
		&& !intersect(line_shorten(line1), fc->edges, fc->edge_count))
	{
		fc->faces[fc->face_count].v_indices[0] = valid[*validcount - 1];
		fc->faces[fc->face_count].v_indices[1] = valid[0];
		fc->faces[fc->face_count].v_indices[2] = valid[1];
		removevalid(valid, *validcount, 0);
		*validcount -= 1;
		if (*validcount == 2)
		{
			ear.first = fc->edges[valid[*validcount - 1]];
			ear.center = fc->edges[valid[0]];
			ear.second = fc->edges[valid[1]];
		}
		fc->face_count++;
		return (true);
	}
	return (false);
}

//TODO: find case where iteration protection triggers, check that
// it handles correctly (doesn't segfault etc.)
void	triangulate(t_floorcalc *fc)
{
	int		valid[MAXSELECTED];
	int		validcount;
	int		i;
	t_ear	ear;

	validcount = 0;
	populatevalid(valid, &validcount, *fc);
	i = 0;
	fc->face_count = 0;
	while (validcount > 2 && i <= 100000)
	{
		if (!fc_add_face(fc, valid, &validcount))
			shift_direction(valid, validcount, i);
		i++;
		if (i == 50000)
		{
			populatevalid_l(valid, &validcount, *fc);
			fc->face_count = 0;
		}
		if (i > 100000)
		{
			fc->face_count = 0;
			doomlog(LOG_WARNING, "Couldn't create floor");
		}
	}
}
