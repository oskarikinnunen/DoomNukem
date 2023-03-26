/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 11:54:48 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "file_io.h"

void	parse_animframe(int fd, t_object_anim_frame *frame, t_object *object)
{
	t_list		*verticelist;
	t_vector3	*vertices;
	uint32_t	vertexcount;
	int			i;

	vertexcount = 0;
	verticelist = get_vertex_list(fd);
	vertices = list_to_ptr(verticelist, &vertexcount);
	listdel(&verticelist);
	i = 0;
	frame->deltavertices = prot_memalloc(sizeof(t_deltavertex) * vertexcount);
	while (i < vertexcount)
	{
		frame->deltavertices[i].delta = vector3_sub(vertices[i], \
													object->vertices[i]);
		frame->deltavertices[i].v_index = i;
		i++;
	}
	free(vertices);
	frame->vertcount = vertexcount;
}

void	parse_anim(char *anim_path, char *anim_name, t_object *object)
{
	int					fd;
	t_object_anim_frame	frame;

	ft_bzero(&frame, sizeof(t_object_anim_frame));
	fd = ft_fileopen(anim_path, O_RDONLY);
	parse_animframe(fd, &frame, object);
	object->o_anim.frames[object->o_anim.framecount] = frame;
	object->o_anim.framecount++;
	ft_fileclose(fd, anim_path);
}
