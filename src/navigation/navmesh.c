#include "doomnukem.h"
#include "movement_defs.h"

//TODO: remove
void	show_navmesh(t_world *world)
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

void	create_navmesh(t_world *world)
{
	t_vector3	start;
	t_vector3	end;
	int			i;
	int			j;

	world->nav.clip_size = ft_clampf(world->nav.clip_size, 50.0f, 10000.0f);
	world->nav.height = COL_STEP; // remove nav.height
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
