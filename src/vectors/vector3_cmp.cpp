/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_cmp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 11:45:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 16:36:48 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

bool	vector3_cmp(t_vector3 first, t_vector3 second)
{
	return ((first.x == second.x)
		&& (first.y == second.y) && (first.z == second.z));
}
