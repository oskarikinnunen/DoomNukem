/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/04 22:25:48 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"

static void sort_quaternions_x(t_quaternion *q, int high)
{
	int i, j;
	t_quaternion key;
	for (i = 1; i < high; i++)
	{
		key = q[i];
		j = i - 1;
		while (j >= 0 && q[j].v.x > key.v.x)
		{
			q[j + 1] = q[j];
			j = j - 1;
		}
		q[j + 1] = key;
	}
}

void render_world3d(t_sdlcontext sdl, t_world world, t_render render)
{
	t_list		*l;
	t_entity	ent;

	render.vtarget = vector3_add(render.position, render.lookdir);
	render.matcamera = matrix_lookat(render.position, render.vtarget, (t_vector3){0, 0, 1});
	render.matview = matrix_quickinverse(render.matcamera);
	l = world.entitylist;
	render_object(sdl, render, &world.skybox);

	render.occlusion = occluded;
	while (l != NULL)
	{
		ent = *(t_entity *)l->content;
		render_object(sdl, render, &ent);
		l = l->next;
	}

	l = world.entitylist;
	int index;
	t_object	*obj;
	t_vector3	temp;
	render.occluder_count = 0;
	while (l != NULL)
	{
		uint32_t clr;
		ent = *(t_entity *)l->content;
		obj = ent.obj;
		if (ent.object_index == 0)
		{
			render.occluder[render.occluder_count++] = ent;
			ent.occluder = true;
			clr = CLR_TURQ;
		}
		else
		{
			ent.occluder = false;
			clr = INT_MAX;
		}
		if (render.occlusion == occluder)
		{
			render.occluder[render.occluder_count++] = ent;
		}
		index = 0;
		while (index < obj->vertice_count)
		{
			temp = vector3_mul_vector3(ent.transform.scale, obj->vertices[index]);
			temp = vector3_add(ent.transform.location, temp);
			render.q[index] = vector3_to_quaternion(temp);
			render.q[index] = quaternion_mul_matrix(render.matworld, render.q[index]);
			render.q[index] = quaternion_mul_matrix(render.matview, render.q[index]);
			render.q[index] = quaternion_to_screenspace(render.matproj, render.q[index], sdl);
			index++;
		}
		sort_quaternions_x(render.q, obj->vertice_count);
		int temp = 0;
		uint32_t f_clr = 0;
		t_quaternion occl[2];
		for (int i = 0; i < sdl.window_w; i++)
		{
			if (i == (int)render.q[temp].v.x)
			{
				f_clr = clr;
				temp++;
			}
			if (((uint32_t *)sdl.surface->pixels)[i + ((sdl.window_h - 50)) * sdl.window_w] != clr)
			for (int j = 0; j < 100; j++)
			{
				((uint32_t *)sdl.surface->pixels)[i + ((sdl.window_h - 50 - j)) * sdl.window_w] = f_clr;
			}
			if (temp == obj->vertice_count)
				f_clr = 0;
		}
		l = l->next;
	}
}

void	calculate_colliders_for_entities(t_world *world)
{
	t_list *l;
	int		i;

	l = world->entitylist;
	i = 0;
	while (l != NULL)
	{
		world->physics.entities[i] = (t_entity *)l->content;
		l = l->next;
		i++;
	}
	world->physics.entities[i] = NULL;
	calculate_colliders(&world->physics);
}

static void	entity_init(t_world *world, t_sdlcontext sdl)
{
	t_list		*l;
	t_entity	*ent;

	l = world->entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		if (ent->object_index < sdl.objectcount)
			ent->obj = &sdl.objects[ent->object_index]; //TODO: getobjectbyindex
		else
			ent->obj = &sdl.objects[0];
		l = l->next;
	}
}

t_world	load_world(char *filename, t_sdlcontext sdl)
{
	t_world	world;

	world.entitylist = load_chunk(filename, "ENT_", sizeof(t_entity));
	entity_init(&world, sdl);
	ft_bzero(&world.skybox, sizeof(t_entity));
	world.skybox.obj = get_object_by_name(sdl, "cube");
	world.skybox.obj->materials[0].img = get_image_by_name(sdl, "quake1.png");
	world.skybox.transform.scale = vector3_mul(vector3_one(), 1000.0f);
	world.skybox.transform.location = (t_vector3){500.0f, 500.0f, 500.0f};
	calculate_colliders_for_entities(&world);
	return (world);
}


static int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1)
		error_log(EC_WRITE);
	return (fd);
}

void	save_world(char *filename, t_world world)
{
	int	fd;

	fd = fileopen(filename, O_RDWR | O_CREAT | O_TRUNC); //Empty the file or create a new one if it doesn't exist
	close(fd);
	save_chunk(filename, "ENT_", world.entitylist);
}