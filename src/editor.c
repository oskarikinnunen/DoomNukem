/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/31 00:58:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "inputhelp.h"

static void	drawsquare(t_sdlcontext sdl, t_point crd, int clr)
{
	int	i;

	i = 0;
	while (i < TILESIZE)
	{
		draw(sdl, point_add(crd, (t_point){i, 0}), clr);
		draw(sdl, point_add(crd, (t_point){0, i}), clr);
		draw(sdl, point_add(crd, (t_point){TILESIZE, i}), clr);
		draw(sdl, point_add(crd, (t_point){TILESIZE, i}), clr);
		i++;
	}
}

static void	drawgrid(t_sdlcontext sdl, t_point origin)
{
	t_point	crd;

	crd.y = origin.y;
	while (crd.y < (TILESIZE * GRIDSIZE) + origin.y)
	{
		crd.x = origin.x;
		while (crd.x < (TILESIZE * GRIDSIZE) + origin.x)
		{
			drawsquare(sdl, crd, CLR_GRAY);
			crd.x += TILESIZE;
		}
		crd.y += TILESIZE;
	}
}

void	buttoncreator(t_editor *ed, t_sdlcontext sdl);

static char	*ft_strcdup(const char *str, char c)
{
	int		index;
	char	*new;

	index = 0;
	while (str[index] != '\0' && str[index] != c)
		index++;
	new = (char *)malloc(sizeof(char) * (index + 1));
	if (new == NULL)
		return (NULL);
	index = 0;
	while (str[index] != '\0' && str[index] != c)
	{
		new[index] = str[index];
		index++;
	}
	new[index] = '\0';
	return (new);
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

static void	load_font(t_sdlcontext *sdl, const char *filename)
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
			//free(line);
		i++;
	}
	sdl->font->file_name = ft_strjoin(sdl->font->name, ".png");
	bitmap = get_image_by_name(*sdl, sdl->font->file_name);
	sdl->font->bitmap = bitmap;
	if (close(fd) == -1)
		error_log(EC_CLOSE);

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

static void	paint_font(t_img *bitmap)
{
	t_point	i;

	i.y = 0;
	while (i.y < bitmap->size.y)
	{
		i.x = 0;
		while (i.x < bitmap->size.x)
		{
			if (bitmap->data[i.x + (i.y * bitmap->size.x)] != 0x000000) // testing black background
				bitmap->data[i.x + (i.y * bitmap->size.x)] = 0x00FF00;
			i.x++;
		}
		i.y++;
	}
}

// ~ Character is so thin and at the bottom of the picture that I think it takes some colors outside of the image

static void	save_text(t_font *font, const char *str)
{
	t_img		*text;
	int			i;
	int			j;
	t_point		cursor;
	t_point		counter;
	int			text_width;
	static int	text_nbr = 0;

	//paint_font(font->bitmap); // for testing

	text = ft_memalloc(sizeof(t_img));
	if (!text)
		error_log(EC_MALLOC);

	// first calculating the amount of space needed by the text and then copying the characters to the image
	
	text_width = 0;
	i = 0;
	while (str[i] != '\0')
	{
		j = str[i] - 32;
		text_width += font->chars[j].xadvance;
		i++;
	}
	alloc_image(text, text_width + 10, font->line_height + 10);
	ft_strcpy(text->name, str);
	cursor.x = 5;
	cursor.y = 5;
	i = 0;
	while (str[i] != '\0')
	{
		j = str[i] - 32;
		cursor.x += font->chars[j].offset.x;
		cursor.y += font->chars[j].offset.y;
		counter.y = 0;
		while (counter.y < font->chars[j].size.y)
		{
			counter.x = 0;
			while (counter.x < font->chars[j].size.x)
			{
				//printf("[%d][%d]: %d\n", counter.y, counter.x, font->bitmap->data[(font->chars[j].pos.x + counter.x) + ((font->chars[j].pos.y + counter.y) * font->bitmap->size.x)]);
				//if (font->bitmap->data[(font->chars[j].pos.x + counter.x) + ((font->chars[j].pos.y + counter.y) * font->bitmap->size.x)] != 0x000000) // testing black background
				text->data[(cursor.x + counter.x) + ((cursor.y + counter.y) * text->size.x)] = font->bitmap->data[(font->chars[j].pos.x + counter.x) + ((font->chars[j].pos.y + counter.y) * font->bitmap->size.x)];
				counter.x++;
			}
			counter.y++;
		}
		cursor.x -= font->chars[j].offset.x;
		cursor.y -= font->chars[j].offset.y;
		cursor.x += font->chars[j].xadvance;
		i++;
	}
	font->texts[text_nbr] = text;
	text_nbr++;
}

static void	draw_text(t_sdlcontext *sdl, t_img *text, t_point pos)
{
	t_point		pixel;
	uint32_t	color;

	pixel.y = 0;
	while (pixel.y < text->size.y)
	{
		pixel.x = 0;
		while (pixel.x < text->size.x)
		{
			color = text->data[pixel.x + (pixel.y * text->size.x)];
			draw(*sdl, point_add(pixel, pos), color);
			pixel.x++;
		}
		pixel.y++;
	}
}

int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;
	t_gamereturn	gr;

	load_font(&sdl, "assets/fonts/sans-serif-black-bg.fnt");
	save_text(sdl.font, "Your text goes here");
	save_text(sdl.font, "Wanna try me guv? Q(._.Q)");
	bzero(&ed, sizeof(t_editor));
	ed.linelist = load_chunk("map_test1", "WALL", sizeof(t_line));
	ed.entitylist = load_chunk("map_test1", "ENT_", sizeof(t_entity));
	ed.buttonlist = load_chunk("buttons", "BUTN", sizeof(t_guibutton));
	initialize_buttons(ed.buttonlist, sdl);
	ed.tool = get_point_tool();
	gr = game_continue;
	while (gr == game_continue)
	{
		update_deltatime(&ed.clock);
		ft_bzero(sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		gr = editor_events(&ed);
		drawgrid(sdl, ed.offset);
		renderlines(&sdl, &ed);
		draw_buttons(ed, sdl);
		if (ed.tool != NULL)
		{
			ed.tool->update(&ed);
			ed.tool->draw_update(&ed, sdl);
			if (ed.tool->icon != NULL) //Indicates which tool is selected
				draw_image(sdl, (t_point){ 8, sdl.window_h - 40 }, *ed.tool->icon, (t_point){32, 32});
			else if (ed.tool->icon_name[0] != '\0')
				ed.tool->icon = get_image_by_name(sdl, ed.tool->icon_name);
		}
		draw_text(&sdl, sdl.font->texts[0], (t_point){10, 100});
		draw_text(&sdl, sdl.font->texts[1], (t_point){10, 200});
		draw_text(&sdl, sdl.font->texts[0], (t_point){10, 300});
		ed.mouse.click_unhandled = false;
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	save_lists_to_file(&ed);
	if (gr == game_exit)
		quit_game(&sdl);
	return (gr);
}