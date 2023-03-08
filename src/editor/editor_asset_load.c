/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_asset_load.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/08 14:57:52 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "tga.h"
#include <dirent.h>

void	editor_allocate_object_count(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/objects";
	int				i;

	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".obj") != NULL)
				i++;
			dfile = readdir(d);
		}
		closedir(d);
	}
	sdl->objectcount = i;
	printf("Found %i .obj files \n", sdl->objectcount);
	sdl->objects = ft_memalloc(sizeof(t_object) * sdl->objectcount);
}

void	editor_load_all_objects(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/objects";
	char fullpath	[512];
	int				i;

	editor_allocate_object_count(sdl);
	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".obj") != NULL)
			{
				snprintf(fullpath, 512, "%s/%s", path, dfile->d_name);
				sdl->objects[i] = objparse(fullpath);
				ft_strcpy(sdl->objects[i].name, dfile->d_name);
				printf("	parsed objectfile: %s \n", fullpath);
				i++;
			}
			dfile = readdir(d);
		}
		closedir(d);
	}
	printf("parsed %i objectfiles \n", i);
}

void	editor_allocate_image_count(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/images/tga";
	int				i;

	d = opendir(path); // TODO: is opendir allowed?
	i = 0;
	if (d)
	{
		dfile = readdir(d); // TODO: is readdir allowed?
		while (dfile != NULL)
		{
			//if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".cng") != NULL)
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".tga") != NULL)
				i++;
			dfile = readdir(d);
		}
		closedir(d);
	}
	sdl->imagecount = i;
	sdl->images = ft_memalloc(sizeof(t_img) * sdl->imagecount);
}

void	editor_allocate_env_texturecount(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/images/tga/env";
	int				i;

	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".tga") != NULL)
				i++;
			dfile = readdir(d);
		}
		closedir(d);
	}
	sdl->env_texturecount = i;
	sdl->env_textures = ft_memalloc(sizeof(t_img) * sdl->env_texturecount);
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
	if (close(fd) == -1)
		doomlog(LOG_EC_CLOSE, filename);
	return (i);
}

// returns a pointer to the start of the file's name without its path
char	*extract_filename(const char *filepath)
{
	char	*filename;

	filename = ft_strrchr(filepath, '/');
	filename++;
	return (filename);
}

void	editor_load_all_images(t_sdlcontext *sdl, char *image_list)
{
	char	*image_path;
	int		ret;
	int		fd;
	int		i;

	printf("LOAD IMAGES! \n");
	sdl->imagecount = count_asset_list(image_list);
	printf(".image_list.txt size = %i\n", sdl->imagecount);
	sdl->images = ft_memalloc(sizeof(t_img) * sdl->imagecount);
	if (sdl->images == NULL)
		doomlog(LOG_EC_MALLOC, "sdl->images");
	fd = open(image_list, O_RDONLY);
	if (fd == -1)
		doomlog(LOG_EC_OPEN, image_list);
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
			printf("	parsed tga file: %s \n", sdl->images[i].name);
			free(image_path);
			image_path = NULL;
			i++;
		}
		ret = get_next_line(fd, &image_path);			
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, image_list);
	doomlog_mul(LOG_NORMAL, (char *[4]){"parsed", s_itoa(i), "imagefiles", NULL});
}

void	editor_load_all_env_textures(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/images/tga/env";
	char fullpath	[512];
	int				i;

	printf("LOAD ENV_TEX! \n");
	editor_allocate_env_texturecount(sdl);
	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".tga") != NULL)
			{
				snprintf(fullpath, 512, "%s/%s", path, dfile->d_name);
				sdl->env_textures[i] = tgaparse(fullpath);
				if (sdl->env_textures[i].data != NULL)
					ft_strcpy(sdl->env_textures[i].name, dfile->d_name);
				printf("	parsed tga file: %s \n", fullpath);
				i++;
			}
			dfile = readdir(d);
		}
		closedir(d);
	}
	printf("parsed %i env_textures \n", i);
}

#include "file_io.h"

void	editor_parse_anim_legend(t_sdlcontext *sdl)
{
	int					fd;
	char				*line;
	int					i;
	uint32_t			prev_frame;
	uint32_t			frame;

	fd = open("assets/objects/animations/anim_legend.txt", O_RDONLY);
	if (fd == -1)
	{
		//TODO: doom log "couldn't open anim legend, EC"
	}
	else
	{
		sdl->human_anims = ft_memalloc(sizeof(t_human_animation) * 30);
		sdl->human_anim_count = 0;
		i = 0;
		prev_frame = 0;
		printf("prev frame %i \n", prev_frame);
		while (ft_get_next_line(fd, &line))
		{
			if (i % 2 == 0)
			{
				ft_bzero(&sdl->human_anims[sdl->human_anim_count], sizeof(t_human_animation));
				//ft_strcpy(sdl->human_anims[sdl->human_anim_count].name, line);
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
		}
	}
}

void	editor_load_assets(t_sdlcontext *sdl)
{
	editor_load_all_images(sdl, "assets/.image_list.txt");
	editor_load_all_env_textures(sdl);
	editor_load_all_objects(sdl);
	editor_load_fonts(&sdl->font);
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
	return ;
	//object_
}