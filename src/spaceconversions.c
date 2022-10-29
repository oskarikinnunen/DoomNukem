/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaceconversions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:31:43 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/26 13:34:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "vectors.h"

t_point worldtoeditorspace(t_editor *ed, t_vector2 worldcrd)
{
	t_point	result;

	result = vector2_to_point(worldcrd);
	result = point_add(result, ed->offset);
	return (result);
}
