/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 17:12:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 18:21:32 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PNG_H
# define PNG_H

# include <inttypes.h>

typedef struct s_pngpalette
{
	uint32_t	*plte;
	uint32_t	length;
} t_pngpalette;

typedef struct s_pngdata
{
	uint32_t		width;
	uint32_t		height;
	uint8_t			*data;
	uint8_t			bitdepth;
	uint8_t			colortype;
	t_pngpalette	palette;
}	t_pngdata;

/* PNG.C */
/* \brief parses a png image
*/
struct s_img	pngparse(char *filename);

#endif