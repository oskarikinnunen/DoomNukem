/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 17:40:53 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/08 06:11:35 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"

void render_world3d(t_sdlcontext sdl, t_world world, t_render *render)
{
	t_list		*l;
	t_entity	ent;
	t_wall		wall;

	render_entity(sdl, *render, &world.skybox);
	l = world.wall_list;
	while (l != NULL)
	{
		wall = *(t_wall *)l->content;
		render_object(sdl, *render, &wall.object);
		l = l->next;
	}
	l = world.entitylist;
	while (l != NULL)
	{
		ent = *(t_entity *)l->content;
		render_entity(sdl, *render, &ent);
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

void	scale_skybox_uvs(t_object *obj)
{
	int	i;

	i = 0;
	while (i < obj->uv_count)
	{
		obj->uvs[i] = vector2_mul(obj->uvs[i], 10.0f);
		i++;
	}
}

static void load_walltextures(t_world *world, t_sdlcontext sdl)
{
	t_list		*l;
	t_wall		*wall;
	t_material	*mat;

	l = world->wall_list;
	while (l != NULL)
	{
		wall = (t_wall *)l->content;
		mat = &wall->object.materials[0];
		mat->img = get_image_by_name(sdl, mat->texturename);
		l = l->next;
	}
}

t_world	load_world(char *filename, t_sdlcontext sdl)
{
	t_world	world;

	ft_bzero(&world, sizeof(t_world));
	world.entitylist = load_chunk(filename, "ENT_", sizeof(t_entity));
	world.wall_list = load_chunk(filename, "WALL", sizeof(t_wall));
	entity_init(&world, sdl);
	walls_init(&world);
	load_walltextures(&world, sdl);
	ft_bzero(&world.skybox, sizeof(t_entity));
	world.skybox.obj = get_object_by_name(sdl, "cube");
	world.skybox.obj->materials[0].img = get_image_by_name(sdl, "grid.png");
	//scale_skybox_uvs(world.skybox.obj);
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
	save_chunk(filename, "WALL", world.wall_list);
	//unscale_skybox_uvs(world.skybox.obj);
}