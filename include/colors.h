/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:03 by raho              #+#    #+#             */
/*   Updated: 2023/03/07 12:55:48 by raho             ###   ########.fr       */
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

}	t_rgb;

/* colors are in the order of bgra (blue green red alhpa),
	because sdl surface flips blue and red */
typedef union u_color
{
	uint32_t	color;
	struct		s_bgra
	{
		uint8_t	blue;
		uint8_t	green;
		uint8_t	red;
		uint8_t	alpha;
	}	bgra;
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

// flips alpha values upside down
uint32_t	flip_alpha(uint32_t clr);

#endif
