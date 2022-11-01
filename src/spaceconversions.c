/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaceconversions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:31:43 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/01 15:07:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "vectors.h"

t_point worldtoeditorspace(t_editor *ed, t_vector2 worldcrd)
{
	t_point	result;

	result = vector2_to_point(worldcrd);
	//result = point_add(result, ed->offset);
	return (result);
}
