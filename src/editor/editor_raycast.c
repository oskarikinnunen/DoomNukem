/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_raycast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 18:03:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/05 18:04:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "doomnukem.h"

t_vector3	raycast(t_editor *ed)
{
	t_vector3	result;
	t_vector3	rayforward;
	int			iter;
	float		dist;

	result = ed->position;
	iter = 0;
	dist = sqrt(powf(ed->position.z, 2.0f) + powf(sin(ft_clampf(ed->angle.y, -RAD90, -0.1f)), 2.0f)); //not correct lol
	rayforward = ed->forward;
	rayforward.z = ft_clampf(rayforward.z, -1.0f, -0.001f);
	while (result.z > 0.0f)
	{
		result = vector3_movetowards(result, rayforward, dist);
		iter++;
	}
	result.x = ft_clampf(result.x, 0.0f, 1000.0f);
	result.y = ft_clampf(result.y, 0.0f, 1000.0f);
	result.z = 0.0f;
	return (result);
}
