/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 11:58:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 12:22:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int	fdf_init(t_fdf *fdf, t_img *img)
{
	int	i;

	ft_bzero(fdf, sizeof(t_fdf));
	fdf->depth = ft_memalloc(sizeof(float) * img->length);
	fdf->verts = ft_memalloc(sizeof(float *) * object->v_count);
	fdf->img = img;
	if (fdf->depth == NULL || fdf->verts == NULL)
		return (-1);
	i = 0;
	while (i < fdf->obj->v_count)
	{
		fdf->verts[i] = ft_memalloc(sizeof(float *) * 3);
		if (fdf->verts[i] == NULL) //TODO: Free the prev vertices in this case of fail
			return (-1);
		i++;
	}
	calc_matrices(fdf);
	return (1);
}