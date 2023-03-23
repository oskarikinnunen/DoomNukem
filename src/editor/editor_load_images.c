/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_load_images.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:44:02 by raho              #+#    #+#             */
/*   Updated: 2023/03/23 18:01:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "tga.h"

static int	parse_image_list(int fd, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*image_path;

	i = 0;
	image_path = NULL;
	ret = get_next_line(fd, &image_path);
	while (ret)
	{
		if (image_path)
		{
			sdl->images[i] = tga_parse(image_path);
			if (sdl->images[i].data != NULL)
				ft_strcpy(sdl->images[i].name, extract_filename(image_path));
			doomlog_mul(LOG_NORMAL, (char *[3]){\
					"parsed .tga file:", sdl->images[i].name, NULL});
			free(image_path);
			image_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &image_path);
	}
	return (ret);
}

void	editor_load_images(t_sdlcontext *sdl)
{
	int	ret;
	int	fd;

	doomlog(LOG_NORMAL, "LOADING IMAGES");
	sdl->imagecount = count_asset_list(IMGLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			IMGLISTPATH, "size =", s_itoa(sdl->imagecount), NULL});
	sdl->images = ft_memalloc(sizeof(t_img) * sdl->imagecount);
	if (sdl->images == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->images");
	fd = fileopen(IMGLISTPATH, O_RDONLY);
	ret = parse_image_list(fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, IMGLISTPATH);
	fileclose(fd, IMGLISTPATH);
}

static int	parse_image_env_list(int fd, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*env_texture_path;

	i = 0;
	env_texture_path = NULL;
	ret = get_next_line(fd, &env_texture_path);
	while (ret)
	{
		if (env_texture_path)
		{
			sdl->env_textures[i] = tga_parse(env_texture_path);
			if (sdl->env_textures[i].data != NULL)
				ft_strcpy(sdl->env_textures[i].name, \
							extract_filename(env_texture_path));
			doomlog_mul(LOG_NORMAL, (char *[3]){\
					"parsed .tga file:", sdl->env_textures[i].name, NULL});
			free(env_texture_path);
			env_texture_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &env_texture_path);
	}
	return (ret);
}

void	editor_load_env_textures(t_sdlcontext *sdl)
{
	int	ret;
	int	fd;

	doomlog(LOG_NORMAL, "LOADING ENV_TEXTURES");
	sdl->env_texturecount = count_asset_list(IMGENVLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			IMGENVLISTPATH, "size =", s_itoa(sdl->env_texturecount), NULL});
	sdl->env_textures = ft_memalloc(sizeof(t_img) * sdl->env_texturecount);
	if (sdl->env_textures == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->env_textures");
	fd = fileopen(IMGENVLISTPATH, O_RDONLY);
	ret = parse_image_env_list(fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, IMGENVLISTPATH);
	fileclose(fd, IMGENVLISTPATH);
}
