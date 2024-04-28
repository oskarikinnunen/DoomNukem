/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_images.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:01:27 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "assets.h"
#include "tga.h"

static void	unpack_and_load_image(int img_i, int level_fd,
									char *image_name, t_sdlcontext *sdl)
{
	load_and_write_filecontent(level_fd, image_name, TEMPIMG);
	sdl->images[img_i] = tga_parse(TEMPIMG);
	if (sdl->images[img_i].data != NULL)
		ft_strncpy_term(sdl->images[img_i].name, \
				extract_file_name(image_name), 120);
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded .tga file:", sdl->images[img_i].name, NULL});
}

static int	parse_image_list(int level_fd, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*image_name;
	int		temp_fd;

	i = 0;
	image_name = NULL;
	temp_fd = ft_fileopen(TEMPIMGLIST, O_RDONLY);
	ret = get_next_line(temp_fd, &image_name);
	while (ret)
	{
		if (image_name)
		{
			unpack_and_load_image(i, level_fd, image_name, sdl);
			free(image_name);
			image_name = NULL;
			i++;
		}
		ret = get_next_line(temp_fd, &image_name);
	}
	ft_fileclose(temp_fd, TEMPIMGLIST);
	return (ret);
}

void	playmode_load_images(int level_fd, t_sdlcontext *sdl)
{
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING IMAGES");
	load_and_write_filecontent(level_fd, IMGLISTPATH, TEMPIMGLIST);
	sdl->imagecount = count_asset_list(TEMPIMGLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPIMGLIST, "size =", s_itoa(sdl->imagecount), NULL});
	sdl->images = prot_memalloc(sizeof(t_img) * sdl->imagecount);
	ret = parse_image_list(level_fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_images");
}
