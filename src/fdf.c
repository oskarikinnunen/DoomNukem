/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 11:58:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 17:35:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "v3lib.h"

static void	calc_matrices(t_fdf *fdf)
{
	float	angles[2];

	ft_bzero(fdf->matrices, sizeof(float [2][3][3]));
	angles[X] = fdf->view[X];
	angles[Y] = fdf->view[Y];
	fdf->matrices[0][X][X] = cos(angles[X]);
	fdf->matrices[0][X][Y] = -sin(angles[X]);
	fdf->matrices[0][Y][X] = sin(angles[X]);
	fdf->matrices[0][Y][Y] = cos(angles[X]);


	/*iso[Y][Y] = cos(angle);
	iso[Y][Z] = sin(angle);
	iso[Z][Y] = -sin(angle);
	iso[Z][Z] = cos(angle);*/
	fdf->matrices[1][X][X] = 1.0f;
	fdf->matrices[1][Y][Y] = cos(angles[Y]);
	fdf->matrices[1][Y][Z] = sin(angles[Y]);
	fdf->matrices[1][Z][Y] = -sin(angles[Y]);
	fdf->matrices[1][Z][Z] = cos(angles[Y]);
}

int	fdf_init(t_fdf *fdf)
{
	int	i;

	fdf->depth = ft_memalloc(sizeof(float) * fdf->img.length);
	fdf->verts = ft_memalloc(sizeof(float *) * fdf->obj.v_count);
	if (fdf->depth == NULL || fdf->verts == NULL)
		return (-1);
	i = 0;
	while (i < fdf->obj.v_count)
	{
		fdf->verts[i] = ft_memalloc(sizeof(float *) * 3);
		if (fdf->verts[i] == NULL) //TODO: Free the prev vertices in this case of fail
			return (-1);
		i++;
	}
	calc_matrices(fdf);
	return (1);
}

static	void drawface(t_fdf *fdf, int i)//wireframe
{
	int	i3[3][3];

	//fdf->obj.faces[i][0] = 2;
	//fdf->obj.faces[i][1] = 2;
	//fdf->obj.faces[i][2] = 2;
	//return ;
	//fdf->verts[0][0] = 42;
	//printf("f index %i \n", fdf->obj.faces[i][0]);
	
	fv3_to_iv3(fdf->verts[fdf->obj.faces[i][0]], i3[0]);
	fv3_to_iv3(fdf->verts[fdf->obj.faces[i][1]], i3[1]);
	fv3_to_iv3(fdf->verts[fdf->obj.faces[i][2]], i3[2]);
	
	drawline(fdf->img.data, i3[0], i3[1], CLR_GRAY);
	drawline(fdf->img.data, i3[1], i3[2], CLR_GRAY);
	drawline(fdf->img.data, i3[2], i3[0], CLR_GRAY);
	return ;
}

void	fdf_draw_wireframe(t_fdf *fdf, int offset[2])
{
	int	i;
	int min_x;

	i = 0;
	min_x = INT_MAX;
	ft_bzero(fdf->img.data, fdf->img.length);
	calc_matrices(fdf);
	//while (i < 66)
	while (i < fdf->obj.v_count) //fix list len
	{
		fdf->verts[i][X] = (float)fdf->obj.verts[i][X];
		fdf->verts[i][Y] = (float)fdf->obj.verts[i][Y];
		fdf->verts[i][Z] = (float)fdf->obj.verts[i][Z];
		v3_mul(fdf->matrices[X], fdf->verts[i]);
		v3_mul(fdf->matrices[Y], fdf->verts[i]);
		v3_add(fdf->verts[i], (float [3]) {offset[X], offset[Y], 0});
		i++;
		//printf("%i \n", i);
	}
	i = 0;
	//while (i < 20)
	while (i < fdf->obj.f_count)
	{
		drawface(fdf, i);
		i++;
	}
}
