/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lua_conf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/01 20:33:46 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
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

void	load_images(lua_State *lua, t_sdlcontext *sdl)
{
	load_all_images(sdl);
}

void	load_objects(lua_State *lua, t_sdlcontext *sdl)
{
	load_all_objects(sdl);
}

void	load_resolution(lua_State *lua, t_sdlcontext *sdl)
{
	lua_getglobal(lua, "resolution_height");
	sdl->window_h = lua_tointeger(lua, -1);
	lua_getglobal(lua, "resolution_width");
	sdl->window_w = lua_tointeger(lua, -1);
	if (sdl->window_w <= 0 || sdl->window_h <= 0) 
	{
		printf("loading resolution info from settings.lua failed\n");
		sdl->window_h = 1280;
		sdl->window_w = 720;
	}
	sdl->screensize = (t_point) {sdl->window_w, sdl->window_h};
}

void	load_assets(t_sdlcontext *sdl)
{
	load_all_images(sdl);
	load_all_env_textures(sdl);
	load_all_objects(sdl);
	load_fonts(&sdl->font);
	load_audio(&sdl->audio);
	objects_init(sdl);
}

void	load_lua_conf(t_sdlcontext *sdl)
{
	lua_State	*lua;
	int			lua_return;
	int			imagecount;

	lua = luaL_newstate();
	luaL_openlibs(lua);
	lua_return = luaL_dofile(lua, "assets/settings.lua");
	if (lua_return == LUA_OK)
	{
		load_resolution(lua, sdl);
		load_images(lua, sdl);
		load_objects(lua, sdl);
	}
	else
	{
		printf("LUA FILE NOT OK \n");
		doomlog(LOGEC_SDL_INIT, NULL);
	}
	lua_close(lua);
	return ;
}