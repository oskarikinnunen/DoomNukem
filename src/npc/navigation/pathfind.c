#include "doomnukem.h"

uint32_t get_nearest_target_node(t_world *world, t_vector3 target)
{
	int e = 0;
	int closest_point = 0;
	float dist;
	dist = vector3_dist(world->nav.navmesh[0].mid_point, target);
	while (e < world->nav.node_amount)
	{
		if (dist > vector3_dist(world->nav.navmesh[e].mid_point, target))
		{
			dist = vector3_dist(world->nav.navmesh[e].mid_point, target);
			closest_point = e;
		}
		e++;
	}
	return(closest_point);
}

t_path path_find(t_vector3 start_vec, t_vector3 end_vec, t_world *world)
{
	t_navnode	*openlist;
	uint32_t	o_amount;
	uint32_t	lowest_f;
	uint32_t	start;
	uint32_t	end;
	t_path		path;
	int	i;

	openlist = world->nav.openlist;
	ft_bzero(&path, sizeof(path));
	ft_bzero(openlist, sizeof(t_navnode) * world->nav.node_amount);
	path.start = get_nearest_target_node(world, start_vec);
	path.end = get_nearest_target_node(world, end_vec);
	start = path.end;
	end = path.start;
	openlist[start] = world->nav.navmesh[start];
	openlist[start].valid = true;
	openlist[start].enter_point = openlist[start].mid_point;
	o_amount = 1;
	if (world->nav.node_amount == 0)
		return((t_path){.valid_path = false});
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
			if (i > world->nav.node_amount)
			{
				printf("o_amount %d found %d\n", o_amount, found);
				printf("buffer overflow in pathfind, should never happen index is %d, node amount is %d\n", i, world->nav.node_amount);
				exit(0);
			}
		}
		if (lowest_f == end)
		{
			int e, i;

			i = 0;
			e = end;
			path.path[i] = openlist[e];
			while (e != start && i < 32)
			{
				path.path[i] = openlist[e];
				e = openlist[e].parent;
				i++;
			}
			if (i != 32)
			{
				path.path[i] = openlist[e];
				path.path[i].enter_point = path.path[i].mid_point;
			}
			else
				path.path[i - 1].enter_point = path.path[i - 1].mid_point;
			path.ai = 0;
			path.bi = i;
			path.valid_path = true;
			return(path);
		}
		openlist[lowest_f].valid = false;
		openlist[lowest_f].visited = true;
		o_amount--;
		i = 0;
		while (i < openlist[lowest_f].neighbors)
		{
			uint32_t id;
			id = openlist[lowest_f].neighbors_id[i];
			if (openlist[id].visited == false && openlist[id].blocked == false && openlist[id].valid == false)
			{
				openlist[id] = world->nav.navmesh[id];
				openlist[id].g = openlist[lowest_f].g + 1.0f;
				openlist[id].h = vector3_dist(openlist[id].mid_point, world->nav.navmesh[end].mid_point);
				openlist[id].f = openlist[id].g + openlist[id].h;
				openlist[id].valid = true;
				openlist[id].parent = lowest_f;
				openlist[id].enter_point = openlist[lowest_f].line_point[i];
				o_amount++;
			}
			i++;
		}
	}
	return((t_path){.valid_path = false});
}

bool pathfind(t_world *world, t_path *path)
{
	t_navnode	*openlist;
	uint32_t	o_amount;
	uint32_t	lowest_f;
	uint32_t	start;
	uint32_t	end;
	int	i;

	openlist = world->nav.openlist;
	ft_bzero(openlist, sizeof(t_navnode) * world->nav.node_amount);
	start = path->end;
	end = path->start;
	openlist[start] = world->nav.navmesh[start];
	openlist[start].valid = true;
	openlist[start].enter_point = openlist[start].mid_point;
	o_amount = 1;
	if (world->nav.node_amount == 0)
		return(false);
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
			if (i > world->nav.node_amount)
			{
				printf("o_amount %d found %d\n", o_amount, found);
				printf("buffer overflow in pathfind, should never happen index is %d, node amount is %d\n", i, world->nav.node_amount);
				exit(0);
			}
		}
		if (lowest_f == end)
		{
			int e, i;

			i = 0;
			e = end;
			path->path[i] = openlist[e];
			while (e != start && i < 32)
			{
				path->path[i] = openlist[e];
				e = openlist[e].parent;
				i++;
			}
			if (i != 32)
			{
				path->path[i] = openlist[e];
				path->path[i].enter_point = path->path[i].mid_point;
			}
			else
				path->path[i - 1].enter_point = path->path[i - 1].mid_point;
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
			if (openlist[id].visited == false && openlist[id].blocked == false && openlist[id].valid == false)
			{
				openlist[id] = world->nav.navmesh[id];
				openlist[id].g = openlist[lowest_f].g + 1.0f;
				openlist[id].h = vector3_dist(openlist[id].mid_point, world->nav.navmesh[end].mid_point);
				openlist[id].f = openlist[id].g + openlist[id].h;
				openlist[id].valid = true;
				openlist[id].parent = lowest_f;
				openlist[id].enter_point = openlist[lowest_f].line_point[i];
				o_amount++;
			}
			i++;
		}
	}
	return(false);
}