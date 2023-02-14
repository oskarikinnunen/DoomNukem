/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perfgraph.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:20:11 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/24 17:31:11 by okinnune         ###   ########.fr       */
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
	//put_image_to_screen(sdl, graph->image, (int [2]) {10, 10});
}
