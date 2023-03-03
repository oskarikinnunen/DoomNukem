/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 19:49:03 by raho              #+#    #+#             */
/*   Updated: 2023/03/03 19:01:58 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

typedef union	u_color
{
	uint32_t	color;
	struct		s_argb
	{
		uint8_t		blue; // reversed for intel storage order?
		uint8_t		green;
		uint8_t		red;
		uint8_t 	alpha;
	}	argb;
}	t_color;

uint32_t	blend_colors_alpha(uint32_t bg, uint32_t fg, uint8_t alpha);

// flips alpha values upside down
uint32_t	flip_alpha(uint32_t clr);

#endif
