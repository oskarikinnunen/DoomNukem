/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_grid_to_obj.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 11:57:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 16:22:48 by raho             ###   ########.fr       */
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
	if (!obj->faces)
		errors(4);
	i = 0;
	ri = 0;
	while (i < obj->f_count)
	{
		obj->faces[i] = ft_memalloc(sizeof(uint32_t [3]));
		obj->faces[i + 1] = ft_memalloc(sizeof(uint32_t [3]));
		if (!obj->faces[i] || !obj->faces[i + 1])
			errors(4);
		setfaceindex(&obj->faces[i], ri);
		i += 2;
		ri++;
		ri += (ri + 1) % GRIDSIZE == 0;
	}
}

static void	save_coordinates(t_obj *obj, int *crd)
{
	obj->verts[obj->v_count] = ft_memalloc(sizeof(int32_t [3]));
	if (!obj->verts[obj->v_count])
		errors(4);
	obj->verts[obj->v_count][X] = crd[X];
	obj->verts[obj->v_count][Y] = crd[Y];
}

void grid_to_obj(t_obj *obj)
{
	int	crd[2];

	obj->v_count = 0;
	obj->verts = ft_memalloc(GRIDSIZE * GRIDSIZE * sizeof(float *));
	if (!obj->verts)
		errors(4);
	obj->mtlcolors = ft_memalloc(sizeof(uint32_t));
	if (!obj->mtlcolors)
		errors(4);
	obj->mtlcolors[0] = CLR_GRAY;
	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < TILESIZE * GRIDSIZE)
	{
		crd[X] = 0;
		while (crd[X] < TILESIZE * GRIDSIZE)
		{
			save_coordinates(obj, crd);
			obj->v_count++;
			crd[X] += TILESIZE;
		}
		crd[Y] += TILESIZE;
	}
	generatefaces(obj);
}
