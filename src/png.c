/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 17:10:14 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/22 10:44:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "png.h"
#include "doomnukem.h"
#include <unistd.h>

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

typedef	struct s_rgb
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} t_rgb;

typedef struct s_color
{
	union cdata_u
	{
		t_rgb		rgb;
		uint32_t	color;
	} dat;
}	t_color;

static uint32_t	flip_channels2(uint32_t clr)
{
	t_color		result;
	t_color		orig;
	orig.dat.color = clr;
	result.dat.rgb.r = orig.dat.rgb.b;
	result.dat.rgb.g = orig.dat.rgb.g;
	result.dat.rgb.b = orig.dat.rgb.r;
	result.dat.rgb.a = orig.dat.rgb.a;
	return (result.dat.color);
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

void	sample_ouroboros(t_img *orig) //samples the image into itself
{
	uint32_t	*newdata;
	t_point		sample;

	newdata = ft_memalloc((orig->length) * sizeof(uint32_t));
	sample = point_zero();
	//orig->size.y -= 1;
	while (sample.y < orig->size.y - 1)
	{
		sample.x = 0;
		while (sample.x < orig->size.x)
		{
			newdata[sample.x + (sample.y * orig->size.x)] = orig->data[sample.x + sample.y + (sample.y * orig->size.x)];
			sample.x++;
		}
		sample.y++;
	}
	free(orig->data);
	orig->size.y--;
	orig->data = newdata;
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
		img->data[i] = png->palette.plte[png->data[i]] & 0xFFFFFF;
		i++;
	}
	sample_ouroboros(img);
	free(png->data);
	free(png->palette.plte); // check if palette exists?
}

#define _WINDOWS_
//TODO: generate placeholder image if the image requested here didn't exist
t_img	pngparse(char *filename)
{
	t_pngdata	png;
	int			fd;
	ssize_t		len;
	uint8_t		*ptr;
	uint8_t		buf[1000000]; //TODO: this is bad
	t_img		result;

	fd = open(filename, O_RDONLY | O_BINARY);
	if (fd < 0)
		error_log(EC_OPEN);
	#ifdef _WINDOWS_
		len = _read(fd, buf, sizeof(uint8_t) * 1000000);
	#else
		len = read(fd, buf, sizeof(uint8_t) * 1000000);
	#endif
	ft_bzero(&png, sizeof(t_pngdata));
	ptr = buf;
	printf("buf: '%s' \n len is : %i \n", buf, len);
	while (ft_strcmp(++ptr, "IHDR") != 0 /*&& len > 0*/)
		len--;
	ptr += 4;
	//#ifndef _WINDOWS_
	//printf("len after IHDR is %i \n", len);
	
	//png.width = rev_bytes(*(uint32_t *)ptr);
	//png.height = rev_bytes(*(uint32_t *)(ptr + 4));
	png.width = rev_bytes(*(uint32_t *)ptr);
	png.height = rev_bytes(*(uint32_t *)(ptr + 4));

	//#else
	//png.width = rev_bytes(rev_bytes(*(uint32_t *)ptr));
	//png.height = rev_bytes(rev_bytes(*(uint32_t *)(ptr + 4)));
	printf("windows image header read; img size %i x %i \n", png.width, png.height);
	//#endif
	if (png.width >= 165 || png.height >= 165)
	{
		printf("images with size over ~165 pixels are unsupported, pls fix the png reader! (image was %s , assumed size %i x %i\n", filename, png.width, png.height);
		exit(0);
	}
	ptr += 8;
	png.bitdepth = *(ptr);
	png.colortype = *(ptr + 1);
	ptr = readpalette(&png, ptr);
	readdat(&png, ptr);
	close(fd);
	pngtosimpleimg(&png, &result);
	if (ft_strlen(filename) < 256)
		ft_strcpy(result.name, filename);
	printf("read png %s \n", result.name);
	return (result);
}
