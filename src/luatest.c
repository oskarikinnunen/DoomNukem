/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luatest.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:14:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/24 19:23:57 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "doomnukem.h"

void	load_lua_conf()
{
	lua_State	*lua;
	int			lua_return;
	int			imagecount;

	//lua = lua_newstate()
	lua = luaL_newstate();
	luaL_openlibs(lua);
	lua_return = luaL_dofile(lua, "test.lua");
	if (lua_return == LUA_OK)
	{
		lua_getglobal(lua, "images");
		imagecount = lua_rawlen(lua, -1);
		printf("image table had %i images \n", imagecount);
	}
	lua_close(lua);
	return ;
}