/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:12:58 by raho              #+#    #+#             */
/*   Updated: 2023/03/03 20:09:08 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tga.h"
#include "colors.h"

static void	convert_grayscale_to_uint32(t_tga *tga)
{
	int		i;
	uint8_t	gray;

	i = 0;
	while (i < tga->image_data.size)
	{
		gray = tga->image_data.pixels[i];
		tga->image_data.pixels[i] = gray | gray << 8 | gray << 16;
		i++;
	}
}

static int	load_tga(int fd, char *filename, t_tga *tga)
{
	save_header(fd, filename, tga);
	if (check_data_type(tga->header.data_type_code, filename) == -1)
		return (-1);
	tga->image_data.size = tga->header.image_height * tga->header.image_width;
	if (tga->header.pixel_depth == 32)
		tga->transparency = true;
	if (tga->header.data_type_code == 3 && tga->header.pixel_depth != 8)
	{
		doomlog_mul(LOG_WARNING, (char *[4]){\
			"image:", filename, \
			"type not supported (grayscale & pixel_deth > 8)", NULL});
		return (-1);
	}
	save_pixel_order(tga);
	if (tga->header.id_length != 0)
		save_image_id(fd, filename, tga);
	save_image_data(fd, filename, tga);
	if (tga->header.data_type_code == 3)
		convert_grayscale_to_uint32(tga);
	return (1);
}

static t_img	tga_to_simpleimg(char *filename, t_tga *tga)
{
	int		i;
	t_img	img;

	img.size.x = tga->header.image_width;
	img.size.y = tga->header.image_height;
	img.length = tga->image_data.size;
	img.transparency = tga->transparency;
	img.data = ft_memalloc(img.length * sizeof(uint32_t));
	if (img.data == NULL)
		doomlog(LOG_EC_MALLOC, "tga_to_simpleimg");
	i = 0;
	while (i < img.length)
	{
		if (!img.transparency)
			img.data[i] = flip_alpha(tga->image_data.pixels[i]);
		else
			img.data[i] = tga->image_data.pixels[i];
		i++;
	}
	if (tga->header.id_length != 0)
		free(tga->image_data.image_id);
	free(tga->image_data.pixels);
	return (img);
}

t_img	tgaparse(char *filename)
{
	int		fd;
	int		ret;
	t_tga	tga;

	ft_bzero(&tga, sizeof(t_tga));
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		doomlog(LOG_EC_OPEN, filename);
	ret = load_tga(fd, filename, &tga);
	if (close(fd) == -1)
		doomlog(LOG_EC_CLOSE, filename);
	if (ret == 1)
		return (tga_to_simpleimg(filename, &tga));
	return ((t_img){0});
}
