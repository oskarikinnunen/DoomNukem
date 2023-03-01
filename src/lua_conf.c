/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lua_conf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/01 21:09:33 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "png.h"
#include <dirent.h>

void	allocate_object_count(t_sdlcontext *sdl)
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

void	load_all_objects(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/objects";
	char fullpath	[512];
	int				i;

	allocate_object_count(sdl);
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

void	allocate_image_count(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/images";
	int				i;

	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".cng") != NULL)
				i++;
			dfile = readdir(d);
		}
		closedir(d);
	}
	sdl->imagecount = i;
	sdl->images = ft_memalloc(sizeof(t_img) * sdl->imagecount);
}

void	allocate_env_texturecount(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/images/env";
	int				i;

	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".cng") != NULL)
				i++;
			dfile = readdir(d);
		}
		closedir(d);
	}
	sdl->env_texturecount = i;
	sdl->env_textures = ft_memalloc(sizeof(t_img) * sdl->env_texturecount);
}

void	load_all_images(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/images";
	char fullpath	[512];
	int				i;

	printf("LOAD IMAGES! \n");
	allocate_image_count(sdl);
	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".cng") != NULL)
			{
				snprintf(fullpath, 512, "%s/%s", path, dfile->d_name);
				sdl->images[i] = pngparse(fullpath);
				ft_strcpy(sdl->images[i].name, dfile->d_name);
				printf("	parsed cpng file: %s \n", fullpath);
				i++;
			}
			dfile = readdir(d);
		}
		closedir(d);
	}
	doomlog_mul(LOG_NORMAL, (char *[32]){"parsed", s_itoa(i), "imagefiles", NULL});
}

void	load_all_env_textures(t_sdlcontext *sdl)
{
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "assets/images/env";
	char fullpath	[512];
	int				i;

	printf("LOAD ENV_TEX! \n");
	allocate_env_texturecount(sdl);
	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".cng") != NULL)
			{
				snprintf(fullpath, 512, "%s/%s", path, dfile->d_name);
				sdl->env_textures[i] = pngparse(fullpath);
				ft_strcpy(sdl->env_textures[i].name, dfile->d_name);
				printf("	parsed cpng file: %s \n", fullpath);
				i++;
			}
			dfile = readdir(d);
		}
		closedir(d);
	}
	printf("parsed %i env_textures \n", i);
}

#include "file_io.h"

void	parse_anim_legend(t_sdlcontext *sdl)
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

void	load_assets(t_sdlcontext *sdl)
{
	load_all_images(sdl);
	load_all_env_textures(sdl);
	load_all_objects(sdl);
	load_fonts(&sdl->font);
	load_audio(&sdl->audio);
	objects_init(sdl);
	t_object *human = get_object_by_name(*sdl, "Human.obj");
	parseanim(human, "anim");
	parse_anim_legend(sdl);
	int i = 0;
	while (i < sdl->human_anim_count)
	{
		printf("anim %s frames %i->%i\n", sdl->human_anims[i].name, sdl->human_anims[i].startframe, sdl->human_anims[i].endframe);
		i++;
	}
	return ;
	//object_
}