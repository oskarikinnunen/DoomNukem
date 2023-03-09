#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void npc_player_raycast(t_entity *entity, t_npc *npc, t_world *world)
{
	t_entity	*player_ent;
	t_ray		r;

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
	destroy_entity(world, player_ent);
}

static void	npc_start_pushanim(t_entity *entity, t_npc *npc, t_world *world)
{
	npc->push_anim.framerate = 30;
	npc->push_anim.lastframe = 15;
	start_anim(&npc->push_anim, anim_forwards);
	start_human_anim(entity, "Walk_weapon", world);
	npc->next_action_time = world->clock.time
		+ game_random_range(world, 700, 4000);
}

static t_path	random_path(t_entity *entity, t_npc *npc, t_world *world)
{
	int			i;
	int			r;
	int			last_id;
	t_navnode	*curnode;
	t_path		result;

	curnode = &world->nav.navmesh[
		get_nearest_target_node(world, entity->transform.position)];
	i = 0;
	while (i < 20)
	{
		r = rand() % curnode->neighbors;
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
}

void npc_update_state(t_entity *entity,  t_npc *npc, t_world *world)
{
	npc_player_raycast(entity, npc, world);
	npc->prev_state = npc->state;
	//state thingies:
	if (!npc->seesplayer && !vector3_cmp(npc->lastseen_playerpos, vector3_zero()) && npc->state != NPC_STATE_CHASE)
	{
		npc->state = NPC_STATE_CHASE;
		npc->path = path_find(entity->transform.position, npc->lastseen_playerpos, world);
		printf("Found new path towards player, valid = %i \n", npc->path.valid_path);
	}
	if (npc->state == NPC_STATE_CHASE && !npc->path.valid_path)
	{
		npc->state = NPC_STATE_CAUTIOUS_MOVE;
		npc->next_action_time = world->clock.time + 3000;
		npc->lastseen_playerpos = vector3_zero();
		//start_human_anim(entity, "Walk_weapon", world);
		printf("GO TO CAUTION!\n");
	}
	if ((npc->seesplayer && npc->state != NPC_STATE_AGGRO && npc->state != NPC_STATE_AGGRO_PUSH)
		||
		(npc->seesplayer && npc->state == NPC_STATE_AGGRO_PUSH && !npc->push_anim.active))
	{
		npc->state = NPC_STATE_AGGRO;
		npc->next_action_time = world->clock.time + game_random_range(world, 700, 4000);
	}
	if (npc->state == NPC_STATE_AGGRO && world->clock.time > npc->next_action_time) 
	{
		npc_start_pushanim(entity, npc, world);
		npc->state = NPC_STATE_AGGRO_PUSH;
	}
	if (npc->state == NPC_STATE_CAUTIOUS_MOVE && world->clock.time > npc->next_action_time && !npc->path.valid_path)
	{
		npc->next_action_time = world->clock.time + 2000;
		npc->path = random_path(entity, npc, world);
		npc->orig_x_rotation = 0.0f;
	}
	if (npc->health <= 0 && npc->state != NPC_STATE_DEAD)
	{
		printf("going from %i to death state \n", npc->state);
		npc->state = NPC_STATE_DEAD;
		entity->animation.loop = false;
		entity->animation.persist = true;
	}
}