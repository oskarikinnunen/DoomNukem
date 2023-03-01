/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 16:14:07 by raho              #+#    #+#             */
/*   Updated: 2023/03/01 18:43:34 by raho             ###   ########.fr       */
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

typedef struct s_tgafooter
{
	
}	t_tgafooter;

typedef struct s_tgaextension
{
	
}	t_tgaextension;

typedef struct s_tgaimg
{
	char			*image_id;
	uint32_t		*pixels;
	int				width;
	int				height;
}	t_tgaimg;

typedef struct s_tgacolormap
{
	uint32_t	*colors;
	int			size;
}	t_tgacolormap;

typedef struct s_tga
{
	int				fd;
	t_tgaheader		header;
	t_tgafooter 	footer;
	t_tgaextension	extension;
	t_tgacolormap	color_map;
	t_tgaimg		imgdata;
}	t_tga;

typedef struct s_img t_img;

void	load_tga(int fd, char *filename, t_tga *tga);
t_img	tgaparse(char *filename);		

#endif
