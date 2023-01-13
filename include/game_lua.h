/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_lua.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:33:13 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/11 14:25:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_LUA_H
# define GAME_LUA_H

struct s_sdlcontext;

void	load_lua_conf(struct s_sdlcontext *sdl);
void	load_assets(t_sdlcontext *sdl);

#endif