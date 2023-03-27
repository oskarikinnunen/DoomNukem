/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_triangulate_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:59:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 11:03:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "collision.h"
#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "objects.h"

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
