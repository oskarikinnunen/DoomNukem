#include "doomnukem.h"

bool is_entity_frustrum_culled(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	calculate_triangles(sdl, render, entity);
	set_square_from_triangles(&entity->occlusion, render->screenspace_ptris, render->screenspace_ptri_count);
	if (render->screenspace_ptri_count > 0)
		return(false);
	return(true);
}

void update_frustrum_culling(struct s_world *world, t_sdlcontext *sdl, t_render *render)
{
	int			i;
	int			found;
	t_entity	*ent;

	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
				ent->occlusion.is_occluded = is_entity_frustrum_culled(*sdl, render, ent);
			found++;
		}
		i++;
	}
}