#include "doomnukem.h"

void	print_node(t_navnode n)
{
	printf("midpoint ");
	print_vector3(n.mid_point);
	printf("amount of neighbors %d\n", n.neighbors);
	printf("list of neighbors\n");
	for (int i = 0; i < n.neighbors; i++)
	{
		printf("%d\n", n.neighbors_id[i]);
	}

}

void    show_navmesh(t_world *world)
{
    uint32_t	clr = world->sdl->render.gizmocolor;
	world->sdl->render.gizmocolor = CLR_RED;
	for (int i = 0; i < world->node_amount; i++)
	{
		render_gizmo3d(world->sdl, world->navmesh[i].mid_point, 10.0f, CLR_BLUE);
		for (int e = 0; e < world->navmesh[i].neighbors; e++)
		{
		//	render_gizmo3d(world->sdl, world->navmesh[i].line_point[e], 10.0f, CLR_TURQ);
			world->sdl->render.gizmocolor = CLR_TURQ;
			//render_ray(world->sdl, world->navmesh[i].mid_point, world->navmesh[i].line_point[e]);
			world->sdl->render.gizmocolor = CLR_PRPL;
           // printf(" i %d e %d %d\n", i, e, world->navmesh[i].neighbors_id[e]);
			render_ray(world->sdl, world->navmesh[i].mid_point, world->navmesh[world->navmesh[i].neighbors_id[e]].mid_point);
		}
		for (int j = 0; j < 3; j++)
		{
			world->sdl->render.gizmocolor = CLR_RED;
			render_ray(world->sdl, world->navmesh[i].vertex[j], world->navmesh[i].vertex[(j + 1) % 3]);
		}
	}
	world->sdl->render.gizmocolor = clr;
}

void	create_navmesh(t_world *world)
{
	int			i;
	int			found;
	t_entity	*ent;
    t_triangle  arr[1000];

	
	bzero(world->navmesh, sizeof(t_navnode) * 1000);
	i = 0;
	found = 0;
	world->node_amount = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
			{
				if (ent->obj->bounds.type == bt_ignore)
				{
                    int start = 0;
                    int end = 1;
                    arr[0].p[0].v = ent->obj->vertices[0];
                    arr[0].p[1].v = ent->obj->vertices[1];
                    arr[0].p[2].v = ent->obj->vertices[2];
                    while(start < end)
                    {
                        float dist;
                        int index = 0;
                        dist = vector3_dist(arr[start].p[0].v, arr[start].p[1].v);
                        for (int j = 1; j < 3; j++)
                        {
                            if (dist < vector3_dist(arr[start].p[j].v, arr[start].p[(j + 1) % 3].v))
                            {
                                dist = vector3_dist(arr[start].p[j].v, arr[start].p[(j + 1) % 3].v);
                                index = j;
                            }
                        }
                        if (dist < 250.0f)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                 world->navmesh[world->node_amount].vertex[j] = arr[start].p[j].v;
                            }
                           // printf("node amount %d\n", world->node_amount);
                            world->navmesh[world->node_amount].mid_point = vector3_div(vector3_add(vector3_add(world->navmesh[world->node_amount].vertex[0], world->navmesh[world->node_amount].vertex[1]), world->navmesh[world->node_amount].vertex[2]), 3.0f);
                            world->node_amount++;
                        }
                        else
                        {
                            t_vector3 plane_p = vector3_lerp(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v, 0.5f);
                            t_vector3 plane_n = vector3_normalise(vector3_sub(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v));
                        
                            end += clip_triangle_against_plane(plane_p, plane_n, arr[start], &arr[end]);
                            plane_n = vector3_negative(plane_n);
                            end += clip_triangle_against_plane(plane_p, plane_n, arr[start], &arr[end]);
                        }
                        start++;
                    }
                    printf("end is %d\n", end);
				}
			}
			found++;
		}
		i++;
	}
    if (world->node_amount > 1000)
    {
        printf("too many nodes\n");
        exit(0);
    }
    printf("%d\n", world->node_amount);
    i = 0;
	while (i < world->node_amount)
	{
		//setup cell
		world->navmesh[i].neighbors = 0;
		for (int j = 0; j < 3; j++)
		{
			t_vector3 start = world->navmesh[i].vertex[j];
			t_vector3 end = world->navmesh[i].vertex[(j + 1) % 3];
			t_vector3 normal = vector3_normalise(vector3_sub(start, end));
			float temp_d = vector3_dist(start, end);
			for (int i1 = 0; i1 < world->node_amount; i1++)
			{
				if (i == i1)
					continue;
				int count = 0;
				for (int j1 = 0; j1 < 3; j1++)
				{
					 float point_dist[2];
					point_dist[0] = vector3_dist(start, world->navmesh[i1].vertex[j1]);
					point_dist[1] = vector3_dist(end, world->navmesh[i1].vertex[j1]);
					if (point_dist[0] + point_dist[1] <= temp_d + 5.0f)
					{
						count++;
					}
				}
				if (count >= 2)
				{
					world->navmesh[i].neighbors_id[world->navmesh[i].neighbors++] = i1;
				}
			}
		}
        i++;
	}
	 i = 0;
	while (i < world->node_amount)
	{
		for (int j = 0; j < world->navmesh[i].neighbors; j++)
		{
			int id_check = -1;
			for (int j1 = 0; j1 < world->navmesh[world->navmesh[i].neighbors_id[j]].neighbors; j1++)
			{
				if (world->navmesh[world->navmesh[i].neighbors_id[j]].neighbors_id[j1] == i)
				{
					id_check == i;
				}
			}
			if (id_check != i)
			{
				world->navmesh[world->navmesh[i].neighbors_id[j]].neighbors_id[world->navmesh[world->navmesh[i].neighbors_id[j]].neighbors++] = i;
			}
		}
		i++;
	}
}

/*
int node_count = world->node_amount;
	for (int i = 0; i < world->node_amount && 0; i++)
	{
		t_navnode temp;
		t_triangle test;
		temp = world->navmesh[i];
		float dist;
		dist = vector3_dist(world->navmesh[i].vertex[0], world->navmesh[i].vertex[1]);
		int index = 0;
		for (int j = 1; j < 3; j++)
		{
			if (dist < vector3_dist(world->navmesh[i].vertex[j], world->navmesh[i].vertex[(j + 1) % 3]))
			{
				dist = vector3_dist(world->navmesh[i].vertex[j], world->navmesh[i].vertex[(j + 1) % 3]);
				index = j;
			}
		}
		for (int j = 0; j < 3; j++)
		{
			test.p[j].v = world->navmesh[i].vertex[j];
		}
		t_vector3 plane_p = vector3_lerp(world->navmesh[i].vertex[index], world->navmesh[i].vertex[(index + 1) % 3], 0.5f);
		t_vector3 plane_n = vector3_normalise(vector3_sub(world->navmesh[i].vertex[index], world->navmesh[i].vertex[(index + 1) % 3]));
	
		t_triangle out[4];
		int count = 0;
		count = clip_triangle_against_plane(plane_p, plane_n, test, out);
		plane_n = vector3_negative(plane_n);
		count += clip_triangle_against_plane(plane_p, plane_n, test, &out[count]);
		for (int i1 = 0; i1 < count; i1++)
		{
			world->navmesh[node_count].neighbors = 0;
			for (int i2 = 0; i2 < count; i2++)
			{
				if (i2 == i1)
					continue;
				world->navmesh[node_count].neighbors_id[world->navmesh[node_count].neighbors++] = node_count + i2;
			}
			for (int j = 0; j < temp.neighbors; j++)
			{
				world->navmesh[node_count].neighbors_id[world->navmesh[node_count].neighbors++] = temp.neighbors_id[j];
			}
			for (int j = 0; j < 3; j++)
			{
				world->navmesh[node_count].vertex[j] = out[i1].p[j].v;
			}
			world->navmesh[i].neighbors_id[world->navmesh[i].neighbors++] = node_count;
			world->navmesh[node_count].mid_point = vector3_div(vector3_add(vector3_add(world->navmesh[node_count].vertex[0], world->navmesh[node_count].vertex[1]), world->navmesh[node_count].vertex[2]), 3.0f);
			node_count++;
		}
		//triangle_clipagainstplane(plane_p, plane_n, test, out);
	}
	world->node_amount = node_count;
    */