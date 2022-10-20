/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 17:10:14 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 19:56:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "png.h"
#include "doomnukem.h"


static uint32_t rev_bytes (uint32_t bytes)
{
	uint32_t result;
	uint8_t	byte;
	int	i;

	i = 0;
	result = 0;
	while (i < 32)
	{
		byte = (bytes >> i) & 0xFF;
		result |= byte << (32 - 8 - i);
		i += 8;
	}
	return (result);
}


//only works with image sizes < 1 byte, result isn't calculated correctly

char	*readpalette(t_pngdata *png, uint8_t *ptr)
{
	int	i;

	png->palette.plte = ft_memalloc(sizeof(uint32_t) * 2048);
	if (png->palette.plte == NULL)
		error_log(EC_MALLOC);
	while (ft_strncmp(ptr, "PLTE", 4) != 0)
		ptr++;
	ptr += 4;
	i = 0;
	while (ft_strncmp(ptr, "IDAT", 4) != 0)
	{
		if (i % 3 == 0)
		{
			png->palette.plte[i / 3] = *(uint32_t *)ptr;
		}
		ptr++;
		i++;
	}
	png->palette.length = i;
	return (ptr);
}

void	readdat(t_pngdata *png, uint8_t *ptr)
{
	int	count;

	while (ft_strncmp(ptr, "IDAT", 4) != 0)
		ptr++;
	ptr += 12;
	png->data = malloc(sizeof(uint8_t) * png->width * png->height);
	printf("malloced %i indexes for data \n", png->width * png->height);
	//png->data[(png->width * png->height) - 1] = 0;
	if (png->data == NULL)
		error_log(EC_MALLOC);
	count = 0;
	while (count < (png->width * png->height) - 1
			/*&& ft_strncmp(ptr, "tEXT", 4) != 0*/)
	{
		png->data[count] =
			*ptr;
		ptr++;
		count++;
	}
}

void	pngtosimpleimg(t_pngdata *png, t_img *img) //dis bad, make return t_img instead
{
	int	i;

	img->size.x = png->width;
	img->size.y = png->height;
	img->length = img->size.x * img->size.y;
	img->data = ft_memalloc(img->length * sizeof(uint32_t));
	i = 0;
	while (i < img->length - 1)
	{
		img->data[i] = png->palette.plte[png->data[i]];
		i++;
	}
	free(png->data);
	free(png->palette.plte); // check if palette exists?
}

//TODO: generate placeholder image if the image requested here didn't exist
t_img	pngparse(char *filename)
{
	t_pngdata	png;
	int			fd;
	int			len;
	uint8_t		*ptr;
	uint8_t		buf[1000000]; //TODO: this is bad
	t_img		result;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_log(EC_OPEN);
	len = read(fd, buf, sizeof(uint8_t) * 1000000);
	ft_bzero(&png, sizeof(t_pngdata));
	ptr = buf;
	while (ft_strcmp(++ptr, "IHDR") != 0 && len > 0)
		len--;
	ptr += 4;
	png.width = rev_bytes(*(uint32_t *)ptr);
	png.height = rev_bytes(*(uint32_t *)(ptr + 4));
	if (png.width >= 165 || png.height >= 165)
	{
		printf("images with size over ~165 pixels are unsupported, pls fix the png reader!\n");
		exit(0);
	}
	ptr += 8;
	png.bitdepth = *(ptr);
	png.colortype = *(ptr + 1);
	ptr = readpalette(&png, ptr);
	readdat(&png, ptr);
	close(fd);
	pngtosimpleimg(&png, &result);
	if (ft_strlen(filename) < 128)
		ft_strcpy(result.name, filename);
	printf("read png %s \n", filename);
	return (result);
}
