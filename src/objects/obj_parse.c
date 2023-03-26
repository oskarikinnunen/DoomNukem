/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:21:41 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "file_io.h"

t_list	*get_material_list(int fd)
{
	char	*line;
	t_list	*list;
	int		ret;

	list = NULL;
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strnstr(line, "mtllib ", sizeof("mtllib")) != NULL)
				parse_mtllib(&list, line + sizeof("mtllib"));
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "get_material_list");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "get_material_list");
	return (list);
}

void	obj_save_vertices_uvs_faces(t_object *object, t_object_parse *op)
{
	object->vertices = list_to_ptr(op->vertices, &object->vertice_count);
	object->uvs = list_to_ptr(op->uvs, &object->uv_count);
	object->faces = list_to_ptr(op->faces, &object->face_count);
	ft_lstdel(&op->vertices, free_list_node);
	ft_lstdel(&op->uvs, free_list_node);
	ft_lstdel(&op->faces, free_list_node);
}

//TODO: return a crash bandicoot if open failed
t_object	objparse(char *filename)
{
	t_object		result;
	t_object_parse	op;
	int				fd;

	fd = ft_fileopen(filename, O_RDONLY);
	ft_bzero(&result, sizeof(t_object));
	op.materials = get_material_list(fd);
	op.vertices = get_vertex_list(fd);
	op.uvs = get_uv_list(fd);
	op.faces = get_face_list(fd, op.materials);
	ft_fileclose(fd, filename);
	result.materials = list_to_ptr(op.materials, &result.material_count);
	if (result.materials == NULL)
		result.materials = prot_memalloc(sizeof(t_material));
	ft_lstdel(&op.materials, free_list_node);
	obj_save_vertices_uvs_faces(&result, &op);
	return (result);
}
