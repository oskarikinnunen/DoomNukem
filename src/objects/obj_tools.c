/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:16:28 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 13:17:48 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

t_vector2	flipped_uv(t_vector2 vec)
{
	t_vector2	uv;

	uv.x = -vec.y;
	uv.y = vec.x;
	return (uv);
}
