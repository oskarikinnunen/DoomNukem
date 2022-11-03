/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/03 18:21:47 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"

void render_world3d(t_sdlcontext sdl, t_world world, t_render render)
{
	t_list		*l;
	t_entity	ent;

	render.vtarget = vector3_add(render.position, render.lookdir);
	render.matcamera = matrix_lookat(render.position, render.vtarget, (t_vector3){0, 0, 1});
	render.matview = matrix_quickinverse(render.matcamera);
	l = world.entitylist;
	render_object(sdl, render, &world.skybox);
	while (l != NULL)
	{
		ent = *(t_entity *)l->content;
		render_object(sdl, render, &ent);
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