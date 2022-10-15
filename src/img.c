/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 13:42:23 by okinnune          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/10/13 20:33:31 by raho             ###   ########.fr       */
=======
/*   Updated: 2022/10/13 13:31:54 by okinnune         ###   ########.fr       */
>>>>>>> vector2-rewrite
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	alloc_image(t_img *img, int width, int height)
{
	img->size.x = width;
	img->size.y = height;
	img->length = width * height;
	img->data = ft_memalloc(img->length * sizeof(uint32_t));
	if (img->data == NULL)
		error_log(EC_MALLOC);
}
