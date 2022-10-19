/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 17:10:14 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/19 18:38:42 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "png.h"
#include "doomnukem.h"

//only works with image sizes < 1 byte, result isn't calculated correctly
uint32_t	png4byte(char *ptr)
{
	uint32_t	result;
	int		i;

	i = 0;
	result = 0;
	while (i < 4)
	{
		result = (unsigned char)ptr[i];
		i++;
	}
	return (result);
}

char	*readpalette(t_pngdata *png, char *ptr)
{
	int	i;

	png->palette.plte = ft_memalloc(sizeof(uint64_t) * 1024);
	if (png->palette.plte == NULL)
		error_log(EC_MALLOC);
	while (ft_strncmp(ptr, "PLTE", 4) != 0)
		ptr++;
	ptr += 4;
	i = 0;
	while (ft_strncmp(ptr, "bKGD", 4) != 0 && ft_strncmp(ptr, "tIME", 4) != 0)
	{
		png->palette.plte[i] = (*ptr << 16) + (*(ptr + 1) << 8) + (*(ptr + 2));
		ptr += 3;
		i++;
	}
	png->palette.length = i;
	return (ptr);
}

void	readdat(t_pngdata *png, char *ptr)
{
	int	count;

	while (ft_strncmp(ptr, "IDAT", 4) != 0)
		ptr++;
	ptr += 12;
	png->data = ft_memalloc(sizeof(uint8_t) * png->width * png->height);
	if (png->data == NULL)
		error_log(EC_MALLOC);
	count = 0;
	while (ft_strncmp(ptr, "tEXt", 4) != 0
		&& count < png->width * png->height)
	{
		png->data[count] = *ptr;
		ptr++;
		count++;
	}
}

void	pngtosimpleimg(t_pngdata *png, t_img *img) //dis bad, make return t_img instead
{
	int	i;

	img->size.x = png->width;
	img->size.y = png->height;
	//ft_memcpy(img->size, png->size, sizeof(Uint32[2]));
	img->length = img->size.x * img->size.y;
	img->data = ft_memalloc(img->length * sizeof(uint32_t));
	i = 0;
	while (i < img->length)
	{
		img->data[i] = png->palette.plte[png->data[i]];
		i++;
	}
	free(png->data);
	free(png->palette.plte); // check if palette exists?
}

//TODO: COPY DRAWSCALED IMAGE FROM WOLF3D SOURCES
//TODO: generate placeholder image if the image requested here didn't exist
t_img	pngparse(char *filename)
{
	t_pngdata	png;
	int			fd;
	int			len;
	char		*ptr;
	char		buf[32000];
	t_img		result;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_log(EC_OPEN);
	len = read(fd, buf, sizeof(uint8_t) * 32000);
	ft_bzero(&png, sizeof(t_pngdata));
	ptr = buf;
	while (ft_strcmp(++ptr, "IHDR") != 0 && len > 0)
		len--;
	ptr += 4;
	png.width = png4byte(ptr);
	png.height = png4byte(ptr + 4);
	ptr += 8;
	png.bitdepth = *(ptr);
	png.colortype = *(ptr + 1);
	ptr = readpalette(&png, ptr);
	readdat(&png, ptr);
	close(fd);
	//CONVERT TO SIMPLEIMG HERE AND RETURN THAT
	pngtosimpleimg(&png, &result);
	return (result);
}