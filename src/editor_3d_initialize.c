/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_3d_initialize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 11:57:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/07 10:43:33 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "v3lib.h"

static void setfaceindex(uint32_t **face, int fi)
{
	int	indx[2];
	int	i;

	i = 0;
	while (i < 3)
	{
		indx[0] = ((i >= 2) * GRIDSIZE) + !((i + 1) % 2);
		indx[1] = ((i + 1 >= 2) * GRIDSIZE) + !((i + 2) % 2);
		face[0][i] = fi + indx[0];
		face[1][i] = fi + indx[1];
		i++;
	}
}

static void generatefaces(t_obj *obj)
{
	int	len;
	int	i;
	int	ri;

	len = GRIDSIZE * GRIDSIZE;
	obj->f_count = (len - GRIDSIZE - (len - GRIDSIZE) / GRIDSIZE) * 2; //FACECOUNT, not tricount?
	obj->f_count -= GRIDSIZE;
	obj->faces = ft_memalloc(obj->f_count * sizeof(uint32_t *));
	i = 0;
	ri = 0;
	while (i < obj->f_count)
	{
		obj->faces[i] = ft_memalloc(sizeof(uint32_t [3]));
		obj->faces[i + 1] = ft_memalloc(sizeof(uint32_t [3]));
		setfaceindex(&obj->faces[i], ri);
		i += 2;
		ri++;
		ri += (ri + 1) % GRIDSIZE == 0;
	}
}

void gridto_obj(t_obj *obj)
{
	int	crd[2];

	obj->verts = ft_memalloc(GRIDSIZE * GRIDSIZE * sizeof(float *));
	obj->v_count = 0;
	obj->mtlcolors = ft_memalloc(sizeof(uint32_t));
	obj->mtlcolors[0] = CLR_GRAY;
	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < TILESIZE * GRIDSIZE)
	{
		crd[X] = 0;
		while (crd[X] < TILESIZE * GRIDSIZE)
		{
			obj->verts[obj->v_count] = ft_memalloc(sizeof(int32_t [3]));
			obj->verts[obj->v_count][X] = crd[X];
			obj->verts[obj->v_count][Y] = crd[Y];
			obj->v_count++;
			crd[X] += TILESIZE;
		}
		crd[Y] += TILESIZE;
	}
	generatefaces(obj);
}

static void linefaces(t_obj *obj, uint32_t i)
{
	//TODO: Protect allocations plz
	int	fi;

	fi = i;
	if (i > 0)
		fi = i / 2;
	obj->faces[fi] = ft_memalloc(3 * sizeof (uint32_t));
	obj->faces[fi][0] = i;
	obj->faces[fi][1] = i + 1;
	obj->faces[fi][2] = i + 2;
	fi++;
	obj->faces[fi] = ft_memalloc(3 * sizeof (uint32_t));
	obj->faces[fi][0] = i + 1;
	obj->faces[fi][1] = i + 2;
	obj->faces[fi][2] = i + 3;
}

static void	i3_mul(int *i3, int mul)
{
	i3[X] = i3[X] * mul;
	i3[Y] = i3[Y] * mul;
	i3[Z] = i3[Z] * mul;
}

static void copyverts(t_obj *obj, t_line line, int i) //Ghetto, TODO: make cpyv2fromv3 function?
{
	obj->verts[i][X] = line.start[X];
	obj->verts[i][Y] = line.start[Y];
	obj->verts[i + 1][X] = line.end[X];
	obj->verts[i + 1][Y] = line.end[Y];
	obj->verts[i + 2][X] = line.start[X];
	obj->verts[i + 2][Y] = line.start[Y];
	obj->verts[i + 3][X] = line.end[X];
	obj->verts[i + 3][Y] = line.end[Y];
}

void lines_to_obj(t_obj *obj, t_editor *ed)
{
	t_list		*l;
	int			len;
	uint32_t	i;

	l = ed->linelist;
	len = ft_listlen(l);
	obj->v_count = len * 4;
	obj->verts = ft_memalloc(obj->v_count * sizeof(int32_t *));
	obj->f_count = len * 2;
	obj->faces = ft_memalloc(obj->f_count * sizeof(uint32_t *));
	i = 0;
	obj->mtlcolors = ft_memalloc(sizeof(uint32_t));
	obj->mtlcolors[0] = CLR_PRPL;
	while (l != NULL)
	{
		obj->verts[i] = ft_memalloc(3 * sizeof(int32_t *));
		obj->verts[i + 1] = ft_memalloc(3 * sizeof(int32_t *));
		obj->verts[i + 2] = ft_memalloc(3 * sizeof(int32_t *));
		obj->verts[i + 3] = ft_memalloc(3 * sizeof(int32_t *));
		copyverts(obj, *((t_line *)l->content), i);
		obj->verts[i + 2][Z] = -5; //HEIGHT OF THE WALL
		obj->verts[i + 3][Z] = -5;
		i3_mul(obj->verts[i], TILESIZE);
		i3_mul(obj->verts[i + 1], TILESIZE);
		i3_mul(obj->verts[i + 2], TILESIZE);
		i3_mul(obj->verts[i + 3], TILESIZE);
		linefaces(obj, i);
		l = l->next;
		i += 4;
	}
}