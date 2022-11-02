/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_complex3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:21:41 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/02 18:48:00 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

float flerp(float from, float to, float delta)
{
	return (from + ((to - from) * delta));
}

t_vector3 vector3_lerp(t_vector3 from, t_vector3 to, float delta)
{
	t_vector3	result;

	result.x = flerp(from.x, to.x, delta);	
	result.y = flerp(from.y, to.y, delta);
	result.z = flerp(from.z, to.z, delta);

	return(result);
}
