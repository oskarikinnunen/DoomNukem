/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nav_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:45:00 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/28 12:17:24 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void	draw_nav_nodes(t_world *world, int i, float dist)
{
	uint32_t	clr;
	t_point		sp;
	int			j;

	sp = vector3_to_screenspace(world->nav.navmesh[i].mid_point, *world->sdl);
	print_text_boxed(world->sdl, s_itoa(world->nav.navmesh[i].neighbors), sp);
	clr = AMBER_1;
	if (dist < 30000.0f)
		clr = AMBER_3;
	j = 0;
	while (j < 3)
	{
		render_ray3d(world->sdl, world->nav.navmesh[i].vertex[j], \
		world->nav.navmesh[i].vertex[(j + 1) % 3], clr);
		j++;
	}
	j = 0;
	while (j < world->nav.navmesh[i].neighbors)
	{
		render_ray3d(world->sdl, world->nav.navmesh[i].mid_point, \
		world->nav.navmesh[\
		world->nav.navmesh[i].neighbors_id[j]].mid_point, CLR_RED);
		j++;
	}
}

void	*ft_realloc(void *src, size_t dest_size, size_t src_size)
{
	void	*dest;

	dest = prot_memalloc(dest_size);
	ft_memcpy(dest, src, src_size);
	if (src)
		free(src);
	return (dest);
}

void	show_navmesh(t_world *world)
{
	int			i;
	int			j;
	float		dist;

	if (!world || !world->nav.show_navmesh
		|| world->nav.navmesh == NULL)
		return ;
	i = 0;
	while (i < world->nav.node_amount)
	{
		dist = vector3_sqr_dist(world->player->transform.position, \
		world->nav.navmesh[i].mid_point);
		if (dist < 60000.0f)
			draw_nav_nodes(world, i, dist);
		i++;
	}
}
