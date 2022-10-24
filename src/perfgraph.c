/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perfgraph.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:20:11 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/24 20:35:57 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
 
Uint32	samplecolor(t_img img, int ix, int iy)
{
	int	index;

	index = ix + (iy * img.size.y);
	if (index < 0)
		index = 0;
	if (ix >= (img.size.x * img.size.y) - 1 || iy >= img.size.y - 1)
		index = img.length - 1;
	if (img.length == 0)
		return (0);
	return (img.data[index]);
}

void	put_image_to_screen(t_sdlcontext sdl, t_img img, int p[2])
{
	int			iy;
	int			ix;
	uint32_t	color;

	iy = 0;
	while (iy++ < img.size.y)
	{
		ix = 0;
		if (iy + p[Y] < 0 || iy + p[Y] > sdl.window_h)
			continue ;
		while (ix++ < img.size.x - 1)
		{
			if (ix + p[X] < 0 || ix + p[X] >= sdl.window_w)
				continue ;
			color = samplecolor(img, ix, iy);
			draw(sdl,
				(t_point){ix + p[X], iy + p[Y]}, color);
		}
	}
}

void	drawperfgraph(t_perfgraph *graph, uint32_t delta, t_sdlcontext sdl)
{
	int			i;
	t_img		img;

	img = graph->image;
	ft_bzero(img.data, img.length * sizeof(uint32_t));
	i = 0;
	while (i < PERFGRAPH_SAMPLES - 1)
	{
		graph->deltas[i] = graph->deltas[i + 1];
		graph->deltas[i]
			= ft_clamp(graph->deltas[i], 0, graph->image.size.y - 1);
		img.data[i + (graph->deltas[i] * img.size.x)] = CLR_TURQ;
		i++;
	}
	graph->deltas[PERFGRAPH_SAMPLES - 1] = delta;
	put_image_to_screen(sdl, graph->image, (int [2]) {10, 10});
}
