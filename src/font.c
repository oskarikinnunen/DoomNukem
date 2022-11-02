/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/10/31 22:00:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	save_font_info(t_font *font, int fd)
{
	int		ret;
	int		rows;
	int		i;
	int		k;
	char	*line;

	rows = 0;
	while (rows < 4)
	{
		k = 0;
		ret = get_next_line(fd, &line); // invalid read of size 1 & 4 by valgrind using oskari's gnl
		if (line == NULL || ret < 0)
			error_log(EC_GETNEXTLINE);
		if (rows == 0)
		{
			i = 11;
			while (k < 11)
			{
				if (k == 0)
				{
					font->name = ft_strcdup(&line[i], '"');
					if (font->name == NULL)
						error_log(EC_MALLOC);
				}
				if (k == 1)
					font->size = ft_atoi(&line[i]);
				if (k == 9)
					font->padding = ft_atoi(&line[i]);
				if (k == 10)
					font->spacing = ft_atoi(&line[i]);
				while (line[i] != '\0' && line[i] != '=')
					i++;
				i++;
				k++;
			}		
		}
		if (rows == 1)
		{
			i = 18;
			while (k < 4)
			{
				if (k == 0)
					font->line_height = ft_atoi(&line[i]);
				if (k == 2)
					font->scale.x = ft_atoi(&line[i]);
				if (k == 3)
					font->scale.y = ft_atoi(&line[i]);
				while (line[i] != '=')
					i++;
				i++;
				k++;
			}
		}
		if (rows == 3)
		{
			i = 12;
			font->char_count = ft_atoi(&line[i]);
		}
		if (line)
			free(line);
		rows++;
	}
}

static void	save_char_info(t_font_chars *c, char *str)
{
	int		i;
	int		k;

	i = 8;
	k = 0;
	while (k < 8)
	{
		if (k == 0)
			c->id = ft_atoi(&str[i]);
		if (k == 1)
			c->pos.x = ft_atoi(&str[i]);
		if (k == 2)
			c->pos.y = ft_atoi(&str[i]);
		if (k == 3)
			c->size.x = ft_atoi(&str[i]);
		if (k == 4)
			c->size.y = ft_atoi(&str[i]);
		if (k == 5)
			c->offset.x = ft_atoi(&str[i]);
		if (k == 6)
			c->offset.y = ft_atoi(&str[i]);
		if (k == 7)
			c->xadvance = ft_atoi(&str[i]);
		while (str[i] != '=')
			i++;
		i++;
		k++;
	}
}

void	load_font(t_sdlcontext *sdl, const char *filename)
{
	int		fd;
	char	*line;
	int		ret;
	int		i;
	t_img	*bitmap;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_log(EC_OPEN);
	sdl->font = ft_memalloc(sizeof(t_font));
	if (sdl->font == NULL)
		error_log(EC_MALLOC);
	
	save_font_info(sdl->font, fd);
	sdl->font->chars = ft_memalloc(sizeof(t_font_chars) * sdl->font->char_count);
	if (sdl->font->chars == NULL)
		error_log(EC_MALLOC);
	line = NULL;
	i = 0;
	ret = 1;
	while (i < sdl->font->char_count)
	{
		ret = get_next_line(fd, &line); // invalid read of size 1 & 4 valgrind using oskari's gnl
		if (line == NULL || ret < 0)
			error_log(EC_GETNEXTLINE);
		save_char_info(&sdl->font->chars[i], line);
		if (line)
			free(line);
		i++;
	}
	sdl->font->file_name = ft_strjoin(sdl->font->name, ".png");
    if (!sdl->font->file_name)
        error_log(EC_MALLOC);
	bitmap = get_image_by_name(*sdl, sdl->font->file_name);
	sdl->font->bitmap = bitmap;
	if (close(fd) == -1)
		error_log(EC_CLOSE);

    // These printfs are for troubleshooting in case something is off

	/*
	printf("font name: %s\n", sdl->font->name);
	printf("font size: %d\n", sdl->font->size);
	printf("font padding: %d\n", sdl->font->padding);
	printf("font spacing: %d\n", sdl->font->spacing);
	printf("font line_height: %d\n", sdl->font->line_height);
	printf("font scale.x: %d\n", sdl->font->scale.x);
	printf("font scale.y: %d\n", sdl->font->scale.y);
	printf("font char_count: %d\n", sdl->font->char_count);
	i = 0;
	while (i < sdl->font->char_count)
	{
		printf("char[%d] id: %d\n", i, sdl->font->chars[i].id);
		printf("char[%d] pos.x: %d\n", i, sdl->font->chars[i].pos.x);
		printf("char[%d] pos.y: %d\n", i, sdl->font->chars[i].pos.y);
		printf("char[%d] size.x: %d\n", i, sdl->font->chars[i].size.x);
		printf("char[%d] size.y: %d\n", i, sdl->font->chars[i].size.y);
		printf("char[%d] offset.x: %d\n", i, sdl->font->chars[i].offset.x);
		printf("char[%d] offset.y: %d\n", i, sdl->font->chars[i].offset.y);
		printf("char[%d] xadvance: %d\n", i, sdl->font->chars[i].xadvance);
		i++;
	}
	*/
}
