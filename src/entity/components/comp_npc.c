#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"
#include "movement_defs.h"

static void npc_update_physics(t_npc *npc, t_entity *entity)
{
	npc->phys.position = &entity->transform.position;
	npc->phys.radius = 15.0f;
	npc->phys.gravity_override = NULL;
	npc->phys.height = PLAYER_HEIGHT;
	npc->phys.max_velocity = PLAYER_WALKSPEED;
}

//float fang_dist(t_float )

bool	npc_player_within_viewcone(t_entity *npc_ent, t_player *player)
{
	float	angle_between;

	angle_between = vector2_anglebetween(vector2_zero(),v3tov2(vector3_sub(npc_ent->transform.position, player->transform.position))) + RAD90;
	if (ft_absf(npc_ent->transform.rotation.x - angle_between) < RAD90 / 2.0f)
		return (true);
	return (false);
	//printf("npc ang %f anglebetween %f \n", npc_ent->transform.rotation.x, vector2_anglebetween(vector2_zero(),v3tov2(vector3_sub(npc_ent->transform.position, player->transform.position))) + RAD90);
	return (false);
}

#include <stdlib.h>
#include <time.h>

t_path	random_path(t_entity *entity, t_npc *npc, t_world *world)
{
	int			i;
	int			r;
	int			last_id;
	t_vector3	origin;
	t_navnode	*curnode;
	t_path		result;

	curnode = &world->nav.navmesh[get_nearest_target_node(world, entity->transform.position)];
	i = 0;
	while (i < 20)
	{
		r = rand() % curnode->neighbors;
		//curnode->
		while (curnode->neighbors_id[r] == last_id && curnode->neighbors > 2)
			r = rand() % curnode->neighbors;
		printf("node %i is %i\n", i, curnode->neighbors_id[r]);
		curnode = &world->nav.navmesh[curnode->neighbors_id[r]];
		last_id = curnode->index;
		i++;
	}
	result = path_find(entity->transform.position, curnode->mid_point, world);
	if (result.bi <= 1)
		result = random_path(entity, npc, world);
	return (result);
	//entity->
}

static inline void start_random_death_anim(t_entity *entity, t_world *world)
{
	uint32_t random = game_random_range(world, 1, 4);
	if (random == 1)
		start_human_anim(entity, "Death1", world);
	if (random == 2)
		start_human_anim(entity, "Death2", world);
	if (random == 3)
		start_human_anim(entity, "Death3", world);
	if (random == 4)
		start_human_anim(entity, "Death4", world);
}

static void npc_update_state(t_entity *entity,  t_npc *npc, t_world *world)
{
	t_entity	*player_ent;
	t_ray		r;

	//npc_raycast
	npc->prev_state = npc->state;
	player_ent = spawn_entity(world);
	player_ent->obj = get_object_by_name(*world->sdl, "Human.obj");
	player_ent->transform.position = world->player->transform.position;
	entity->ignore_raycasts = true;
	r.origin = vector3_add(entity->transform.position, (t_vector3){.z = 5.0f});
	r.dir = vector3_sub(world->player->headposition, r.origin);
	t_raycastinfo info;
	r.dir = vector3_mul(r.dir, 100.0f);
	npc->seesplayer = false;
	r.dir = vector3_normalise(r.dir);
	if (vector3_sqr_dist(world->player->transform.position, entity->transform.position) < 100000.0f && raycast_new(r, &info, world) && !world->player->noclip)
	{
		if (info.hit_entity == player_ent)
		{
			npc->seesplayer = true;
			npc->lastseen_playerpos = world->player->transform.position;
		}
	}
	entity->ignore_raycasts = false;
	//state thingies:
	if (!npc->seesplayer && !vector3_cmp(npc->lastseen_playerpos, vector3_zero()) && npc->state != NPC_STATE_CHASE)
	{
		npc->state = NPC_STATE_CHASE;
		npc->path = path_find(entity->transform.position, npc->lastseen_playerpos, world);
		printf("Found new path towards player, valid = %i \n", npc->path.valid_path);
		//start_human_anim(entity, "Walk_weapon", world);
	}
	if (npc->state == NPC_STATE_CHASE && !npc->path.valid_path)
	{
		npc->state = NPC_STATE_CAUTIOUS_MOVE;
		npc->next_action_time = world->clock.time + 3000;
		npc->lastseen_playerpos = vector3_zero();
		//start_human_anim(entity, "Walk_weapon", world);
		printf("GO TO CAUTION!\n");
	}
	if (npc->seesplayer && npc->state != NPC_STATE_AIM)
	{
		npc->state = NPC_STATE_AIM;
		//start_human_anim(entity, "Shoot_rifle", world);
		//update_anim(&entity->animation, world->clock.delta);
		//entity->animation.active = false;
	}
	if (npc->state == NPC_STATE_CAUTIOUS_MOVE && world->clock.time > npc->next_action_time && !npc->path.valid_path)
	{
		npc->next_action_time = world->clock.time + 2000;
		//if (rand() % 2 == 0)
		//{
		npc->path = random_path(entity, npc, world);
		//if (npc->path.)
		npc->orig_x_rotation = 0.0f;
		//start_human_anim(entity, "Walk_weapon", world);
		/*}
		else if (npc->orig_x_rotation == 0.0f)
		{
			if (rand() % 2 == 0)
			{
				start_human_anim(entity, "Shoot_rifle", world);
				entity->animation.lastframe = entity->animation.startframe;
			}
			else
				start_human_anim(entity, "Idle", world);
			npc->orig_x_rotation = entity->transform.rotation.x + RAD90; //ADD random
		}*/
	}
	destroy_entity(world, player_ent);
	if (npc->health <= 0 && npc->state != NPC_STATE_DEAD)
	{
		start_random_death_anim(entity, world);
		npc->state = NPC_STATE_DEAD;
		entity->animation.loop = false;
		entity->animation.persist = true;
	}
}

void npc_play_sound(t_entity *entity, t_world *world, char *soundname)
{
	t_npc			*npc;
	t_audiosource	source;
	if (entity->component.type != COMP_NPC)
		return;
	
	npc = entity->component.data;
	source = npc->audiosource;
	source.sample = get_sample(world->sdl, soundname);
	source._realrange = 250.0f;
	source.volume = 1.0f;
	source.play_always = false;
	_audiosource_start(world->sdl, &source, &entity->transform.position);
}

void npc_shoot(t_entity *entity, t_npc *npc, t_world *world)
{
	t_audiosource source = npc->audiosource;

	if (world->player->health == 0)
		return ;
	npc_play_sound(entity, world, "gun_machinegun.wav");
	if (npc->seesplayer)
	{
		uint32_t hitdice = game_random_range(world, 0, 10);
		uint32_t hitchance = 5;
		if (vector3_magnitude(world->player->cp.new_velocity) > 0.5f)
			hitchance = 2;
		if (hitdice < hitchance)
		{
			protagonist_play_audio(world->player, world, "protag_hurt.wav");
			world->player->health -= 10;
			world->player->lasthurttime = world->clock.time;
			world->player->lasthurtpos = entity->transform.position;
		}
		else
		{
			protagonist_play_audio(world->player, world, "ricochet.wav");
		}
	}
	npc->next_shoot_time = world->clock.time + game_random_range(world, 750, 2200);
}

static inline void	npc_update_pushanim(t_entity *entity, t_npc *npc, t_world *world)
{
	if (!npc->push_anim.active)
		return ;
	update_anim(&npc->push_anim, world->clock.delta);
	if (!npc->push_anim.active) //Should change state to NPC_STATE_AIM
		start_human_anim(entity, "Shoot_rifle", world);
	t_vector2 move;
	move = v3tov2(vector3_sub(world->player->transform.position, entity->transform.position));
	capsule_add_xy_velocity(move, &npc->phys, world);
}

//"animation" for when the npc starts moving towards player, while the npc is in state NPC_STATE_AIM
static void npc_start_pushanim(t_entity *entity, t_npc *npc, t_world *world)
{
	npc->push_anim.framerate = 30;
	npc->push_anim.lastframe = 15;
	start_anim(&npc->push_anim, anim_forwards);
	start_human_anim(entity, "Walk_weapon", world);
	npc->next_action_time = world->clock.time + game_random_range(world, 700, 4000);
}

static void entity_rotatetowards_xy(t_entity *entity, t_vector2 pos, float delta)
{
	float	angle;

	angle = vector2_anglebetween(vector2_zero(),vector2_sub(v3tov2(entity->transform.position), pos));
	entity->transform.rotation.x = ft_fmovetowards(entity->transform.rotation.x, angle + RAD90, delta);
}

static void	entity_rotatetowards_player(t_entity *entity, float speed, t_world *world)
{
	float	angle;

	angle = vector2_anglebetween(vector2_zero(),v3tov2(vector3_sub(entity->transform.position, world->player->transform.position)));
	entity->transform.rotation.x = ft_fmovetowards(entity->transform.rotation.x, angle + RAD90, world->clock.delta * speed);
}

static void npc_pathfind_step(t_entity *entity, t_npc *npc, t_world *world)
{
	t_vector2	enterpoint_2d;

	if (!npc->path.valid_path/* || (npc->state != NPC_STATE_CHASE && npc->state != NPC_STATE_CAUTIOUS_MOVE)*/)
		return ;
	enterpoint_2d = v3tov2(npc->path.path[npc->path.ai].enter_point);
	t_vector2 move = vector2_sub(enterpoint_2d, v3tov2(entity->transform.position));
	move = vector2_mul(vector2_normalise(move), world->clock.delta * PLAYER_ACCELERATION);
	capsule_add_xy_velocity(move, &npc->phys, world);
	entity_rotatetowards_xy(entity, enterpoint_2d, 0.015f * world->clock.delta);
	if (vector3_sqr_dist(entity->transform.position, npc->path.path[npc->path.ai].enter_point) < 160.1f && npc->path.ai < npc->path.bi)
		npc->path.ai++;
	if (npc->path.ai >= npc->path.bi && npc->path.valid_path)
		npc->path.valid_path = false;
}

static void npc_update_anim_state(t_entity *entity, t_npc *npc, t_world *world)
{
	bool	changed;

	changed = npc->prev_state != npc->state;
	if (!changed)
		return ;
	if (npc->state == NPC_STATE_AIM)
	{
		start_human_anim(entity, "Shoot_rifle", world);
		update_anim(&entity->animation, world->clock.delta);
	}
	//if (vector2_magnitude(v3tov2(npc->phys.new_velocity) > 0.5f)
	if (npc->state == NPC_STATE_CHASE || npc->state == NPC_STATE_CAUTIOUS_MOVE)
		start_human_anim(entity, "Walk_weapon", world);
	//if (npc->state == NPC_STATE_CHASE || npc->state == NPC_STATE_CAUTIOUS_MOVE)
}

static void	comp_npc_update(t_entity *entity, t_world *world)
{
	t_npc	*npc;

	npc = entity->component.data;
	if (npc == NULL || world->gamemode == MODE_EDITOR || world->player->health == 0)
		return;
	if (entity->animation.active)
		update_anim(&entity->animation, world->clock.delta);
	if (npc->state != NPC_STATE_DEAD)
	{
		npc_update_state(entity, npc, world);
		if (npc->state == NPC_STATE_AIM)
		{
			if (world->clock.time > npc->next_shoot_time)
				npc_shoot(entity, npc, world);
			entity_rotatetowards_xy(entity, v3tov2(world->player->transform.position), 0.015f * world->clock.delta);
			if (world->clock.time > npc->next_action_time) //Move this to update_state, and 
				npc_start_pushanim(entity, npc, world);
			npc_update_pushanim(entity, npc, world);
		}
		if (npc->state == NPC_STATE_CAUTIOUS_MOVE && npc->orig_x_rotation != 0.0f)
			entity->transform.rotation.x = ft_fmovetowards(entity->transform.rotation.x, npc->orig_x_rotation + (sinf(world->clock.time * 0.0012f) * RAD90) - (RAD90 / 2.0f), world->clock.delta * 0.015f);
		npc_pathfind_step(entity, npc, world);
	}
	npc_update_physics(npc, entity);
	capsule_applygravity_new(&npc->phys, world);
	capsule_damp(&npc->phys, world);
	npc_update_anim_state(entity, npc, world);
}

static char *get_npc_type_name(int i)
{
	//Might be against norm to use enum to stack mem
	static char npc_types[nt_count][64] = \
	{
		"Default",
		"Civilian",
		"Enemy"
	};
	if (i < nt_count)
		return(npc_types[i]);
	return(NULL);
}

static void	comp_npc_ui_update(t_entity *entity, t_world *world) 
{
	t_npc			*npc;

	if (world->gamemode == MODE_EDITOR || 1)
		return ;
	npc = entity->component.data;
	//if (world->nav.show_navmesh)
	//{
	uint32_t clr = world->sdl->render.gizmocolor;
	world->sdl->render.gizmocolor = CLR_RED;
	render_ray(world->sdl, entity->transform.position, npc->path.path[npc->path.ai].enter_point);
	for (int i = npc->path.ai; i < npc->path.bi; i++)
	{
		render_ray(world->sdl, npc->path.path[i].enter_point, npc->path.path[(i + 1)].enter_point);
		render_circle(world->sdl, npc->path.path[i].enter_point, 10.0f, CLR_GREEN);
	}
	int i = 0;
	while (i < world->nav.node_amount)
	{
		if (!world->nav.navmesh[i].blocked)
		{
			float fdist = vector3_sqr_dist(entity->transform.position, world->nav.navmesh[i].mid_point);
			if (fdist < 60000.0f) 
			{
				world->sdl->render.gizmocolor = AMBER_1;
				if (fdist < 30000.0f)
					world->sdl->render.gizmocolor = AMBER_3;
				int j = 0;
				while (j < 3)
				{
					render_ray(world->sdl, world->nav.navmesh[i].vertex[j], world->nav.navmesh[i].vertex[(j + 1) % 3]);
					j++;
				}
			}
		}
		i++;
	}
	render_circle(world->sdl, world->nav.navmesh[npc->path.end].mid_point, 10.0f, CLR_RED);
	world->sdl->render.gizmocolor = clr;
	render_circle(world->sdl, npc->lastseen_playerpos, 6.0f, AMBER_2);
}

static void	comp_npc_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_npc			*npc;
	static bool		toggle_select;
	npc = (t_npc *)entity->component.data;
	
	if (npc == NULL)
	{
		gui_label("!NULL source", gui);
		return;
	}
	gui_starthorizontal(gui);
	gui_label("NPC: ", gui);
	gui_label(get_npc_type_name(npc->npc_type), gui);
	gui_endhorizontal(gui);
	gui_labeled_bool("ANIM:", entity->animation.active, gui);
	gui_labeled_int("i:", entity->animation.frame, gui);
}

static void	comp_npc_allocate(t_entity *entity, t_world *world)
{
	t_npc *npc;

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
