/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:33:13 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 17:13:28 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSETS_H
# define ASSETS_H

struct s_sdlcontext;

int		count_asset_list(char *filename);
void	editor_load_assets(t_sdlcontext *sdl);
void	playmode_load_assets(char *level_name, t_sdlcontext *sdl);

#endif