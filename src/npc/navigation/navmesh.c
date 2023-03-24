#include "doomnukem.h"
#include "movement_defs.h"

bool	node_would_collide(t_world *world, t_vector3 midpoint)
{
	t_character_physics	phys;
	t_vector3			vec;

	ft_bzero(&phys, sizeof(phys));
	phys.radius = 17.5f;
	phys.height = PLAYER_HEIGHT;
	phys.position = &midpoint;
	vec = midpoint;
	if (check_character_collision(world, phys, vector3_add(midpoint, (t_vector3){0.0f, 0.0f, 0.0001f}), &vec))
		return (true);
	return (false);
}

void    show_navmesh(t_world *world)
{
	int	i;
	int	j;
    uint32_t	clr = world->sdl->render.gizmocolor;

	if (!world->nav.show_navmesh)
		return;
	i = 0;
	world->sdl->render.gizmocolor = CLR_RED;
	while (i < world->nav.node_amount)
	{
		if (world->nav.navmesh[i].blocked)
		{
			//render_circle(world->sdl, world->nav.navmesh[i].mid_point, 10.0f, CLR_RED);
		}	else
		{
			//render_circle(world->sdl, world->nav.navmesh[i].mid_point, 10.0f, CLR_BLUE);
			float fdist = vector3_sqr_dist(world->player->transform.position, world->nav.navmesh[i].mid_point);
			if (fdist < 40000.0f) 
			{
				world->sdl->render.gizmocolor = AMBER_1;
				if (fdist < 40000.0f)
					world->sdl->render.gizmocolor = AMBER_3;
				j = 0;
				while (j < 3)
				{
					render_ray(world->sdl, world->nav.navmesh[i].vertex[j], world->nav.navmesh[i].vertex[(j + 1) % 3]);
					j++;
				}
				j = 0;
				while (j < world->nav.navmesh[i].neighbors)
				{
					render_ray3D(world->sdl, world->nav.navmesh[i].mid_point, world->nav.navmesh[world->nav.navmesh[i].neighbors_id[j]].mid_point, CLR_RED);
					j++;
				}
				
				t_point sp = vector3_to_screenspace(world->nav.navmesh[i].mid_point, *world->sdl);
				print_text_boxed(world->sdl, s_itoa(world->nav.navmesh[i].neighbors), sp);
			}

			
		}
		i++;
	}
	world->sdl->render.gizmocolor = clr;
}

/*
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
*/

//TODO: modify this in a way that allows higher nodes to connect to lower ones
int	line_laps_line(t_vector3 start1, t_vector3 end1, t_vector3 p)
{
	float len;
	float dist[2];

	if (vector2_cmp_epsilon(v3tov2(p), v3tov2(start1), 0.001f) || vector2_cmp_epsilon(v3tov2(p), v3tov2(end1), 0.001f))
	{
		if (float_cmp_epsilon(p.z, start1.z, COL_STEP) || float_cmp_epsilon(p.z, end1.z, COL_STEP))
			return(1);
		return(0);
	}
	len = vector2_dist(v3tov2(start1), v3tov2(end1));
	dist[0] = vector2_dist(v3tov2(p), v3tov2(start1));
	dist[1] = vector2_dist(v3tov2(p), v3tov2(end1));
	if (dist[0] + dist[1] <= len + 0.0001f && fabsf(vector3_fdist_to_plane(p, vector3_up(), start1)) <= COL_STEP) //0.00001f
		return(2);
	return(0);
}

bool	line_overlaps_parallel_line(t_vector3 start1, t_vector3 end1, t_vector3 start2, t_vector3 end2)
{
	int	count;

	count = 0;
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

void	*ft_realloc(void *src, size_t dest_size, size_t src_size) //TODO: Should probably do the actual implementation of this
{
	void *dest;

	dest = prot_memalloc(dest_size);
	if (!dest)
		return(NULL);
	ft_memcpy(dest, src, src_size);
	free(src);
	return(dest);
}



void	add_to_navmesh(t_navigation *nav, t_world_triangle t, t_world *world)
{
	int i;
	t_vector3	midpoint;

	i = 0;
	while (i < 3)
	{
		nav->navmesh[nav->node_amount].vertex[i] = t.p[i].v;
		i++;
	}
	midpoint = vector3_div(vector3_add(vector3_add(nav->navmesh[nav->node_amount].vertex[0], nav->navmesh[nav->node_amount].vertex[1]), nav->navmesh[nav->node_amount].vertex[2]), 3.0f);
	if (!is_triangle_degenerate(nav->navmesh[nav->node_amount].vertex)
		&& !node_would_collide(world, midpoint))
	{
		if (nav->node_amount + 1 == nav->malloc_size)
		{
			nav->navmesh = ft_realloc(nav->navmesh, (nav->malloc_size + 1000) * sizeof(t_navnode), nav->malloc_size * sizeof(t_navnode));
			nav->malloc_size += 1000;
			if (!nav->navmesh)
				doomlog(LOG_EC_MALLOC, NULL);
		}
		nav->navmesh[nav->node_amount].mid_point = vector3_div(vector3_add(vector3_add(nav->navmesh[nav->node_amount].vertex[0], nav->navmesh[nav->node_amount].vertex[1]), nav->navmesh[nav->node_amount].vertex[2]), 3.0f);
		nav->navmesh[nav->node_amount].index = nav->node_amount;
		nav->node_amount++;
	}
}

t_world_triangle	*add_to_cliplist(t_world_triangle *arr, int start, int *end, int *size, int index)
{
	t_vector3	plane_p;
	t_vector3	plane_n;

	plane_p = vector3_lerp(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v, 0.5f);
	plane_n = vector3_normalise(vector3_sub(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v));
	if (*end + 4 >= *size)
	{
		arr = ft_realloc(arr, (*size + 1000) * sizeof(t_world_triangle), *size * sizeof(t_world_triangle));
		if (!arr)
			exit(0);
		*size += 1000;
	}
	*end += clip_triangle_plane(plane_p, plane_n, arr[start], &(arr[*end]));
	plane_n = vector3_negative(plane_n);
	*end += clip_triangle_plane(plane_p, plane_n, arr[start], &(arr[*end]));
	return(arr);
}	

int		get_max_dist_index(t_world_triangle t)
{
	float	dist;
	int		index;
	int		i;

	index = 0;
	dist = vector3_dist(t.p[0].v, t.p[1].v);
	i = 1;
	while (i < 3)
	{
		if (dist < vector3_dist(t.p[i].v, t.p[(i + 1) % 3].v))
		{
			dist = vector3_dist(t.p[i].v, t.p[(i + 1) % 3].v);
			index = i;
		}
		i++;
	}
	return (index);
}

void	clip_navmesh(t_navigation *nav, t_entity *ent, t_world *world)
{
	t_world_triangle	*arr;
	int			size;
	int			start = 0;
	int			end = 1;
	float		dist;
	int			index;

	size = 1000;
	arr = prot_memalloc(sizeof(t_world_triangle) * size);
	arr[0].p[0].v = ent->obj->vertices[0];
	arr[0].p[1].v = ent->obj->vertices[1];
	arr[0].p[2].v = ent->obj->vertices[2];
	while(start < end)
	{
		index = get_max_dist_index(arr[start]);
		dist = vector3_dist(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v);
		if (dist < nav->clip_size)
			add_to_navmesh(nav, arr[start], world);
		else
			arr = add_to_cliplist(arr, start, &end, &size, index);
		start++;
	}
	free(arr);
}

//TODO: change to use world->roomlist floors
void	malloc_space_for_navmesh(t_world *world)
{
	int			i;
	int			found;
	t_entity	*ent;

	if (world->nav.navmesh)
		free(world->nav.navmesh);
	world->nav.malloc_size = 1000;
	world->nav.navmesh = prot_memalloc(sizeof(t_navnode) * world->nav.malloc_size);
	bzero(world->nav.navmesh, sizeof(t_navnode) * world->nav.malloc_size);
	i = 0;
	found = 0;
	world->nav.node_amount = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = world->entitycache.sorted_entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
				if (ent->occlusion.world_tri_count == 1)
					clip_navmesh(&world->nav, ent, world);
			found++;
		}
		i++;
	}
}

bool	is_already_neighbor(t_navigation *nav, int i, int nb)
{
	int	index;

	index = 0;
	while (index < nav->navmesh[i].neighbors)
	{
		if (nav->navmesh[i].neighbors_id[index] == nb)
			return(true);
		index++;
	}
	return(false);
}

bool	triangles_share_border(t_navigation *nav, int i, int i1, t_vector3 start, t_vector3 end)
{
	int	j;

	if (vector3_dist(nav->navmesh[i].mid_point,  nav->navmesh[i1].mid_point) < 1000000.0f)
	{
		j = 0;
		while (j < 3)
		{
			if (line_overlaps_parallel_line(start, end, nav->navmesh[i1].vertex[j], nav->navmesh[i1].vertex[(j + 1) % 3]))
				return(true);
			j++;
		}
	}
	return(false);
}

void	check_neighbors(t_navigation *nav, int i, t_vector3 start, t_vector3 end)
{
	int		i1;

	i1 = 0;
	while (i1 < nav->node_amount)
	{
		if (i == i1 && i1++)
			continue;
		if (is_already_neighbor(nav, i, i1) && i1++)
			continue;
		if (triangles_share_border(nav, i, i1, start, end))
		{
			nav->navmesh[i].line_point[nav->navmesh[i].neighbors] = vector3_lerp(start, end, 0.5f);
			nav->navmesh[i].neighbors_id[nav->navmesh[i].neighbors++] = i1;
		}
		i1++;
	}
}

#include "physics.h"

void	navmesh_process(t_world *world)
{
	return ;
	t_vector3	newpos;
	t_character_physics	phys;
	int			i;

	phys.height = PLAYER_HEIGHT;
	phys.radius = 25.0f;
	i = 0;
	while (i < world->nav.node_amount)
	{
		/*if (node_would_collide(world, &world->nav.navmesh[i]))
		{
			printf("blocking nav %i \n", i);
			world->nav.navmesh[i].blocked = true;
		}*/
			
		//if 
		//world->nav.navmesh[i];
		i++;
	}
	//world->nav.navmesh->mid_point
}

#include "movement_defs.h"

void	create_navmesh(t_world *world)
{
	t_vector3	start;
	t_vector3	end;
	int			i;
	int			j;

	world->nav.clip_size = ft_clampf(world->nav.clip_size, 50.0f, 10000.0f);
	world->nav.height = ft_clampf(world->nav.height, 0.0f, 100000.0);
	world->nav.height = COL_STEP;
	malloc_space_for_navmesh(world);
	printf("navigation node count %d(printf called from navigation/navmesh.c)\n", world->nav.node_amount);
    i = 0;
	while (i < world->nav.node_amount)
	{
		world->nav.navmesh[i].neighbors = 0;
		j = 0;
		while (j < 3)
		{
			start = world->nav.navmesh[i].vertex[j];
			end = world->nav.navmesh[i].vertex[(j + 1) % 3];
			check_neighbors(&world->nav, i, start, end);
			j++;
		}
		i++;
	}
	if (world->nav.openlist)
		free(world->nav.openlist);
	world->nav.openlist = prot_memalloc(world->nav.malloc_size * sizeof(t_navnode));
	if (!world->nav.openlist)
		doomlog(LOG_EC_MALLOC, NULL);
	ft_bzero(world->nav.openlist, world->nav.malloc_size * sizeof(t_navnode));
}
