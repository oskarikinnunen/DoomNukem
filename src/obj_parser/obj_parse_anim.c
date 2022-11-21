/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_anim.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:44:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/21 19:50:43 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "doomnukem.h"
#include "objects.h"
#include "vectors.h"

void	parse_animframe(int fd, t_objectanimframe *frame, t_object *object)
{
	t_list		*verticelist;
	t_vector3	*vertices;
	uint32_t	vertexcount;
	int			i;

	verticelist = get_vertex_list(fd);
	list_to_ptr(vertices, &vertexcount);
	i = 0;
	frame->deltavertices = ft_memalloc(sizeof(t_vector3) * vertexcount);
	while (i < vertexcount)
	{
		frame->deltavertices[i].delta = vector3_sub(object->vertices[i], vertices[i]);
		frame->deltavertices[i].v_index = i;
		i++;
	}
}