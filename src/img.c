/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 13:42:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 09:55:16 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static t_img	*error_image(void)
{
	static t_img		err_img;
	static uint32_t		data[32 * 32];
	int					i;

	if (err_img.size.x == 0 || err_img.size.y == 0)
	{
		err_img.size.x = 32;
		err_img.size.y = 32;
		err_img.length = 32 * 32;
		ft_strncpy_term(err_img.name, "ERRORIMAGE", 120);
		i = 0;
		while (i < 32 * 32)
		{
			if (i % 2 == 0)
			{
				data[i] = AMBER_2;
			}
			i++;
		}
		err_img.data = data;
	}
	return (&err_img);
}

t_img	*get_image_by_index(t_sdlcontext sdl, int index)
{
	int		i;

	i = 0;
	while (i < sdl.imagecount && sdl.images != NULL)
	{
		if (i == index)
			return (&sdl.images[i]);
		i++;
	}
	return (error_image());
}

int	ft_strcmpdot(const char *s1, const char *s2)
{
	while (*s1 == *s2 && *s1 && *s2 && *s1 != '.' && *s2 != '.')
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

// Finds an image by name 'name' from 'sdl.images'.
// If you're planning to draw a image a bunch of times,
// you should cache this result and use it multiple times instead of calling 
// this unction for each draw call.
t_img	*get_image_by_name(t_sdlcontext sdl, char *name)
{
	int		i;
	char	correct_name[64];
	char	fullpath[256];

	i = 0;
	ft_strncpy_term(correct_name, name, 60);
	if (ft_strstr(name, ".tga") == NULL)
		ft_strcat(correct_name, ".tga");
	while (i < sdl.imagecount && sdl.images != NULL)
	{
		if (ft_strcmpdot(sdl.images[i].name, correct_name) == 0)
			return (&sdl.images[i]);
		i++;
	}
	i = 0;
	while (i < sdl.env_texturecount && sdl.env_textures != NULL)
	{
		if (ft_strcmp(sdl.env_textures[i].name, correct_name) == 0)
			return (&sdl.env_textures[i]);
		i++;
	}
	return (error_image());
}
