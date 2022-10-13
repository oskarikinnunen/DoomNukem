/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 13:42:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 20:33:31 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	alloc_image(t_img *img, int width, int height)
{
	img->size[X] = width;
	img->size[Y] = width;
	img->length = width * height * sizeof(uint32_t);
	img->data = ft_memalloc(img->length);
	if (!img->data)
		error_log(EC_MALLOC);
}
