#include "doomnukem.h"

void calculate_pointlight_step(t_pointlight *pointlight, t_world *world, t_render *render)
{
    int					i = 0;
    int					found = 0;
    int					index = 0;
	int					j;
	t_lighting			l;
	t_entity			*ent;
	t_triangle_polygon	test;
	uint32_t			timestart;
	uint32_t			timecur;

	l = world->lighting;
	l.pointlight = pointlight;
    l.triangles = malloc(sizeof(t_triangle_polygon *) * world->entitycache.existing_entitycount);
    l.entities = malloc(sizeof(t_entity *) * world->entitycache.existing_entitycount);
    while (found < world->entitycache.existing_entitycount)
    {
        ent = &world->entitycache.entities[i];
        if (ent->status != es_free)
        {
            if (ent->status == es_active && ent->obj->uv_count != 0)
            {
				l.triangles[index] = malloc(sizeof(t_triangle_polygon) * ent->obj->face_count);
				l.entities[index] = ent;
				j = 0;
				while (j < ent->obj->face_count)
				{
					for (int e = 0; e < 3; e++)
					{
						t_quaternion temp;
						temp.v = ent->obj->vertices[ent->obj->faces[j].v_indices[e] - 1];
						temp = transformed_vector3(ent->transform, temp.v);
						temp = quaternion_mul_matrix(render->camera.matworld, temp);
						l.triangles[index][j].p3[e] = temp.v;
						l.triangles[index][j].uv[e] = ent->obj->uvs[ent->obj->faces[j].uv_indices[e] - 1];
					}
					j++;
				}
				index++;
            }
            found++;
        }
        i++;
    }
	l.entities_count = index;
	i = 0;
	timestart = SDL_GetTicks();
	timecur = timestart;
	while (i < l.entities_count)
	{
		ent = l.entities[i];
		j = ent->lightmap->progress;
		while (timecur - timestart < 5 && j < ent->obj->face_count)
		{
			if (j == ent->lightmap->progress/* || ent->obj->faces[j].materialindex != ent->obj->faces[j - 1].materialindex*/)
			{
				l.lightmap = &ent->lightmap[ent->obj->faces[j].materialindex];
				/*l.drawnbuff = malloc(sizeof(bool) * l.lightmap->size.x * l.lightmap->size.y);
				bzero(l.drawnbuff, sizeof(bool) * l.lightmap->size.x * l.lightmap->size.y);*/
			}
			for (int e = 0; e < 3; e++)
			{
				test.p2[e].x =
					l.triangles[i][j].uv[e].x
					* (ent->obj->materials
						[ent->obj->faces[j].materialindex].img->size.x - 1);
				test.p2[e].y = l.triangles[i][j].uv[e].y * (ent->obj->materials[ent->obj->faces[j].materialindex].img->size.y - 1);

				test.uv[e].x = test.p2[e].x / (float)l.lightmap->size.x;
				test.uv[e].y = test.p2[e].y / (float)l.lightmap->size.y;
			}
			memcpy(test.p3, l.triangles[i][j].p3, sizeof(t_vector3) * 3);
			l.entity_id = ent->id;
			render_triangle_uv(l, test);
			/*if (j + 1 == ent->obj->face_count || ent->obj->faces[j].materialindex != ent->obj->faces[j + 1].materialindex)
				free(l.drawnbuff);*/
			j++;
			ent->lightmap->progress = j;
			printf("tri %i \n", j);
			timecur = SDL_GetTicks();
		}
		if (ent->lightmap->progress == ent->obj->face_count)
		{
			ent->lightmap->done = true;
			//printf("entity %i lightmap done, did %i triangles\n", j, ent->lightmap->progress + 1);
			create_map_for_entity(ent, world);
		}
		i++;
		//timecur = SDL_GetTicks();
	}
	i = 0;
	bool	done = true;
	while (i < l.entities_count)
	{
		if (!l.entities[i]->lightmap->done)
			done = false;
		free(l.triangles[i]);
		i++;
	}
	pointlight->done = done;
	free(l.triangles);
	free(l.entities);

}

void calculate_pointlight(t_pointlight *pointlight, t_world *world, t_render *render)
{
    int					i = 0;
    int					found = 0;
    int					index = 0;
	int					j;
	t_entity			*ent;
	t_triangle_polygon	test;

	t_lighting l = world->lighting;
	l.pointlight = pointlight;
    l.triangles = malloc(sizeof(t_triangle_polygon *) * world->entitycache.existing_entitycount);
    l.entities = malloc(sizeof(t_entity *) * world->entitycache.existing_entitycount);
    while (found < world->entitycache.existing_entitycount)
    {
        ent = &world->entitycache.entities[i];
        if (ent->status != es_free)
        {
            if (ent->status == es_active && ent->obj->uv_count != 0)
            {
				l.triangles[index] = malloc(sizeof(t_triangle_polygon) * ent->obj->face_count);
				l.entities[index] = ent;
				j = 0;
				while (j < ent->obj->face_count)
				{
					for (int e = 0; e < 3; e++)
					{
						t_quaternion temp;
						temp.v = ent->obj->vertices[ent->obj->faces[j].v_indices[e] - 1];
						temp = transformed_vector3(ent->transform, temp.v);
						temp = quaternion_mul_matrix(render->camera.matworld, temp);
						l.triangles[index][j].p3[e] = temp.v;
						l.triangles[index][j].uv[e] = ent->obj->uvs[ent->obj->faces[j].uv_indices[e] - 1];
					}
					j++;
				}
				index++;
            }
            found++;
        }
        i++;
    }
	l.entities_count = index;
	i = 0;
	while (i < index)
	{
		ent = l.entities[i];
		j = 0;
		while (j < ent->obj->face_count)
		{
			if (j == 0 || ent->obj->faces[j].materialindex != ent->obj->faces[j - 1].materialindex)
			{
				l.lightmap = &ent->lightmap[ent->obj->faces[j].materialindex];
				//l.drawnbuff = malloc(sizeof(bool) * l.lightmap->size.x * l.lightmap->size.y);
				//bzero(l.drawnbuff, sizeof(bool) * l.lightmap->size.x * l.lightmap->size.y);
			}
			for (int e = 0; e < 3; e++)
			{
				test.p2[e].x = l.triangles[i][j].uv[e].x * (ent->obj->materials[ent->obj->faces[j].materialindex].img->size.x - 1);
				test.p2[e].y = l.triangles[i][j].uv[e].y * (ent->obj->materials[ent->obj->faces[j].materialindex].img->size.y - 1);

				test.uv[e].x = test.p2[e].x / (float)l.lightmap->size.x;
				test.uv[e].y = test.p2[e].y / (float)l.lightmap->size.y;
			}
			memcpy(test.p3, l.triangles[i][j].p3, sizeof(t_vector3) * 3);
			render_triangle_uv(l, test);
			/*if (j + 1 == ent->obj->face_count || ent->obj->faces[j].materialindex != ent->obj->faces[j + 1].materialindex)
				free(l.drawnbuff);*/
			j++;
		}
		i++;
	}
	i = 0;
	while (i < index)
	{
		free(l.triangles[i]);
		i++;
	}
	free(l.triangles);
	free(l.entities);

}
