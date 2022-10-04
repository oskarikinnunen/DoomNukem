/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 11:57:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 16:34:32 by okinnune         ###   ########.fr       */
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

static void linefaces(t_obj *obj, int i)
{
	//TODO: Protect allocations plz
	int	fi;

	fi = i / 2;
	obj->faces[fi] = ft_memalloc(sizeof (uint32_t [3]));
	obj->faces[fi][0] = i;
	obj->faces[fi][1] = i + 1;
	obj->faces[fi][2] = i + 2;
	fi++;
	obj->faces[fi] = ft_memalloc(sizeof (uint32_t [3]));
	obj->faces[fi][0] = i + 1;
	obj->faces[fi][1] = i + 2;
	obj->faces[fi][2] = i;
	printf("fi %i \n", fi);
}

static void	i3_mul(int *i3, int mul)
{
	i3[X] = i3[X] * mul;
	i3[Y] = i3[Y] * mul;
	i3[Z] = i3[Z] * mul;
}

void lines_to_obj(t_obj *obj, t_editor *ed)
{
	t_list	*l;
	int		len;
	int		i;

	l = ed->linelist;
	len = ft_listlen(l) - 1;
	printf("listlen %i \n", len);
	obj->v_count = len * 4;
	obj->verts = ft_memalloc(obj->v_count * sizeof(int32_t));
	obj->f_count = len * 2;
	obj->faces = ft_memalloc(obj->f_count * sizeof(int32_t *));
	i = 0;
	while (l != NULL)
	{
		obj->verts[i] = ft_memalloc(sizeof(int32_t [3]));
		obj->verts[i + 1] = ft_memalloc(sizeof(int32_t [3]));
		obj->verts[i + 2] = ft_memalloc(sizeof(int32_t [3]));
		obj->verts[i + 3] = ft_memalloc(sizeof(int32_t [3]));
		ft_memcpy(obj->verts[i], (*(t_line *)l->content).start, sizeof(int32_t [2]));
		ft_memcpy(obj->verts[i + 1], (*(t_line *)l->content).end, sizeof(int32_t [2]));
		ft_memcpy(obj->verts[i + 2], (*(t_line *)l->content).start, sizeof(int32_t [2]));
		ft_memcpy(obj->verts[i + 3], (*(t_line *)l->content).end, sizeof(int32_t [2]));
		obj->verts[i + 2][Z] = 20; //HEIGHT OF THE WALL
		obj->verts[i + 3][Z] = 20;
		i3_mul(obj->verts[i], TILESIZE);
		i3_mul(obj->verts[i + 1], TILESIZE);
		i3_mul(obj->verts[i + 2], TILESIZE);
		i3_mul(obj->verts[i + 3], TILESIZE);
		linefaces(obj, i);
		l = l->next;
		printf("lcount %i \n", i);
		i += 4;
	}
	printf("fcount %i \n", obj->f_count);
}