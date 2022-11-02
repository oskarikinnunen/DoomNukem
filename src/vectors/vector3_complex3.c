/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_complex3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:21:41 by vlaine            #+#    #+#             */
/*   Updated: 2022/11/02 14:35:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_vector3	vector3_movetowards(t_vector3 vec, t_vector3 dir, float delta)
{
	dir = vector3_clamp_magnitude(dir, delta);
	return (vector3_add(vec, dir));
}