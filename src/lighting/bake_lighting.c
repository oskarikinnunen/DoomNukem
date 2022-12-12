#include "doomnukem.h"

void bake_lighting(t_render *render, t_world *world)
{
	int			i;
	int			found;
	t_entity	*ent;

	bzero(world->sdl->zbuffer, sizeof(uint32_t) * world->sdl->window_h * world->sdl->window_w);
	render->camera.lookdir = (t_vector3){-0.630105, 0.650868, -0.423484};
	render->camera.position = (t_vector3){1208.355713, 912.929382, 154.534698};
	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount && 1)
	{
		ent = &world->entitycache.entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active)
			{
				render_entity_depth_buffer(*world->sdl, render, ent);
			}
			found++;
		}
		i++;
	}
	//render_entity_depth_buffer(*world->sdl, render, &world->skybox);
	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount && 1)
	{
		ent = &world->entitycache.entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active)
			{
				update_arealights_for_entity(*world->sdl, render, ent);
			}
			found++;
		}
		i++;
	}
	//world->skybox.map->img = NULL;
//	update_arealights_for_entity(*world->sdl, render, &world->skybox);
	printf("clean exit\n");
	//exit(0);
}