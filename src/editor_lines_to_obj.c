/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_lines_to_obj.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 11:57:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 20:30:37 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "v3lib.h"

static void linefaces(t_obj *obj, uint32_t i)
{
	int	fi;

	fi = i;
	if (i > 0)
		fi = i / 2;
	obj->faces[fi] = ft_memalloc(3 * sizeof (uint32_t));
	if (!obj->faces[fi])
		error_log(EC_MALLOC);
	obj->faces[fi][0] = i;
	obj->faces[fi][1] = i + 1;
	obj->faces[fi][2] = i + 2;
	fi++;
	obj->faces[fi] = ft_memalloc(3 * sizeof (uint32_t));
	if (!obj->faces[fi])
		error_log(EC_MALLOC);
	obj->faces[fi][0] = i + 1;
	obj->faces[fi][1] = i + 2;
	obj->faces[fi][2] = i + 3;
}

static void	i3_mul(t_obj *obj, int i)
{
	int	k;
	int	*i3;

	k = 0;
	while (k < 4)
	{
		i3 = obj->verts[i + k];
		i3[X] = i3[X] * TILESIZE;
		i3[Y] = i3[Y] * TILESIZE;
		i3[Z] = i3[Z] * TILESIZE;
		k++;
	}
}

static void	allocate_and_copy_verts(t_obj *obj, t_line line, int i) //Ghetto, TODO: make cpyv2fromv3 function?
{
	obj->verts[i] = ft_memalloc(3 * sizeof(int32_t *));
	obj->verts[i + 1] = ft_memalloc(3 * sizeof(int32_t *));
	obj->verts[i + 2] = ft_memalloc(3 * sizeof(int32_t *));
	obj->verts[i + 3] = ft_memalloc(3 * sizeof(int32_t *));
	if (!obj->verts[i] || !obj->verts[i + 1] || \
		!obj->verts[i + 2] || !obj->verts[i + 3])
		error_log(EC_MALLOC);
	obj->verts[i][X] = line.start[X];
	obj->verts[i][Y] = line.start[Y];
	obj->verts[i + 1][X] = line.end[X];
	obj->verts[i + 1][Y] = line.end[Y];
	obj->verts[i + 2][X] = line.start[X];
	obj->verts[i + 2][Y] = line.start[Y];
	obj->verts[i + 3][X] = line.end[X];
	obj->verts[i + 3][Y] = line.end[Y];
}

static void	create_verts_faces_colors(t_obj *obj)
{
	obj->verts = ft_memalloc(obj->v_count * sizeof(int32_t *));
	if (!obj->verts)
		error_log(EC_MALLOC);
	obj->faces = ft_memalloc(obj->f_count * sizeof(uint32_t *));
	if (!obj->faces)
		error_log(EC_MALLOC);
	obj->mtlcolors = ft_memalloc(sizeof(uint32_t));
	if (!obj->mtlcolors)
		error_log(EC_MALLOC);
	obj->mtlcolors[0] = CLR_PRPL;
}

void	lines_to_obj(t_obj *obj, t_editor *ed)
{
	t_list		*l;
	int			len;
	uint32_t	i;

	l = ed->linelist;
	len = ft_listlen(l);
	obj->v_count = len * 4;
	obj->f_count = len * 2;
	create_verts_faces_colors(obj);
	i = 0;
	while (l != NULL)
	{
		allocate_and_copy_verts(obj, *((t_line *)l->content), i);
		obj->verts[i + 2][Z] = -5; //HEIGHT OF THE WALL
		obj->verts[i + 3][Z] = -5;
		i3_mul(obj, i);
		linefaces(obj, i);
		l = l->next;
		i += 4;
	}
}