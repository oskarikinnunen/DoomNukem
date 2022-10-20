/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 13:42:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 13:44:44 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

// Finds an image by name 'name' from 'sdl.images'.
//
// If you're planning to draw a image a bunch of times,
// you should cache this result and use it multiple times instead of calling 
// this function for each draw call.
t_img	get_image_by_name(t_sdlcontext sdl, char *name)
{
	int	i;

	i = 0;
	while (i < sdl.imagecount)
	{
		if (ft_strcmp(sdl.images[i].name, name) == 0)
			return (sdl.images[i]);
		i++;
	}
	return ((t_img){0, 0, "INVALID IMAGE!!!", 0}); //TODO: RETURN DEBUGIMAGE HERE
}

void	alloc_image(t_img *img, int width, int height)
{
	img->size.x = width;
	img->size.y = height;
	img->length = width * height;
	img->data = ft_memalloc(img->length * sizeof(uint32_t));
	if (img->data == NULL)
		error_log(EC_MALLOC);
}
