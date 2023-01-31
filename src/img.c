/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 13:42:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/31 15:49:29 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"


static t_img	*error_image()
{
	static t_img		err_img;
	static	uint32_t	data[32 * 32];
	int					i;

	if (err_img.size.x == 0 || err_img.size.y == 0)
	{
		err_img.size.x = 32;
		err_img.size.y = 32;
		err_img.length = 32 * 32;
		ft_strcpy(err_img.name, "ERRORIMAGE");
		i = 0;
		while (i < 32 * 32)
		{
			if (i % 2 == 0)
			{
				data[i] = flip_channels(AMBER_2);
			}
			i++;
		}
		err_img.data = data;
		printf("made error img\n");
	}
	return (&err_img);
}


t_img	*get_image_by_index(t_sdlcontext sdl, int index)
{
	int		i;

	i = 0;
	while (i < sdl.texturecount && sdl.textures != NULL)
	{
		if (i == index)
			return (&sdl.textures[i]);
		i++;
	}
	return (error_image()); //TODO: RETURN DEBUGIMAGE HERE
}

// Finds an image by name 'name' from 'sdl.images'.
//
// If you're planning to draw a image a bunch of times,
// you should cache this result and use it multiple times instead of calling 
// this unction for each draw call.
t_img	*get_image_by_name(t_sdlcontext sdl, char *name) //TODO: return pointer, this is wasteful use of memory especially when used for all of the objects
{
	int		i;
	char	correct_name[64];
	char	fullpath[256];

	i = 0;
	ft_strcpy(correct_name, name);
	if (ft_strstr(name, ".cng") == NULL)
		ft_strcat(correct_name, ".cng");
	/*if (ft_strncmp(name, IMGPATH, sizeof(IMGPATH) - 1) != 0)
		sprintf(fullpath, "%s%s", IMGPATH, name);
	else
		ft_strcpy(fullpath, name);*/
	while (i < sdl.texturecount && sdl.textures != NULL)
	{
		//printf("image name %s \n", sdl.images[i].name);
		if (ft_strcmp(sdl.textures[i].name, correct_name) == 0)
			return (&sdl.textures[i]);
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
		doomlog(LOGEC_MALLOC, NULL);
}
