/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:03 by raho              #+#    #+#             */
/*   Updated: 2023/03/06 17:11:17 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

typedef struct s_rgba
{
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
	uint8_t	alpha;

}	t_rgba;

typedef union u_color
{
	uint32_t	color;
	t_rgba		rgba;
}	t_color;

typedef struct s_argbf
{
	float	a;
	float	r;
	float	g;
	float	b;
}	t_argbf;

SDL_Color	color32_to_sdlcolor(uint32_t color);

t_argbf		color_to_argbf(t_color color);

// OpenGL - transparency (alpha blending) Youtube
uint32_t	blend_colors_alpha(uint32_t bg, uint32_t fg, uint8_t alpha);

// flips alpha value upside down
uint32_t	flip_alpha(uint32_t clr);

// flips blue and red channels with each other
uint32_t	flip_rb_channels(uint32_t clr);

#endif
