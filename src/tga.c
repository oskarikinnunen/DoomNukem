/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:12:58 by raho              #+#    #+#             */
/*   Updated: 2023/03/03 19:57:10 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tga.h"
#include "colors.h"

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

static int	check_data_type(uint8_t data_type_code, char *filename)
{
	if (data_type_code == 0)
	{
		doomlog_mul(LOG_WARNING, (char *[4]){
			"image:", filename, "has no imagedata present", NULL});
		return (-1);
	}
	if (data_type_code == 1)
	{
		doomlog_mul(LOG_WARNING, (char *[4]){
			"image:", filename, "data type not supported (colormap)", NULL});
		return (-1);
	}
	if (data_type_code > 3)
	{
		doomlog_mul(LOG_WARNING, (char *[4]){
			"image:", filename, "data type not supported (compressed)", NULL});
		return (-1);
	}
	return (0);
}

static void	save_pixel_order(t_tga *tga)
{
	if (((tga->header.image_descriptor >> 4) & 1) == 0)
	{
		tga->pixel_order.left = true;
		tga->pixel_order.right = false;
	}
	else
	{
		tga->pixel_order.left = false;
		tga->pixel_order.right = true;
	}
	if (((tga->header.image_descriptor >> 5) & 1) == 0)
	{
		tga->pixel_order.bottom = true;
		tga->pixel_order.top = false;
	}
	else
	{
		tga->pixel_order.bottom = false;
		tga->pixel_order.top = true;
	}
}

static void	save_image_id(int fd, char *filename, t_tga *tga)
{
	tga->image_data.image_id = ft_memalloc(tga->header.id_length + 1);
	if (tga->image_data.image_id == NULL)
		doomlog(LOG_EC_MALLOC, filename);
	tga->image_data.image_id[tga->header.id_length] = '\0';
	if (read(fd, tga->image_data.image_id, tga->header.id_length) == -1)
		doomlog(LOG_EC_READ, filename);
}

static void	parse_top_left(int fd, char *filename, t_tga *tga)
{
	int	i;
	
	i = 0;
	while (i < tga->image_data.size)
	{
		if (read(fd, &tga->image_data.pixels[i], 
				(tga->header.pixel_depth / 8)) == -1)
			doomlog(LOG_EC_READ, filename);
		/* if (tga->transparency)
			tga->image_data.pixels[i] = flip_alpha(tga->image_data.pixels[i]); */
		i++;
	}
}

static void	parse_top_right(int fd, char *filename, t_tga *tga)
{
	int	i;
	int start_of_the_row;

	i = tga->header.image_width - 1;
	start_of_the_row = 0;
	while (i < tga->image_data.size)
	{
		while (i >= start_of_the_row)
		{
			if (read(fd, &tga->image_data.pixels[i], 
					(tga->header.pixel_depth / 8)) == -1)
				doomlog(LOG_EC_READ, filename);
			/* if (tga->transparency)
				tga->image_data.pixels[i] = flip_alpha(tga->image_data.pixels[i]); */
			i--;
		}
		i += tga->header.image_width * 2;
		start_of_the_row += tga->header.image_width;
	}
}

static void	parse_bottom_left(int fd, char *filename, t_tga *tga)
{
	int	i;
	int	end_of_the_row;

	i = (tga->header.image_height - 1) * tga->header.image_width;
	end_of_the_row = tga->image_data.size;
	while (i >= 0)
	{
		while (i < end_of_the_row)
		{
			if (read(fd, &tga->image_data.pixels[i], 
					(tga->header.pixel_depth / 8)) == -1)
				doomlog(LOG_EC_READ, filename);
			/* if (tga->transparency)
				tga->image_data.pixels[i] = flip_alpha(tga->image_data.pixels[i]); */
			i++;
		}
		i -= tga->header.image_width * 2;
		end_of_the_row -= tga->header.image_width;
	}
}

static void	parse_bottom_right(int fd, char *filename, t_tga *tga)
{
	int	i;
	int	start_of_the_row;

	i = tga->image_data.size - 1;
	start_of_the_row = (tga->header.image_height - 1) * tga->header.image_width;
	while (i >= 0)
	{
		while (i >= start_of_the_row)
		{
			if (read(fd, &tga->image_data.pixels[i], 
					(tga->header.pixel_depth / 8)) == -1)
				doomlog(LOG_EC_READ, filename);
			/* if (tga->transparency)
				tga->image_data.pixels[i] = flip_alpha(tga->image_data.pixels[i]); */
			i--;
		}
		start_of_the_row -= tga->header.image_width;
	}
}

static void	save_image_data(int fd, char *filename, t_tga *tga)
{
	tga->image_data.pixels = ft_memalloc(sizeof(uint32_t) * tga->image_data.size);
	if (tga->image_data.pixels == NULL)
		doomlog(LOG_EC_MALLOC, filename);
	if (tga->pixel_order.top && tga->pixel_order.left)
		parse_top_left(fd, filename, tga);
	else if (tga->pixel_order.top && tga->pixel_order.right)
		parse_top_right(fd, filename, tga);
	else if (tga->pixel_order.bottom && tga->pixel_order.left)
		parse_bottom_left(fd, filename, tga);
	else if (tga->pixel_order.bottom && tga->pixel_order.right)
		parse_bottom_right(fd, filename, tga);
}

static void	convert_grayscale_to_rgb(t_tga *tga)
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

int	load_tga(int fd, char *filename, t_tga *tga)
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
	if (check_data_type(tga->header.data_type_code, filename) == -1)
		return (-1);
	tga->image_data.size = tga->header.image_height * tga->header.image_width;
	if (tga->header.pixel_depth == 32)
		tga->transparency = true;
	if (tga->header.data_type_code == 3 && tga->header.pixel_depth != 8)
	{
		doomlog_mul(LOG_WARNING, (char *[4]){
			"image:", filename, 
			"type not supported (grayscale & pixel_deth > 8)", NULL});
		return (-1);
	}
	save_pixel_order(tga);
	if (tga->header.id_length != 0)
		save_image_id(fd, filename, tga);
	save_image_data(fd, filename, tga);
	if (tga->header.data_type_code == 3)
		convert_grayscale_to_rgb(tga);
	return (1);
}

t_img	tga_to_simpleimg(char *filename, t_tga *tga)
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
