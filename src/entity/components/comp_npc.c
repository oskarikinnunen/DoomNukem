/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_npc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:57:39 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 16:43:48 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "movement_defs.h"
#include <stdlib.h>
#include <time.h>

static void	npc_update_physics(t_npc *npc, t_entity *entity)
{
	npc->phys.position = &entity->transform.position;
	npc->phys.radius = 15.0f;
	npc->phys.gravity_override = NULL;
	npc->phys.height = PLAYER_HEIGHT;
	npc->phys.max_velocity = PLAYER_WALKSPEED;
}

bool	npc_player_within_viewcone(t_entity *npc_ent, t_player *player)
{
	float	angle_between;

	angle_between = vector2_anglebetween(vector2_zero(),
			v3tov2(vector3_sub(npc_ent->transform.position,
					player->transform.position)))
		+ RAD90;
	if (ft_absf(npc_ent->transform.rotation.x - angle_between) < RAD90 / 2.0f)
		return (true);
	return (false);
}

static inline void	start_random_death_anim(t_entity *entity, t_world *world)
{
	uint32_t	random;

	random = game_random_range(world, 1, 4);
	if (random == 1)
		start_human_anim(entity, "Death1", world);
	if (random == 2)
		start_human_anim(entity, "Death2", world);
	if (random == 3)
		start_human_anim(entity, "Death3", world);
	if (random == 4)
		start_human_anim(entity, "Death4", world);
	entity->animation.loop = false;
	entity->animation.persist = true;
}

//"animation" for when the npc starts moving towards player,
//	while the npc is in state NPC_STATE_AIM

void	npc_update_state(t_entity *entity, t_npc *npc, t_world *world);

void	npc_play_sound(t_entity *entity, t_world *world, char *soundname)
{
	t_npc			*npc;
	t_audiosource	source;

	if (entity->component.type != COMP_NPC)
		return ;
	npc = entity->component.data;
	source = npc->audiosource;
	source.sample = get_sample(world->sdl, soundname);
	source._realrange = 250.0f;
	source.volume = 1.0f;
	source.play_always = false;
	_audiosource_start(world->sdl, &source, &entity->transform.position);
}

void	npc_shoot(t_entity *entity, t_npc *npc, t_world *world)
{
	uint32_t		hitdice;
	uint32_t		hitchance;

	if (world->player->health == 0)
		return ;
	npc_play_sound(entity, world, "gun_machinegun.wav");
	if (npc->seesplayer)
	{
		hitdice = game_random_range(world, 0, 10);
		hitchance = 5;
		if (vector3_magnitude(world->player->cp.velocity) > 0.5f)
			hitchance = 2;
		if (hitdice < hitchance)
		{
			protagonist_play_audio(world->player, world, "protag_hurt.wav");
			world->player->health -= 10;
			world->player->lasthurttime = world->clock.time;
			world->player->lasthurtpos = entity->transform.position;
		}
		else
			protagonist_play_audio(world->player, world, "ricochet.wav");
	}
	npc->next_shoot_time = world->clock.time
		+ game_random_range(world, 750, 2200);
}

//TODO: ugly norm
static void	entity_rotatetowards_xy(t_entity *entity,
									t_vector2 pos, float delta)
{
	float	angle;

	angle = vector2_anglebetween(vector2_zero(),
			vector2_sub(v3tov2(entity->transform.position), pos));
	entity->transform.rotation.x
		= ft_fmovetowards(entity->transform.rotation.x, angle + RAD90, delta);
}

static void	entity_rotatetowards_player(t_entity *entity,
										float speed, t_world *world)
{
	float	angle;

	angle = vector2_anglebetween(vector2_zero(),
			v3tov2(vector3_sub(entity->transform.position,
					world->player->transform.position)));
	entity->transform.rotation.x
		= ft_fmovetowards(entity->transform.rotation.x,
			angle + RAD90, world->clock.delta * speed);
}

static void	npc_pathfind_step(t_entity *entity, t_npc *npc, t_world *world)
{
	t_vector2	enterpoint_2d;
	t_vector2	move;

	if (!npc->path.valid_path || npc->state == NPC_STATE_AGGRO)
		return ;
	enterpoint_2d = v3tov2(npc->path.path[npc->path.ai].enter_point);
	move = vector2_sub(enterpoint_2d, v3tov2(entity->transform.position));
	move = vector2_mul(vector2_normalise(move), world->clock.delta
			* PLAYER_ACCELERATION);
	capsule_add_xy_velocity(move, &npc->phys, world);
	entity_rotatetowards_xy(entity, enterpoint_2d, 0.015f * world->clock.delta);
	if (vector3_sqr_dist(entity->transform.position,
			npc->path.path[npc->path.ai].enter_point) < 160.1f
		&& npc->path.ai < npc->path.bi)
		npc->path.ai++;
	if (npc->path.ai >= npc->path.bi && npc->path.valid_path)
		npc->path.valid_path = false;
}

static void	npc_update_anim_state(t_entity *entity, t_npc *npc, t_world *world)
{
	bool	changed;

	changed = npc->prev_state != npc->state;
	if (!changed)
		return ;
	if (npc->state == NPC_STATE_AGGRO)
	{
		start_human_anim(entity, "Shoot_rifle", world);
		update_anim(&entity->animation, world->clock.delta);
	}
	if (npc->state == NPC_STATE_CHASE
		|| npc->state == NPC_STATE_CAUTIOUS_MOVE
		|| npc->state == NPC_STATE_AGGRO_PUSH)
		start_human_anim(entity, "Walk_weapon", world);
	if (npc->state == NPC_STATE_DEAD)
	{
		printf("prev state %i \n", npc->prev_state);
		printf("starting death anim \n");
		npc->prev_state = npc->state;
		start_random_death_anim(entity, world);
	}
		
}

static void	lookaround(t_entity *entity, t_npc *npc, t_world *world)
{
	if (npc->state == NPC_STATE_CAUTIOUS_MOVE && npc->orig_x_rotation != 0)
		entity->transform.rotation.x = ft_fmovetowards(
				entity->transform.rotation.x, npc->orig_x_rotation
				+ (sinf(world->clock.time * 0.0012f) * RAD90) - (RAD90 / 2.0f),
				world->clock.delta * 0.015f);
}


static inline void	npc_update_pushanim(t_entity *entity, t_npc *npc, t_world *world)
{
	if (npc->state != NPC_STATE_AGGRO_PUSH || !npc->push_anim.active)
		return ;
	update_anim(&npc->push_anim, world->clock.delta);
	if (!npc->push_anim.active) //Should change state to NPC_STATE_AIM
		start_human_anim(entity, "Shoot_rifle", world);
	t_vector2 move;
	move = v3tov2(vector3_sub(world->player->transform.position, entity->transform.position));
	capsule_add_xy_velocity(move, &npc->phys, world);
}

//TODO: this file is norminette compliant down to here (lines 0-207 are ok)
static void	comp_npc_update(t_entity *entity, t_world *world)
{
	t_npc	*npc;

	npc = entity->component.data;
	if (npc == NULL || world->gamemode == MODE_EDITOR
		|| world->player->health == 0)
		return ;
	update_anim(&entity->animation, world->clock.delta);
	if (npc->state != NPC_STATE_DEAD)
	{
		npc_update_state(entity, npc, world); //TODO: figure where it resets to aggro after death
		if (npc->state == NPC_STATE_AGGRO || npc->state == NPC_STATE_AGGRO_PUSH)
		{
			if (world->clock.time > npc->next_shoot_time)
				npc_shoot(entity, npc, world);
			entity_rotatetowards_xy(entity,
				v3tov2(npc->lastseen_playerpos), 0.015f * world->clock.delta);
		}
		if (npc->state == NPC_STATE_AGGRO_PUSH)
			npc_update_pushanim(entity, npc, world);
		lookaround(entity, npc, world);
		npc_pathfind_step(entity, npc, world);
	} else {
		printf("npc dead state in npc_update! \n\n");
	}
	npc_update_physics(npc, entity);
	capsule_applygravity_new(&npc->phys, world);
	npc_update_anim_state(entity, npc, world);
}

static void	render_navnode(t_entity *entity,
						t_navnode *navnode, t_sdlcontext *sdl)
{
	float	fdist;
	int		i;

	fdist = vector3_sqr_dist(entity->transform.position, navnode->mid_point);
	if (fdist > 60000.0f)
		return ;
	i = 0;
	while (i < 3)
	{
		render_ray3D(sdl, navnode->vertex[i],
			navnode->vertex[(i + 1) % 3], AMBER_2);
		i++;
	}
}

static void	comp_npc_ui_update(t_entity *entity, t_world *world)
{
	t_npc			*npc;
	int				i;

	if (world->gamemode == MODE_EDITOR || 1)
		return ;
	npc = entity->component.data;
	render_ray3D(world->sdl, entity->transform.position,
		npc->path.path[npc->path.ai].enter_point, CLR_RED);
	i = 0;
	while (i < npc->path.bi)
	{
		render_ray3D(world->sdl, npc->path.path[i].enter_point,
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
	render_circle(world->sdl, npc->lastseen_playerpos, 6.0f, AMBER_2);
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
	gui_starthorizontal(gui);
	gui_label("NPC: ", gui);
	//gui_label(get_npc_type_name(npc->npc_type), gui);
	gui_endhorizontal(gui);
	gui_labeled_bool("ANIM:", entity->animation.active, gui);
	gui_labeled_int("i:", entity->animation.frame, gui);
}

static void	comp_npc_allocate(t_entity *entity, t_world *world)
{
	t_npc	*npc;

	entity->component.data = ft_memalloc(sizeof(t_npc));
	entity->component.data_size = sizeof(t_npc);
	npc = entity->component.data;
	bzero(entity->component.data, sizeof(t_npc));
	npc->movementspeed = 0.1f;
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
