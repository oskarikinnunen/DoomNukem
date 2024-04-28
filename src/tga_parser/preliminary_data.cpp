/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preliminary_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:12:58 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 11:59:29 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tga.h"

void	save_header(int fd, char *filename, t_tga *tga)
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

int	check_data_type(uint8_t data_type_code, char *filename)
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

void	save_pixel_order(t_tga *tga)
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

void	save_image_id(int fd, char *filename, t_tga *tga)
{
	tga->image_data.image_id = prot_memalloc(tga->header.id_length + 1);
	tga->image_data.image_id[tga->header.id_length] = '\0';
	if (read(fd, tga->image_data.image_id, tga->header.id_length) == -1)
		doomlog(LOG_EC_READ, filename);
}
