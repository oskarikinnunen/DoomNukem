/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 22:01:46 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/18 22:06:43 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "png.h"

//only works with image sizes < 1 byte, result isn't calculated correctly
Uint32	png4byte(char *ptr)
{
	Uint32	result;
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

	png->palette.plte = ft_memalloc(sizeof(Uint64) * 1024);
	if (png->palette.plte == NULL)
		error_exit("alloc fail");
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
	png->data = ft_memalloc(sizeof(Uint8) * png->size[X] * png->size[Y]);
	if (png->data == NULL)
		error_exit("alloc fail");
	count = 0;
	while (ft_strncmp(ptr, "tEXt", 4) != 0
		&& count < png->size[X] * png->size[Y])
	{
		png->data[count] = *ptr;
		ptr++;
		count++;
	}
}

void	pngparse(t_pngdata	*png, char *filename) //TODO: modify so this returns simpleimage, no knowledge/reference to t_pngdata should be needed when calling this
{
	int			fd;
	int			len;
	char		*ptr;
	char		buf[32000];

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit(0);
	len = read(fd, buf, sizeof(Uint8) * 32000);
	ft_bzero(png, sizeof(t_pngdata));
	ptr = buf;
	while (ft_strcmp(++ptr, "IHDR") != 0 && len > 0)
		len--;
	ptr += 4;
	png->size[X] = png4byte(ptr);
	png->size[Y] = png4byte(ptr + 4);
	ptr += 8;
	png->bitdepth = *(ptr);
	png->colortype = *(ptr + 1);
	ptr = readpalette(png, ptr);
	readdat(png, ptr);
	close(fd);
}