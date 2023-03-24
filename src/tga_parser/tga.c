/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:12:58 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 14:47:34 by raho             ###   ########.fr       */
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

static int	load_tga(int fd, char *file_name, t_tga *tga)
{
	save_header(fd, file_name, tga);
	if (check_data_type(tga->header.data_type_code, file_name) == -1)
		return (-1);
	tga->image_data.size = tga->header.image_height * tga->header.image_width;
	if (tga->header.pixel_depth == 32)
		tga->transparency = true;
	if (tga->header.data_type_code == 3 && tga->header.pixel_depth != 8)
	{
		doomlog_mul(LOG_WARNING, (char *[4]){\
			"image:", file_name, \
			"type not supported (grayscale & pixel_deth > 8)", NULL});
		return (-1);
	}
	save_pixel_order(tga);
	if (tga->header.id_length != 0)
		save_image_id(fd, file_name, tga);
	save_image_data(fd, file_name, tga);
	if (tga->header.data_type_code == 3)
		convert_grayscale_to_uint32(tga);
	return (1);
}

void	check_transparency(t_img *img)
{
	t_point	p;

	p.y = 0;
	img->transparency = false;
	while (p.y < img->size.y)
	{
		p.x = 0;
		while (p.x < img->size.x)
		{
			if (((img->data[p.x + (p.y * img->size.x)] >> 24) & 0xFF) != 255)
			{
				if (!img->transparency)
					img->transparency = true;
			}
			p.x++;
		}
		p.y++;
	}
}

static t_img	tga_to_simpleimg(char *file_name, t_tga *tga)
{
	int		i;
	t_img	img;

	img.size.x = tga->header.image_width;
	img.size.y = tga->header.image_height;
	img.length = tga->image_data.size;
	img.transparency = tga->transparency;
	img.data = prot_memalloc(img.length * sizeof(uint32_t));
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
	check_transparency(&img);
	return (img);
}

t_img	tga_parse(char *file_name)
{
	int		fd;
	int		ret;
	t_tga	tga;

	ft_bzero(&tga, sizeof(t_tga));
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		doomlog(LOG_EC_OPEN, file_name);
	ret = load_tga(fd, file_name, &tga);
	if (close(fd) == -1)
		doomlog(LOG_EC_CLOSE, file_name);
	if (ret == 1)
		return (tga_to_simpleimg(file_name, &tga));
	return ((t_img){0});
}
