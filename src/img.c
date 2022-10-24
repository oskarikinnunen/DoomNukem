/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 13:42:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/24 20:49:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"


static t_img	error_image()
{
	static t_img		err_img;
	static	uint32_t	data[256];
	int					i;

	if (err_img.size.x == 0 || err_img.size.y == 0)
	{
		err_img.size.x = 16;
		err_img.size.y = 16;
		err_img.length = 256;
		ft_strcpy(err_img.name, "ERRORIMAGE");
		i = 0;
		while (i < 256)
		{
			if (i % 2 == 0)
			{
				data[i] = CLR_PRPL;
			}
			i++;
		}
		err_img.data = data;
		printf("made error img\n");
	}
	return (err_img);
}

// Finds an image by name 'name' from 'sdl.images'.
//
// If you're planning to draw a image a bunch of times,
// you should cache this result and use it multiple times instead of calling 
// this unction for each draw call.
t_img	get_image_by_name(t_sdlcontext sdl, char *name)
{
	int	i;

	i = 0;
	while (i < sdl.imagecount && sdl.images != NULL)
	{
		if (ft_strcmp(sdl.images[i].name, name) == 0)
			return (sdl.images[i]);
		i++;
	}
	return (error_image()); //TODO: RETURN DEBUGIMAGE HERE
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
