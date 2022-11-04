/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/11/02 21:01:56 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	set_font_size(t_sdlcontext *sdl, int font_size)
{
	sdl->font = &sdl->all_fonts[font_size];
}

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
		ret = get_next_line(fd, &line); // using Rene's gnl - invalid read of size 1 & 4 by valgrind using oskari's gnl
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

void	load_fonts(t_sdlcontext *sdl)
{
	int		fd;
	char	*line;
	int		ret;
	int		i;
	int		j;
	t_img	*bitmap;
	char	path[19];


	sdl->all_fonts = ft_memalloc(sizeof(t_font) * 7); // We have 7 different font sizes for now
	if (sdl->all_fonts == NULL)
		error_log(EC_MALLOC);
	ft_strcpy(path, "assets/fonts/0.fnt");
	i = 0;
	while (i < 7)
	{
		fd = open(path, O_RDONLY);
		if (fd < 0)
			error_log(EC_OPEN);
		save_font_info(&sdl->all_fonts[i], fd);
		sdl->all_fonts[i].chars = ft_memalloc(sizeof(t_font_chars) * sdl->all_fonts[i].char_count);
		if (sdl->all_fonts[i].chars == NULL)
			error_log(EC_MALLOC);
		line = NULL;
		j = 0;
		ret = 1;
		while (j < sdl->all_fonts[i].char_count)
		{
			ret = get_next_line(fd, &line); // using Rene's gnl - invalid read of size 1 & 4 valgrind using oskari's gnl
			if (line == NULL || ret < 0)
				error_log(EC_GETNEXTLINE);
			save_char_info(&sdl->all_fonts[i].chars[j], line);
			if (line)
				free(line);
			j++;
		}
		sdl->all_fonts[i].file_name = ft_strjoin(sdl->all_fonts[i].name, ".png");
		if (!sdl->all_fonts[i].file_name)
			error_log(EC_MALLOC);
		bitmap = get_image_by_name(*sdl, sdl->all_fonts[i].file_name);
		sdl->all_fonts[i].bitmap = bitmap;
		if (close(fd) == -1)
			error_log(EC_CLOSE);
		path[13] = path[13] + 1;
		i++;
	}
	set_font_size(sdl, 6);
	
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
