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
		for (int j = 0; j < 3; j++)
		{
		//	world->sdl->render.gizmocolor = CLR_GREEN;
			//render_ray(world->sdl, world->navmesh[i].vertex[j], world->navmesh[i].vertex[(j + 1) % 3]);
		}
		for (int e = 0; e < world->navmesh[i].neighbors; e++)
		{
			render_gizmo3d(world->sdl, world->navmesh[i].line_point[e], 10.0f, CLR_BLUE);
		}
	}
	world->sdl->render.gizmocolor = clr;
}

int	line_laps_line(t_vector3 start1, t_vector3 end1, t_vector3 p)
{
	float len;
	float dist[2];

	if (vector3_cmp_epsilon(p, start1, 0.001f) || vector3_cmp_epsilon(p, end1, 0.001f))
		return(1);
	len = vector3_dist(start1, end1);
	dist[0] = vector3_dist(p, start1);
	dist[1] = vector3_dist(p, end1);
	if (dist[0] + dist[1] <= len + 0.0001f) //0.00001f
		return(2);
	return(0);
}

bool line_overlaps_parallel_line(t_vector3 start1, t_vector3 end1, t_vector3 start2, t_vector3 end2)
{
	int count = 0;
	count += line_laps_line(start1, end1, start2);
	count += line_laps_line(start1, end1, end2);
	count += line_laps_line(start2, end2, start1);
	count += line_laps_line(start2, end2, end1);
	if (count >= 3)
		return(true);
	return(false);
}

bool	is_triangle_degenerate(t_vector3 *p)
{
	int		i;
	int		index;
	float	dist;

	index = 0;
	i = 1;
	dist = vector3_dist(p[0], p[1]);
	while (i < 3)
	{
		if (dist < vector3_dist(p[i], p[(i + 1) % 3]))
		{
			index = i;
			dist = vector3_dist(p[i], p[(i + 1) % 3]);
		}
		i++;
	}
	if (vector3_dist(p[(index + 2) % 3], p[index]) + vector3_dist(p[(index + 2) % 3], p[(index + 1) % 3]) <= dist + 0.0001f)
		return(true);
	return(false);
}

void	create_navmesh(t_world *world)
{
	int			i;
	int			found;
	t_entity	*ent;
    t_triangle  arr[1000];

	
	bzero(world->navmesh, sizeof(t_navnode) * 1000);
	bzero(arr, sizeof(t_triangle) * 1000);
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

							if (1 || dist + 1.0f < vector3_dist(world->navmesh[world->node_amount].vertex[(index + 2) % 3], world->navmesh[world->node_amount].vertex[index]) + vector3_dist(world->navmesh[world->node_amount].vertex[(index + 2) % 3], world->navmesh[world->node_amount].vertex[(index + 1) % 3]))
							{
								if (!is_triangle_degenerate(world->navmesh[world->node_amount].vertex))
								{
									world->navmesh[world->node_amount].mid_point = vector3_div(vector3_add(vector3_add(world->navmesh[world->node_amount].vertex[0], world->navmesh[world->node_amount].vertex[1]), world->navmesh[world->node_amount].vertex[2]), 3.0f);
                           			world->node_amount++;
								}
							}
                        }
                        else
                        {
                            t_vector3 plane_p = vector3_lerp(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v, 0.5f);
                            t_vector3 plane_n = vector3_normalise(vector3_sub(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v));
                        
                            end += clip_triangle_against_plane(plane_p, plane_n, arr[start], &(arr[end]));
                            plane_n = vector3_negative(plane_n);
                            end += clip_triangle_against_plane(plane_p, plane_n, arr[start], &(arr[end]));
                        }
                        start++;
                    }
				}
			}
			found++;
		}
		i++;
	}
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
			float temp_d = roundf(vector3_dist(start, end));
			for (int i1 = 0; i1 < world->node_amount; i1++)
			{
				bool	skip = false;
				if (i == i1)
					continue;
				for (int i2 = 0; i2 < world->navmesh[i].neighbors; i2++)
				{
					if (world->navmesh[i].neighbors_id[i2] == i1)
						skip = true;
				}
				if (skip)
					continue;
				skip = false;
				for (int j1 = 0; j1 < 3; j1++)
				{
					if (line_overlaps_parallel_line(start, end, world->navmesh[i1].vertex[j1], world->navmesh[i1].vertex[(j1 + 1) % 3]))
						skip = true;
				}
				if (skip == true)
				{
					world->navmesh[i].line_point[world->navmesh[i].neighbors] = vector3_lerp(start, end, 0.5f);
					world->navmesh[i].neighbors_id[world->navmesh[i].neighbors++] = i1;
				}
			}
		}
		printf("neighbor amount %d %d\n", world->navmesh[i].neighbors, i);
		for (int test3 = 0; test3 < world->navmesh[i].neighbors; test3++)
		{
			printf("%d\n", world->navmesh[i].neighbors_id[test3]);
		}
		printf("\n");
	//	print_vector3(world->navmesh[i].mid_point);
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