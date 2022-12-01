/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:03 by raho              #+#    #+#             */
/*   Updated: 2022/12/01 20:57:43 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

typedef union u_color
{
	uint32_t	hexadecimal;
	struct s_uargb
	{
		uint8_t		blue; // reversed for intel storage order?
		uint8_t		green;
		uint8_t		red;
		uint8_t 	alpha;
	}	uargb;
}	t_color;

uint32_t	blend_colors_alpha(uint32_t bg, uint32_t fg, SDL_PixelFormat *fmt, uint8_t alpha);

#endif
