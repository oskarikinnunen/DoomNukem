/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_triangulate_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:00:02 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:36:42 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "collision.h"
#include "tools/walltool.h"
#include "tools/roomtool.h"
#include "objects.h"

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
