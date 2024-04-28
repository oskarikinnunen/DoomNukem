/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_round.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:37:29 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/21 16:40:11 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector2	vector2_ceilf(t_vector2 v)
{
	v.x = ceilf(v.x);
	v.y = ceilf(v.y);
	return (v);
}

t_vector2	vector2_floorf(t_vector2 v)
{
	v.x = floorf(v.x);
	v.y = floorf(v.y);
	return (v);
}

t_vector2	vector2_roundf(t_vector2 v)
{
	v.x = roundf(v.x);
	v.y = roundf(v.y);
	return (v);
}

t_vector2	vector2_truncf(t_vector2 v)
{
	v.x = truncf(v.x);
	v.y = truncf(v.y);
	return (v);
}
