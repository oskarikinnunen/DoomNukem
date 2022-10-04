/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 13:42:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 14:01:14 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	alloc_image(t_img *img, int width, int height)
{
	img->size[X] = width;
	img->size[Y] = width;
	img->length = width * height * sizeof(uint32_t);
	img->data = ft_memalloc(img->length);
}
