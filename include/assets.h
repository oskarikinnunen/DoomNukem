/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:33:13 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/07 16:07:17 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSETS_H
# define ASSETS_H

struct s_sdlcontext;

void	editor_load_assets(t_sdlcontext *sdl);
void	playmode_load_assets(t_sdlcontext *sdl);

#endif