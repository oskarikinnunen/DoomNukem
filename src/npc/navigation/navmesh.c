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
	int	i;
	int	j;
    uint32_t	clr = world->sdl->render.gizmocolor;

	if (!world->nav.show_navmesh)
		return;
	i = 0;
	world->sdl->render.gizmocolor = CLR_RED;
	while (i < world->nav.node_amount)
	{
		render_gizmo3d(world->sdl, world->nav.navmesh[i].mid_point, 10.0f, CLR_BLUE);
		world->sdl->render.gizmocolor = CLR_GREEN;
		j = 0;
		while (j < 3)
		{
			render_ray(world->sdl, world->nav.navmesh[i].vertex[j], world->nav.navmesh[i].vertex[(j + 1) % 3]);
			j++;
		}
		world->sdl->render.gizmocolor = CLR_RED;
		j = 0;
		while (j < world->nav.navmesh[i].neighbors)
		{
			render_ray(world->sdl, world->nav.navmesh[i].mid_point, world->nav.navmesh[world->nav.navmesh[i].neighbors_id[j]].mid_point);
			j++;
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

int	line_laps_line(t_vector3 start1, t_vector3 end1, t_vector3 p, float height)
{
	float len;
	float dist[2];

	if (vector2_cmp_epsilon(v3tov2(p), v3tov2(start1), 0.001f) || vector2_cmp_epsilon(v3tov2(p), v3tov2(end1), 0.001f))
	{
		if (float_cmp_epsilon(p.z, start1.z, height) || float_cmp_epsilon(p.z, end1.z, height))
			return(1);
		return(0);
	}
	len = vector2_dist(v3tov2(start1), v3tov2(end1));
	dist[0] = vector2_dist(v3tov2(p), v3tov2(start1));
	dist[1] = vector2_dist(v3tov2(p), v3tov2(end1));
	if (dist[0] + dist[1] <= len + 0.0001f && fabsf(vector3_fdist_to_plane(p, vector3_up(), start1)) <= height) //0.00001f
		return(2);
	return(0);
}

bool line_overlaps_parallel_line(t_vector3 start1, t_vector3 end1, t_vector3 start2, t_vector3 end2, float height)
{
	int	count;

	count = 0;
	count += line_laps_line(start1, end1, start2, height);
	count += line_laps_line(start1, end1, end2, height);
	count += line_laps_line(start2, end2, start1, height);
	count += line_laps_line(start2, end2, end1, height);
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

	dest = ft_memalloc(dest_size);
	if (!dest)
		return(NULL);
	ft_memcpy(dest, src, src_size);
	free(src);
	return(dest);
}

void	add_to_navmesh(t_navigation *nav, t_triangle t)
{
	int i;

	i = 0;
	while (i < 3)
	{
		nav->navmesh[nav->node_amount].vertex[i] = t.p[i].v;
		i++;
	}
	if (!is_triangle_degenerate(nav->navmesh[nav->node_amount].vertex))
	{
		if (nav->node_amount + 1 == nav->malloc_size)
		{
			nav->navmesh = ft_realloc(nav->navmesh, (nav->malloc_size + 1000) * sizeof(t_navnode), nav->malloc_size * sizeof(t_navnode));
			nav->malloc_size += 1000;
			if (!nav->navmesh)
				error_log(EC_MALLOC);
		}
		nav->navmesh[nav->node_amount].mid_point = vector3_div(vector3_add(vector3_add(nav->navmesh[nav->node_amount].vertex[0], nav->navmesh[nav->node_amount].vertex[1]), nav->navmesh[nav->node_amount].vertex[2]), 3.0f);
		nav->navmesh[nav->node_amount].index = nav->node_amount;
		nav->node_amount++;
	}
}

t_triangle	*add_to_cliplist(t_triangle *arr, int start, int *end, int *size, int index)
{
	t_vector3	plane_p;
	t_vector3	plane_n;

	plane_p = vector3_lerp(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v, 0.5f);
	plane_n = vector3_normalise(vector3_sub(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v));
	if (*end + 4 >= *size)
	{
		arr = ft_realloc(arr, (*size + 1000) * sizeof(t_triangle), *size * sizeof(t_triangle));
		if (!arr)
			exit(0);
		*size += 1000;
	}
	*end += clip_triangle_against_plane(plane_p, plane_n, arr[start], &(arr[*end]));
	plane_n = vector3_negative(plane_n);
	*end += clip_triangle_against_plane(plane_p, plane_n, arr[start], &(arr[*end]));
	return(arr);
}	

int		get_max_dist_index(t_triangle t)
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

void	clip_navmesh(t_navigation *nav, t_entity *ent)
{
	t_triangle	*arr;
	int			size;
	int			start = 0;
	int			end = 1;
	float		dist;
	int			index;

	size = 1000;
	arr = ft_memalloc(sizeof(t_triangle) * size);
	arr[0].p[0].v = ent->obj->vertices[0];
	arr[0].p[1].v = ent->obj->vertices[1];
	arr[0].p[2].v = ent->obj->vertices[2];
	while(start < end)
	{
		index = get_max_dist_index(arr[start]);
		dist = vector3_dist(arr[start].p[index].v, arr[start].p[(index + 1) % 3].v);
		if (dist < nav->clip_size)
			add_to_navmesh(nav, arr[start]);
		else
			arr = add_to_cliplist(arr, start, &end, &size, index);
		start++;
	}
	free(arr);
}

void	malloc_space_for_navmesh(t_world *world)
{
	int			i;
	int			found;
	t_entity	*ent;

	if (world->nav.navmesh)
		free(world->nav.navmesh);
	world->nav.malloc_size = 1000;
	world->nav.navmesh = ft_memalloc(sizeof(t_navnode) * world->nav.malloc_size);
	if (!world->nav.navmesh)
		error_log(EC_MALLOC);
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
				if (ent->obj->bounds.type == bt_ignore)
                    clip_navmesh(&world->nav, ent);
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

	if (vector3_dist(nav->navmesh[i].mid_point,  nav->navmesh[i1].mid_point) < nav->clip_size * 2.0f + nav->height)
	{
		j = 0;
		while (j < 3)
		{
			if (line_overlaps_parallel_line(start, end, nav->navmesh[i1].vertex[j], nav->navmesh[i1].vertex[(j + 1) % 3], nav->height))
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

void	create_navmesh(t_world *world)
{
	t_vector3	start;
	t_vector3	end;
	int			i;
	int			j;

	world->nav.clip_size = ft_clampf(world->nav.clip_size, 50.0f, 10000.0f);
	world->nav.height = ft_clampf(world->nav.height, 0.0f, 100000.0);
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
	world->nav.openlist = ft_memalloc(world->nav.malloc_size * sizeof(t_navnode));
	if (!world->nav.openlist)
		error_log(EC_MALLOC);
	ft_bzero(world->nav.openlist, world->nav.malloc_size * sizeof(t_navnode));
}
