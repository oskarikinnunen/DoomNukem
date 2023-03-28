/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_set_up.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 19:26:59 by raho              #+#    #+#             */
/*   Updated: 2023/03/28 12:12:12 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "editor_tools.h"
#include "movement_defs.h"

static void	world_add_ramp(t_world *world, t_entity *entity)
{
	int				i;
	t_vector3_tri	tri;

	i = 0;
	while (i < entity->obj->face_count)
	{
		if (i == 4 || i == 7)
		{
			tri = worldspace_tri(entity, i);
			list_push(&world->ramps, &tri, sizeof(tri));
		}
		i++;
	}
}

// Go through all the objects, if the name has "_ramp",
// then go through the triangles
static void	playmode_preprocess_world(t_world *world)
{
	t_entitycache	*cache;
	t_object		*ramp;
	int				i;

	cache = &world->entitycache;
	i = 0;
	ramp = get_object_by_name(*world->sdl, "concrete_ramp.obj");
	while (i < cache->alloc_count)
	{
		if (cache->entities[i].obj == ramp)
			world_add_ramp(world, &cache->entities[i]);
		if (cache->entities[i].transform.position.x > 20000000.0f || \
				cache->entities[i].transform.position.y > 20000000.0f)
			destroy_entity(world, &cache->entities[i]);
		if (cache->entities[i].component.type == COMP_STORYEVENT)
			entity_assign_object(world, &cache->entities[i], NULL);
		i++;
	}
}

t_vector3	find_playerspawn(t_world *world)
{
	int				i;
	t_entitycache	*cache;
	t_entity		*ent;
	t_vector3		result;

	cache = &world->entitycache;
	result = vector3_zero();
	i = 0;
	while (i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status == es_active && \
				ent->component.type == COMP_PLAYERSPAWN)
		{
			result = ent->transform.position;
			result.z += 10.0f;
			ent->hidden = true;
		}
		i++;
	}
	return (result);
}

void	set_up_world(t_sdlcontext *sdl, t_game *game)
{
	playmode_preprocess_world(&game->world);
	playmode_protect_npcs(&game->world);
	player_init(&game->player, sdl, &game->world);
	game->player.transform.position = find_playerspawn(&game->world);
	sdl->fog = false;
	*(game->world.debug_gui) = init_gui((t_gui_init){sdl, &game->hid, \
					&game->player, sdl->screensize, "Debugging menu (F2)"});
	game->world.debug_gui->hidden = true;
	sdl->lighting_toggled = true;
	sdl->render.occlusion.occlusion = true;
	game->world.sdl = sdl;
	sdl->audio.sfx_volume = 1.0f;
	game->world.player = &game->player;
}

void	respawn_player(t_game *game)
{
	game->player.transform.position = find_playerspawn(&game->world);
	game->player.ammo_arr[AM_ASSAULT] = 120;
	game->player.ammo_arr[AM_SNIPER] = 40;
	game->player.ammo_arr[AM_GRENADE] = 20;
	game->player.ammo_arr[AM_PISTOL] = 60;
	game->player.transform.scale = vector3_one();
	game->player.transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f};
	game->player.jump.frame_rate = 30;
	game->player.jump.last_frame = 18;
	game->player.height = PLAYER_HEIGHT;
	game->player.fov = 90.0f;
	game->player.health = 120;
	game->player.gun = &game->player.guns[0];
	game->player.gun->entity->hidden = true;
	game->player.guns[0].player_owned = false;
	game->player.guns[1].player_owned = false;
}
