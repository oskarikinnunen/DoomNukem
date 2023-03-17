/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_images.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:01:27 by raho              #+#    #+#             */
/*   Updated: 2023/03/17 20:07:37 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "tga.h"

static void	unpack_and_load_image(int img_i, char *image_name,
									t_sdlcontext *sdl)
{
	load_and_write_filecontent(LEVEL0FILE, image_name, TEMPIMG);
	sdl->images[img_i] = tgaparse(TEMPIMG);
	if (sdl->images[img_i].data != NULL)
		ft_strcpy(sdl->images[img_i].name, extract_filename(image_name));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded .tga file:", sdl->images[img_i].name, NULL});
	remove(TEMPIMG);
}

static int	parse_image_list(int fd, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*image_name;

	i = 0;
	image_name = NULL;
	ret = get_next_line(fd, &image_name);
	while (ret)
	{
		if (image_name)
		{
			unpack_and_load_image(i, image_name, sdl);
			free(image_name);
			image_name = NULL;
			i++;
		}
		ret = get_next_line(fd, &image_name);
	}
	return (ret);
}

void	playmode_load_images(t_sdlcontext *sdl)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING IMAGES");
	load_and_write_filecontent(LEVEL0FILE, IMGLISTPATH, TEMPIMGLIST);
	sdl->imagecount = count_asset_list(TEMPIMGLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPIMGLIST, "size =", s_itoa(sdl->imagecount), NULL});
	sdl->images = ft_memalloc(sizeof(t_img) * sdl->imagecount);
	if (sdl->images == NULL)
		doomlog(LOG_EC_MALLOC, "playmode_load_images");
	fd = fileopen(TEMPIMGLIST, O_RDONLY);
	ret = parse_image_list(fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_images");
	fileclose(fd, TEMPIMGLIST);
	remove(TEMPIMGLIST);
}
