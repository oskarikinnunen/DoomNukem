#include "doomnukem.h"

void    civilian_movement(t_entity *entity, t_world *world)
{
	t_npc	*source;

	source = entity->component.data;
	if (source->path.start == source->path.end || !source->path.valid_path)
		source->path.end = game_random_range(world, 0, world->nav.node_amount - 1);
	if (source->path.ai == source->path.bi)
	{
		source->path.target = world->player->transform.position;
		//source->path.start = get_nearest_target_node(world, entity->transform.position);
		source->path.start = source->path.path[source->path.ai].index;
		//source->path.valid_path = pathfind(world, &source->path);
	}
	if (world->nav.show_navmesh)
	{
		uint32_t clr = world->sdl->render.gizmocolor;
		world->sdl->render.gizmocolor = CLR_RED;
		render_ray(world->sdl, entity->transform.position, source->path.path[source->path.ai].enter_point);
		for (int i = source->path.ai; i < source->path.bi; i++)
		{
			render_ray(world->sdl, source->path.path[i].enter_point, source->path.path[(i + 1)].enter_point);
			render_gizmo3d(world->sdl, source->path.path[i].enter_point, 10.0f, CLR_GREEN);
		}
		render_gizmo3d(world->sdl, world->nav.navmesh[source->path.end].mid_point, 10.0f, CLR_RED);
		world->sdl->render.gizmocolor = clr;
	}
	if (source->path.valid_path)
		entity->transform.position = vector3_lerp(entity->transform.position, source->path.path[source->path.ai].enter_point, 4.0f / vector3_dist(entity->transform.position, source->path.path[source->path.ai].enter_point));
	if (vector3_dist(entity->transform.position, source->path.path[source->path.ai].enter_point) < 5.0f && source->path.ai < source->path.bi && source->path.valid_path)
	{
		source->path.ai++;
	}
}
