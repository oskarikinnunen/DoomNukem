/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lua_conf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/24 20:51:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "doomnukem.h"
#include "png.h"

void	load_images(lua_State *lua, t_sdlcontext *sdl)
{
	char	imagename[256];
	char	indexer[256];
	int		i;

	lua_getglobal(lua, "images");
	sdl->imagecount = lua_rawlen(lua, -1);
	if (sdl->imagecount == 0)
	{
		printf("no images in settings.lua \n");
		return ;
	}
	sdl->images = ft_memalloc(sizeof(t_img) * sdl->imagecount);
	if (sdl->images == NULL)
		error_log(EC_MALLOC);
	i = 1;
	while (i <= sdl->imagecount)
	{
		sprintf(indexer, "eval=images[%i]", i);
		luaL_dostring(lua, indexer);
		lua_getglobal(lua, "eval");
		sprintf(imagename, "images/%s", lua_tostring(lua, -1));
		sdl->images[i - 1] = pngparse(imagename);
		i++;
	}
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
}

void	load_lua_conf(t_sdlcontext *sdl)
{
	lua_State	*lua;
	int			lua_return;
	int			imagecount;

	lua = luaL_newstate();
	luaL_openlibs(lua);
	lua_return = luaL_dofile(lua, "settings.lua");
	if (lua_return == LUA_OK)
	{
		load_resolution(lua, sdl);
		load_images(lua, sdl);
	}
	lua_close(lua);
	return ;
}