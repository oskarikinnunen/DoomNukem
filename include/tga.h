/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:14:07 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:22:56 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TGA_H
# define TGA_H

# include <inttypes.h>

typedef struct s_tgaheader
{
	uint8_t		id_length;
	uint8_t		color_map_type;
	uint8_t		data_type_code;
	uint16_t	color_map_first_entry;
	uint16_t	color_map_length;
	uint8_t		color_map_entry_size;
	uint16_t	x_origin;
	uint16_t	y_origin;
	uint16_t	image_width;
	uint16_t	image_height;
	uint8_t		pixel_depth;
	uint8_t		image_descriptor;
}	t_tgaheader;

typedef struct s_tgaimg
{
	char			*image_id;
	uint32_t		*pixels;
	int				size;
}	t_tgaimg;

typedef struct s_pixel_order
{
	bool	bottom;
	bool	top;
	bool	left;
	bool	right;
}	t_pixel_order;

typedef struct s_tga
{
	int				fd;
	t_tgaheader		header;
	t_pixel_order	pixel_order;
	t_tgaimg		image_data;
	bool			transparency;
}	t_tga;

typedef struct s_img	t_img;

t_img	tga_parse(char *filename);
void	save_header(int fd, char *file_name, t_tga *tga);
int		check_data_type(uint8_t data_type_code, char *file_name);
void	save_pixel_order(t_tga *tga);
void	save_image_id(int fd, char *file_name, t_tga *tga);
void	save_image_data(int fd, char *file_name, t_tga *tga);

#endif
