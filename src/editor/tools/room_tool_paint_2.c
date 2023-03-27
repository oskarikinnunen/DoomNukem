/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_paint_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:30:48 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:32:53 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

static void	area_apply_walltex_if_hit(t_area *area, t_entity *hit_ent,
		t_img *tex, t_world *world)
{
	int	i;

	i = 0;
	while (i < area->wall_count)
	{
		if (area->walls[i].entity == hit_ent)
		{
			ft_strncpy_term(area->walls[i].s_walltex.str, tex->name, 60);
			room_init(area, world);
			return ;
		}
		i++;
	}
}

static void	area_apply_floor_tex_if_hit(t_area *area, t_entity *hit_ent,
		t_img *tex, t_world *world)
{
	int	i;

	i = 0;
	while (i < area->floor_count)
	{
		if (area->floors[i].entity == hit_ent)
		{
			ft_strncpy_term(area->s_floor_tex.str, tex->name, 60);
			room_init(area, world);
			return ;
		}
		i++;
	}
}

static void	area_apply_ceilingtex_if_hit(t_area *area, t_entity *hit_ent,
		t_img *tex, t_world *world)
{
	int	i;

	i = 0;
	while (i < area->ceiling_count)
	{
		if (area->ceilings[i].entity == hit_ent)
		{
			ft_strncpy_term(area->s_ceil_tex.str, tex->name, 60);
			room_init(area, world);
			return ;
		}
		i++;
	}
}

void	paint_room(t_entity *hit_ent, t_img *tex, t_world *world)
{
	t_list	*l;
	t_area	*area;

	l = world->arealist;
	while (l != NULL)
	{
		area = l->content;
		area_apply_walltex_if_hit(area, hit_ent, tex, world);
		area_apply_floor_tex_if_hit(area, hit_ent, tex, world);
		area_apply_ceilingtex_if_hit(area, hit_ent, tex, world);
		l = l->next;
	}
}
