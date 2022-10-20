/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:11:21 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/19 13:30:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PNG_H
# define PNG_H

#include <inttypes.h>
#include "vectors.h"


typedef struct s_pngpalette
{
	uint64_t	*plte;
	uint32_t	length;
}	t_pngpalette;

typedef struct s_pngdata
{
	uint32_t		width;
	uint32_t		height;
	uint8_t			*data;
	uint8_t			bitdepth;
	uint8_t			colortype;
	t_pngpalette	palette;
}	t_pngdata;

/* parses a png image */
void	pngparse(t_pngdata *data, char *filename);

#endif