/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_anims.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:51:14 by raho              #+#    #+#             */
/*   Updated: 2023/03/13 17:15:02 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"

static char	*basename(char *filename)
{
	char	**split;
	char	*result;
	int		i;

	split = ft_strsplit(filename, '.');
	if (split == NULL)
		doomlog(LOG_EC_MALLOC, "parseanim basename");
	i = 1;
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	result = split[0];
	free(split);
	return (result);
}

static char	*glue_anim_name(char *base, char *animname, int i)
{
	char	name[256];
	char	*newname;

	ft_strcpy(name, "assets/objects/animations/");
	ft_strcat(name, base);
	ft_strcat(name, "_");
	ft_strcat(name, animname);
	ft_strcat(name, s_itoa(i));
	ft_strcat(name, ".obj");
	newname = ft_memalloc(sizeof(name));
	if (newname == NULL)
		doomlog(LOG_EC_MALLOC, "parseanim anim_name");
	return (ft_strcpy(newname, name));
}

void	parse_animframe(int fd, t_objectanimframe *frame, t_object *object)
{
	t_list		*verticelist;
	t_vector3	*vertices;
	uint32_t	vertexcount;
	int			i;

	verticelist = get_vertex_list(fd);
	vertices = list_to_ptr(verticelist, &vertexcount);
	i = 0;
	frame->deltavertices = ft_memalloc(sizeof(t_deltavertex) * vertexcount);
	if (frame->deltavertices == NULL)
		doomlog(LOG_EC_MALLOC, "frame->deltavertices");
	while (i < vertexcount)
	{
		frame->deltavertices[i].delta = vector3_sub(vertices[i], \
													object->vertices[i]);
		frame->deltavertices[i].v_index = i;
		i++;
	}
}

void	editor_load_anims(t_object *object, char *anim_name, int i)
{
	int					fd;
	char				*name;
	t_objectanimframe	frame;
	char				*base;

	fd = 0;
	base = basename(object->name);
	object->o_anim.framecount = 0;
	while (fd != -1)
	{
		name = glue_anim_name(base, anim_name, i);
		fd = open(name, O_RDONLY);
		if (fd != -1)
		{
			ft_strcpy(object->o_anim.name, anim_name);
			parse_animframe(fd, &frame, object);
			ptr_add((void **)&object->o_anim.frames, \
				&object->o_anim.framecount, sizeof(t_objectanimframe), &frame);
			fileclose(fd, name);
		}
		free(name);
		i++;
	}
	free(base);
}
