/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_triangulate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:35:35 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:36:42 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "collision.h"
#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "objects.h"

static bool	iteration_protect(int *valid, int validcount,
		int i, t_floorcalc *fc)
{
	if (i == 50000)
	{
		populatevalid_l(valid, &validcount, *fc);
		fc->face_count = 0;
	}
	if (i > 100000)
	{
		fc->face_count = 0;
		doomlog(LOG_WARNING, "Couldn't create floor, too many iterations!");
		return (true);
	}
	return (false);
}

typedef struct s_ear
{
	t_vector2	first;
	t_vector2	center;
	t_vector2	second;
}	t_ear;

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
	while (validcount > 2)
	{
		if (!fc_add_face(fc, valid, &validcount))
			shift_direction(valid, validcount, i);
		i++;
		if (iteration_protect(valid, validcount, i, fc))
			return ;
	}
}
