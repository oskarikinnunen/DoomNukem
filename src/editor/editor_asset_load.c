/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_asset_load.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/09 20:06:20 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "tga.h"
#include <dirent.h>
#include "file_io.h"

// returns a pointer to the start of the file's name without its path
char	*extract_filename(const char *filepath)
{
	char	*filename;

	filename = ft_strrchr(filepath, '/');
	filename++;
	return (filename);
}

int	count_asset_list(char *filename)
{
	char	*line;
	int		ret;
	int		fd;
	int		i;

	if (filename == NULL)
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		doomlog(LOG_EC_CLOSE, filename);
	line = NULL;
	ret = get_next_line(fd, &line);
	i = 0;
	while (ret)
	{
		i++;
		if (line)
		{
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);	
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, filename);
	fileclose(fd, filename);
	return (i);
}

void	editor_load_all_objects(t_sdlcontext *sdl)
{
	char	*object_path;
	int		ret;
	int		fd;
	int		i;

	doomlog(LOG_NORMAL, "LOADING OBJECTS");
	sdl->objectcount = count_asset_list(OBJLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){OBJLISTPATH, "size =", s_itoa(sdl->objectcount), NULL});
	sdl->objects = ft_memalloc(sizeof(t_object) * sdl->objectcount);
	if (sdl->objects == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->objects");
	fd = fileopen(OBJLISTPATH, O_RDONLY);
	object_path = NULL;
	ret = get_next_line(fd, &object_path);
	i = 0;
	while (ret)
	{
		if (object_path)
		{
			sdl->objects[i] = objparse(object_path);
			if (sdl->objects[i].vertices != NULL)
				ft_strcpy(sdl->objects[i].name, extract_filename(object_path));
			doomlog_mul(LOG_NORMAL, (char *[3]){"parsed .obj file:", sdl->objects[i].name, NULL});
			free(object_path);
			object_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &object_path);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, OBJLISTPATH);
	fileclose(fd, OBJLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){"parsed", s_itoa(i), "objectfiles", NULL});
}

void	editor_load_all_images(t_sdlcontext *sdl)
{
	char	*image_path;
	int		ret;
	int		fd;
	int		i;

	doomlog(LOG_NORMAL, "LOADING IMAGES");
	sdl->imagecount = count_asset_list(IMGLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){IMGLISTPATH, "size =", s_itoa(sdl->imagecount), NULL});
	sdl->images = ft_memalloc(sizeof(t_img) * sdl->imagecount);
	if (sdl->images == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->images");
	fd = fileopen(IMGLISTPATH, O_RDONLY);
	image_path = NULL;
	ret = get_next_line(fd, &image_path);
	i = 0;
	while (ret)
	{
		if (image_path)
		{
			sdl->images[i] = tgaparse(image_path);
			if (sdl->images[i].data != NULL)
				ft_strcpy(sdl->images[i].name, extract_filename(image_path));
			doomlog_mul(LOG_NORMAL, (char *[3]){"parsed .tga file:", sdl->images[i].name, NULL});
			free(image_path);
			image_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &image_path);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, IMGLISTPATH);
	fileclose(fd, IMGLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){"parsed", s_itoa(i), "imagefiles", NULL});
}

void	editor_load_all_env_textures(t_sdlcontext *sdl)
{
	char	*env_texture_path;
	int		ret;
	int		fd;
	int		i;

	doomlog(LOG_NORMAL, "LOADING ENV_TEXTURES");
	sdl->env_texturecount = count_asset_list(IMGENVLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){IMGENVLISTPATH, "size =", s_itoa(sdl->env_texturecount), NULL});
	sdl->env_textures = ft_memalloc(sizeof(t_img) * sdl->env_texturecount);
	if (sdl->env_textures == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->env_textures");
	fd = fileopen(IMGENVLISTPATH, O_RDONLY);
	env_texture_path = NULL;
	ret = get_next_line(fd, &env_texture_path);
	i = 0;
	while (ret)
	{
		if (env_texture_path)
		{
			sdl->env_textures[i] = tgaparse(env_texture_path);
			if (sdl->env_textures[i].data != NULL)
				ft_strcpy(sdl->env_textures[i].name, extract_filename(env_texture_path));
			doomlog_mul(LOG_NORMAL, (char *[3]){"parsed .tga file:", sdl->env_textures[i].name, NULL});
			free(env_texture_path);
			env_texture_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &env_texture_path);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, IMGENVLISTPATH);
	fileclose(fd, IMGENVLISTPATH);
	doomlog_mul(LOG_NORMAL, (char *[4]){"parsed", s_itoa(i), "env_textures", NULL});
}

#include "file_io.h"

void	editor_parse_anim_legend(t_sdlcontext *sdl)
{
	int					fd;
	char				*line;
	int					i;
	uint32_t			prev_frame;
	uint32_t			frame;
	int					ret;

	fd = fileopen("assets/objects/animations/anim_legend.txt", O_RDONLY);
	sdl->human_anims = ft_memalloc(sizeof(t_human_animation) * 30);
	if (sdl->human_anims == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->human_anims");
	sdl->human_anim_count = 0;
	i = 0;
	prev_frame = 0;
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (i % 2 == 0)
			{
				ft_bzero(&sdl->human_anims[sdl->human_anim_count], sizeof(t_human_animation));
				ft_memcpy(sdl->human_anims[sdl->human_anim_count].name, line, ft_strlen(line + 1));
			}
			else 
			{
				frame = ft_atoi(line);
				sdl->human_anims[sdl->human_anim_count].endframe = frame;
				sdl->human_anims[sdl->human_anim_count].startframe = prev_frame;
				prev_frame = frame;
				sdl->human_anim_count++;
			}
			i++;
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "anim_legend.txt");
}

void	editor_load_assets(t_sdlcontext *sdl)
{
	editor_load_all_images(sdl);
	editor_load_all_env_textures(sdl);
	editor_load_all_objects(sdl);
	editor_load_fonts(sdl);
	editor_load_audio(&sdl->audio);
	objects_init(sdl);
	t_object *human = get_object_by_name(*sdl, "Human.obj");
	parseanim(human, "anim");
	editor_parse_anim_legend(sdl);
	int i = 0;
	while (i < sdl->human_anim_count)
	{
		printf("anim %s frames %i->%i\n", sdl->human_anims[i].name, sdl->human_anims[i].startframe, sdl->human_anims[i].endframe);
		i++;
	}
}