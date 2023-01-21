#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static uint32_t get_target_node(t_world *world, t_vector3 target)
{
	int e = 0;
	int closest_point = 0;
	float dist;
	dist = vector3_dist(world->navmesh[0].mid_point, target);
	while (e < world->node_amount)
	{
		if (dist > vector3_dist(world->navmesh[e].mid_point, target))
		{
			dist = vector3_dist(world->navmesh[e].mid_point, target);
			closest_point = e;
		}
		e++;
	}
	return(closest_point);
}
//remember audio.sounds.c has a return remove it.	
static void	comp_npc_update(t_entity *entity, t_world *world)
{
	t_npc			*source;

	//destroy_entity(world, entity);
	//return;
	source = entity->component.data;
	if (source == NULL)
		return;
	printf("ai %d, b1 %d\n", source->path.ai, source->path.bi);
//	destroy_entity(world, entity);
	if (source->path.ai == source->path.bi)
	{
		source->path.target = world->player->transform.position;
		source->path.start = get_target_node(world, entity->transform.position);
		pathfind(world, &source->path);
	}
	for (int i = source->path.ai; i < source->path.bi; i++)
	{
		render_gizmo3d(world->sdl, source->path.path[i].mid_point, 10.0f, CLR_PRPL);
		//render_gizmo3d(world->sdl, source->path.path[i].enter_point, 10.0f, CLR_GREEN);
	}
	render_gizmo3d(world->sdl, source->path.path[source->path.bi].mid_point, 10.0f, CLR_RED);
//	entity->transform.position = vector3_movetowards(entity->transform.position, source->path.path[source->path.ai].mid_point, world->clock.delta * source->movementspeed);
//	printf("%d %d.  ", source->path.ai, source->path.bi);
	print_vector3(source->path.path[source->path.ai].mid_point);
	print_vector3(entity->transform.position);
	entity->transform.position = vector3_lerp(entity->transform.position, source->path.path[source->path.ai].mid_point, 5.0f / vector3_dist(entity->transform.position, source->path.path[source->path.ai].mid_point));
	printf("%f dist\n", vector3_dist(entity->transform.position, source->path.path[source->path.ai].mid_point));
	if (vector3_dist(entity->transform.position, source->path.path[source->path.ai].mid_point) < 50.0f && source->path.ai < source->path.bi)
	{
		printf("true\n");
		source->path.start = source->path.path[source->path.ai].parent; // prob doesnt work
		source->path.ai++;
	}
}

static void	comp_npc_allocate(t_entity *entity, t_world *world)
{
	t_npc *source;

	entity->component.data = ft_memalloc(sizeof(t_npc));
	entity->component.data_size = sizeof(t_npc);
	source = entity->component.data;
	bzero(entity->component.data, sizeof(t_npc));
	source->movementspeed = 0.1f;
	source->path.start = get_target_node(world, entity->transform.position);
}

void	assign_component_npc(t_component *component)
{
	component->data_size = sizeof(t_npc);
	component->func_allocate = comp_npc_allocate;
	component->func_update = comp_npc_update;
}
