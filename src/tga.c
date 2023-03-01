/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:12:58 by raho              #+#    #+#             */
/*   Updated: 2023/03/01 18:55:02 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tga.h"

static void	save_header(int fd, char *filename, t_tga *tga)
{
	if (read(fd, &(tga->header.id_length), 1) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.color_map_type), 1) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.data_type_code), 1) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.color_map_first_entry), 2) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.color_map_length), 2) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.color_map_entry_size), 1) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.x_origin), 2) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.y_origin), 2) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.image_width), 2) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.image_height), 2) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.pixel_depth), 1) == -1)
		doomlog(LOG_EC_READ, filename);
	if (read(fd, &(tga->header.image_descriptor), 1) == -1)
		doomlog(LOG_EC_READ, filename);
}

static void	save_image_id(int fd, char *filename, t_tga *tga)
{
	tga->imgdata.image_id = malloc(tga->header.id_length + 1);
	if (tga->imgdata.image_id == NULL)
		doomlog(LOG_EC_MALLOC, filename);
	if (read(fd, tga->imgdata.image_id, tga->header.id_length) == -1)
		doomlog(LOG_EC_READ, filename);
}

static void	save_colormap_data(int fd, char *filename, t_tga *tga)
{
	tga->color_map.size = (tga->header.color_map_entry_size / 8) * tga->header.color_map_length;
	tga->color_map.colors = malloc(tga->color_map.size);
	read(fd, tga->color_map.colors, tga->color_map.size);
}

static void	save_truecolor_data(int fd, char *filename, t_tga *tga)
{
	int	i;

	tga->imgdata.pixels = malloc(sizeof(uint32_t) * (tga->header.image_height * tga->header.image_width));
	if (tga->imgdata.pixels == NULL)
	{
		printf("mallocing imagedata failed\n");
		exit (1);
	}
	ft_bzero(tga->imgdata.pixels, tga->header.image_height * tga->header.image_width);
	i = 0;
	while (i < tga->header.image_height * tga->header.image_width)
	{
		if (read(fd, &tga->imgdata.pixels[i], (tga->header.pixel_depth / 8)) == -1)
		{
			printf("saving imagedata failed\n");
			exit (1);
		}
		i++;
	}
}

static void	save_grayscale_data(int fd, char *filename, t_tga *tga)
{
	
}

void	load_tga(int fd, char *filename, t_tga *tga)
{
	save_header(fd, filename, tga);
	
	printf("idLength: %d \n", tga->header.id_length);
	printf("colorMapData: %d \n", tga->header.color_map_type);
	printf("dataTypeCode: %d \n", tga->header.data_type_code);
	printf("colorMapFirstEntry: %d \n", tga->header.color_map_first_entry);
	printf("colorMapLength: %d \n", tga->header.color_map_length);
	printf("colorMapEntrySize: %d \n",tga->header.color_map_entry_size);
	printf("x_origin: %d \n", tga->header.x_origin);
	printf("y_origin: %d \n", tga->header.y_origin);
	printf("imagewidth: %d \n", tga->header.image_width);
	printf("imageheight: %d \n", tga->header.image_height);
	printf("pixeldepth: %d \n", tga->header.pixel_depth);
	printf("imageDescriptor: %d \n", tga->header.image_descriptor);
	
	if (tga->header.data_type_code == 0)
		doomlog_mul(LOG_FATAL, (char *[4]){
			"image:", filename, "has no imagedata present", NULL});
	if (tga->header.id_length != 0)
		save_image_id(fd, filename, tga);
	if (tga->header.data_type_code == 1)
		save_colormap_data(fd, filename, tga);
	else if (tga->header.data_type_code == 2)
		save_truecolor_data(fd, filename, tga);
	else if (tga->header.data_type_code == 3)
		save_grayscale_data(fd, filename, tga);
	else
		doomlog_mul(LOG_FATAL, (char *[5]){
			"image:", filename, "has unsupported data type =",
			tga->header.data_type_code, NULL});
}

// test transparent picture blending in draw_image
// transparency boolean to image struct

t_img	tga_to_simpleimg(t_tga *tga)
{
	int		i;
	t_img	img;

	img.size.x = tga->header.image_width;
	img.size.y = tga->header.image_height;
	img.length = img.size.x * img.size.y;
	img.data = ft_memalloc(img.length * sizeof(uint32_t));
	if (img.data == NULL)
		doomlog(LOG_EC_MALLOC, "tga_to_simpleimg");
	i = 0;
	while (i < img.length)
	{
		img.data[i] =  tga->imgdata.pixels[i];//flip_channels();
		i++;
	}
	if (tga->header.id_length != 0)
		free(tga->imgdata.image_id);
	if (tga->header.data_type_code == 1)
		free(tga->color_map.colors);
	free(tga->imgdata.pixels);
	return (img);
}

t_img	tgaparse(char *filename)
{
	int		fd;
	t_tga	tga;
	t_img	result;

	ft_bzero(&tga, sizeof(t_tga));
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		doomlog(LOG_EC_OPEN, filename);
	load_tga(fd, filename, &tga);
	if (close(fd) == -1)
		doomlog(LOG_EC_CLOSE, filename);
	result = tga_to_simpleimg(&tga);
	return (result);
}
