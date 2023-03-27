/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc_funcs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:22:39 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:25:52 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "movement_defs.h"
#include <stdlib.h>
#include <time.h>
#include "npc.h"

//TODO: move to header?
void	comp_npc_update(t_entity *entity, t_world *world);

static void	render_navnode(t_entity *entity,
						t_nav_node *navnode, t_sdlcontext *sdl)
{
	float	fdist;
	int		i;

	fdist = vector3_sqr_dist(entity->transform.position, navnode->mid_point);
	if (fdist > 60000.0f)
		return ;
	i = 0;
	while (i < 3)
	{
		render_ray3d(sdl, navnode->vertex[i],
			navnode->vertex[(i + 1) % 3], AMBER_2);
		i++;
	}
}

static void	comp_npc_ui_update(t_entity *entity, t_world *world)
{
	t_npc			*npc;
	int				i;

	if (world->app_mode == APPMODE_EDIT || 1)
		return ;
	npc = entity->component.data;
	render_ray3d(world->sdl, entity->transform.position,
		npc->path.path[npc->path.ai].enter_point, CLR_RED);
	i = 0;
	while (i < npc->path.bi)
	{
		render_ray3d(world->sdl, npc->path.path[i].enter_point,
			npc->path.path[(i + 1)].enter_point, CLR_RED);
		render_circle(world->sdl,
			npc->path.path[i].enter_point, 10.0f, CLR_GREEN);
		i++;
	}
	i = 0;
	while (i < world->nav.node_amount)
	{
		if (!world->nav.navmesh[i].blocked)
			render_navnode(entity, &world->nav.navmesh[i], world->sdl);
		i++;
	}
	render_circle(world->sdl, npc->last_seen_player_pos, 6.0f, AMBER_2);
}

static void	comp_npc_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_npc			*npc;

	npc = (t_npc *)entity->component.data;
	if (npc == NULL)
	{
		gui_label("!NULL source", gui);
		return ;
	}
	gui_start_horizontal(gui);
	gui_label("NPC: ", gui);
	gui_end_horizontal(gui);
	gui_labeled_bool("ANIM:", entity->animation.active, gui);
	gui_labeled_int("i:", entity->animation.frame, gui);
}

static void	comp_npc_allocate(t_entity *entity, t_world *world)
{
	t_npc	*npc;

	entity->component.data = prot_memalloc(sizeof(t_npc));
	entity->component.data_size = sizeof(t_npc);
	entity->dynamic_lit = true;
	npc = entity->component.data;
	ft_bzero(entity->component.data, sizeof(t_npc));
	npc->movement_speed = 0.1f;
	npc->path.valid_path = false;
	npc->health = 100;
}

void	assign_component_npc(t_component *component)
{
	component->data_size = sizeof(t_npc);
	component_empty_function_pointers(component);
	component->func_allocate = comp_npc_allocate;
	component->func_update = comp_npc_update;
	component->func_ui_update = comp_npc_ui_update;
	component->func_gui_edit = comp_npc_gui_edit;
}
