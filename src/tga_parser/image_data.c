/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:12:58 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 13:57:07 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tga.h"

static void	parse_top_left(int fd, char *filename, t_tga *tga)
{
	int	i;

	i = 0;
	while (i < tga->image_data.size)
	{
		if (read(fd, &tga->image_data.pixels[i], \
				(tga->header.pixel_depth / 8)) == -1)
			doomlog(LOG_EC_READ, filename);
		i++;
	}
}

static void	parse_top_right(int fd, char *filename, t_tga *tga)
{
	int	i;
	int	start_of_the_row;

	i = tga->header.image_width - 1;
	start_of_the_row = 0;
	while (i < tga->image_data.size)
	{
		while (i >= start_of_the_row)
		{
			if (read(fd, &tga->image_data.pixels[i], \
					(tga->header.pixel_depth / 8)) == -1)
				doomlog(LOG_EC_READ, filename);
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
			if (read(fd, &tga->image_data.pixels[i], \
					(tga->header.pixel_depth / 8)) == -1)
				doomlog(LOG_EC_READ, filename);
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
			if (read(fd, &tga->image_data.pixels[i], \
					(tga->header.pixel_depth / 8)) == -1)
				doomlog(LOG_EC_READ, filename);
			i--;
		}
		start_of_the_row -= tga->header.image_width;
	}
}

void	save_image_data(int fd, char *filename, t_tga *tga)
{
	tga->image_data.pixels = \
			prot_memalloc(sizeof(uint32_t) * tga->image_data.size);
	if (tga->pixel_order.top && tga->pixel_order.left)
		parse_top_left(fd, filename, tga);
	else if (tga->pixel_order.top && tga->pixel_order.right)
		parse_top_right(fd, filename, tga);
	else if (tga->pixel_order.bottom && tga->pixel_order.left)
		parse_bottom_left(fd, filename, tga);
	else if (tga->pixel_order.bottom && tga->pixel_order.right)
		parse_bottom_right(fd, filename, tga);
}
