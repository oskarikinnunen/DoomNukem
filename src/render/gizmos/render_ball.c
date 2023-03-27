/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ball.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 15:10:29 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 11:31:54 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "render.h"

static void	render_ball_xy(t_vector3 *edges, t_vector3 pos,
							float size, t_sdlcontext *sdl)
{
	int		i;
	float	angl;

	angl = 0.0f;
	i = 0;
	while (i < RCRCL_SIDES + 1)
	{
		edges[i].x = pos.x + (sinf(angl) * size);
		edges[i].y = pos.y + (cosf(angl) * size);
		edges[i].z = pos.z;
		if (i >= 1)
		{
			render_ray3d(sdl, edges[i - 1], edges[i], sdl->render.gizmocolor);
		}
		angl += FULLRAD / RCRCL_SIDES;
		i++;
	}
}

static void	render_ball_yz(t_vector3 *edges, t_vector3 pos,
							float size, t_sdlcontext *sdl)
{
	int		i;
	float	angl;

	angl = 0.0f;
	i = 0;
	while (i < RCRCL_SIDES + 1)
	{
		edges[i].x = pos.x;
		edges[i].y = pos.y + (cosf(angl) * size);
		edges[i].z = pos.z - (sinf(angl) * size);
		if (i >= 1)
			render_ray3d(sdl, edges[i - 1], edges[i], sdl->render.gizmocolor);
		angl += FULLRAD / RCRCL_SIDES;
		i++;
	}
}

static void	render_ball_xz(t_vector3 *edges, t_vector3 pos,
							float size, t_sdlcontext *sdl)
{
	int		i;
	float	angl;

	angl = 0.0f;
	i = 0;
	while (i < RCRCL_SIDES + 1)
	{
		edges[i].x = pos.x + (sinf(angl) * size);
		edges[i].y = pos.y;
		edges[i].z = pos.z + (cosf(angl) * size);
		if (i >= 1)
			render_ray3d(sdl, edges[i - 1], edges[i], sdl->render.gizmocolor);
		angl += FULLRAD / RCRCL_SIDES;
		i++;
	}
}

void	render_ball(t_sdlcontext *sdl, t_vector3 pos,
						float size, uint32_t clr)
{
	t_vector3	edges[RCRCL_SIDES + 1];

	ft_bzero(&edges, sizeof(t_vector3));
	sdl->render.gizmocolor = clr;
	render_ball_xy(edges, pos, size, sdl);
	render_ball_yz(edges, pos, size, sdl);
	render_ball_xz(edges, pos, size, sdl);
}
