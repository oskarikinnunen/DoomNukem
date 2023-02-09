#include "doomnukem.h"

void    civilian_movement(t_entity *entity, t_world *world)
{
	t_npc	*source;

	source = entity->component.data;
	//if (source->path.start == source->path.end || !source->path.valid_path)
	//	source->path.end = game_random_range(world, 0, world->nav.node_amount - 1);
	if (source->aggroed)
	{
		source->path = path_find(entity->transform.position, world->player->transform.position, world);
		printf("path valid = %i \n", source->path.valid_path);
		source->aggroed = false;
	}
	if (world->nav.show_navmesh)
	{
		uint32_t clr = world->sdl->render.gizmocolor;
		world->sdl->render.gizmocolor = CLR_RED;
		render_ray(world->sdl, entity->transform.position, source->path.path[source->path.ai].enter_point);
		for (int i = source->path.ai; i < source->path.bi; i++)
		{
			render_ray(world->sdl, source->path.path[i].enter_point, source->path.path[(i + 1)].enter_point);
			render_circle(world->sdl, source->path.path[i].enter_point, 10.0f, CLR_GREEN);
		}
		render_circle(world->sdl, world->nav.navmesh[source->path.end].mid_point, 10.0f, CLR_RED);
		world->sdl->render.gizmocolor = clr;
	}
	//angle_between
	float fang = vector2_anglebetween(vector2_zero(),v3tov2(vector3_sub(entity->transform.position, source->path.path[source->path.ai].enter_point)));
	entity->transform.rotation.x = fang + RAD90;
	if (source->path.valid_path && source->path.ai < source->path.bi)
		entity->transform.position = vector3_lerp(entity->transform.position, source->path.path[source->path.ai].enter_point, 4.0f / vector3_dist(entity->transform.position, source->path.path[source->path.ai].enter_point));
	if (vector3_dist(entity->transform.position, source->path.path[source->path.ai].enter_point) < 5.0f && source->path.ai < source->path.bi && source->path.valid_path)
	{
		source->path.ai++;
	}
}
