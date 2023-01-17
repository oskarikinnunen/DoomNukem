#include "doomnukem.h"

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

bool pathfind(t_world *world, t_path *path)
{
	t_navnode	openlist[1000];
	uint32_t	o_amount;
	uint32_t	lowest_f;
	int	i;

	//exit(0);
	bzero(openlist, sizeof(t_navnode) * 1000);
	path->end = get_target_node(world, path->target);
	ft_swap(&path->start, &path->end, sizeof(uint32_t));
	openlist[path->start] = world->navmesh[path->start];
	openlist[path->start].valid = true;
	printf("start %d end %d\n", path->start, path->end);
	o_amount = 1;
	while (o_amount > 0)
	{
		uint32_t found = 0;
		i = 0;
		while (found < o_amount)
		{
			if (openlist[i].valid == true)
			{
				if (found == 0 || openlist[i].f < openlist[lowest_f].f)
					lowest_f = i;
				found++;
			}
			i++;
			if (i > 999)
			{
				printf("buffer overflow pathfind, should never happen\n");
				exit(0);
			}
		}
		if (lowest_f == path->end)
		{
			int e, i;

			i = 0;
			e = path->end;
			path->path[i] = openlist[e];
			while (e != path->start && i < 32)
			{
				print_vector3(openlist[e].mid_point);
				printf("%d\n", i);
				path->path[i] = openlist[e];
				e = openlist[e].parent;
				i++;
			}
			path->path[i] = openlist[e];
			print_vector3(openlist[e].mid_point);
			printf("%d\n", i);
			//exit(0);
			path->ai = 0;
			path->bi = i;
			return(true);
		}
		openlist[lowest_f].valid = false;
		openlist[lowest_f].visited = true;
		o_amount--;
		i = 0;
		while (i < openlist[lowest_f].neighbors)
		{
			uint32_t id;
			id = openlist[lowest_f].neighbors_id[i];
			if (openlist[id].visited == false && openlist[id].blocked == false)
			{
				openlist[id] = world->navmesh[id];
				openlist[id].g = vector3_dist(openlist[id].mid_point, world->navmesh[path->start].mid_point); // should be amount of parents to start
				openlist[id].h = vector3_dist(openlist[id].mid_point, world->navmesh[path->end].mid_point);
				openlist[id].f = openlist[id].g + openlist[id].h;
				openlist[id].valid = true;
				openlist[id].parent = lowest_f;
				o_amount++;
			}
			i++;
		}
		//printf("o amount %d %d\n", o_amount, lowest_f);
	}
	printf("goal not found\n");
	exit(0);
	return(false);
}