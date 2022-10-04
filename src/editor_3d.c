/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 11:57:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 13:12:18 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void setfaceindex(uint32_t **face, int fi)
{
	int	indx[2];
	int	i;

	i = 0;
	while (i < 3)
	{
		indx[0] = ((i >= 2) * GRIDSIZE) + !((i + 1) % 2);
		//indx[1] = ((i + 1 >= 2) * GRIDSIZE) + !((i + 2) % 2);
		face[0][i] = fi + indx[0];
		i++;
	}
}

static void generatefaces(t_obj *obj)
{
	int	len;
	int	i;

	len = GRIDSIZE * GRIDSIZE;
	obj->f_count = (len - GRIDSIZE - (len - GRIDSIZE) / GRIDSIZE) * 2;
	obj->faces = ft_memalloc(obj->f_count * sizeof(uint32_t *));
	i = 0;
	while (i < obj->f_count - GRIDSIZE - 2)
	{
		obj->faces[i] = ft_memalloc(sizeof(uint32_t [3]));
		setfaceindex(&obj->faces[i], i);
		i++;
		//i += (i + 1) % GRIDSIZE == 0;
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

void	render3d(t_fdf *fdf)
{
	
}