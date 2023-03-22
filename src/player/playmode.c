/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:44:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 13:06:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "objects.h"
/*place holder for inits*/

#include "input.h"

static void	debug_ramps(t_game *game)
{
	t_list			*l;
	t_vector3_tri	*tri;
	int				i;

	l = game->world.ramps;
	i = 0;
	while (l != NULL)
	{
		tri = (t_vector3_tri *)l->content;
		if (i % 2 == 0)
			render_ray3D(game->world.sdl, tri->c, tri->a, CLR_RED);
		else
			render_ray3D(game->world.sdl, tri->b, tri->c, CLR_BLUE);
		i++;
		l = l->next;
	}
}
/*main game loop*/
static int gameloop(t_sdlcontext sdl, t_game game)
{
	t_gamereturn	gr;

	gr = game_continue;
	sdl.lighting_toggled = false;
	sdl.render.occlusion.occlusion = false;
	game.world.sdl = &sdl;
	sdl.audio.sfx_volume = 1.0f;
	game.world.player = &game.player;
	protagonist_play_audio(&game.player, &game.world, "protag_letsdo.wav");
	play_music(&sdl, "music_arp1_ambient.wav");
	/* //LIGHTING
	for_all_active_entities(&game.world, render_entity_worldtriangles);
	recalculate_lighting(&game.world);
	sdl.lighting_toggled = true;*/
	
	while (gr == game_continue)
	{
		if (game.player.health > 0)
		{
			update_deltatime(&game.world.clock);
			gr = playmode_events(&game);
			moveplayer(&game.player, &game.hid.input, &game.world);
			update_world3d(&game.world, &sdl.render);
			// FPS COUNTER
			print_text(&sdl, s_itoa(game.world.clock.fps), (t_point){sdl.window_w - 80, 10});
			debug_ramps(&game);
			draw_player_hud(&game.world);
			memcpy(sdl.window_surface->pixels, sdl.surface->pixels, sizeof(uint32_t) * sdl.window_w * sdl.window_h);
			if (SDL_UpdateWindowSurface(sdl.window) < 0)
				doomlog(LOG_EC_SDL_UPDATEWINDOWSURFACE, NULL);
			update_audio(&game.world);
			game_random(&game.world);
		}
		else
		{
			playmode_death(&game);
		}
	}
	if (gr == game_exit)
	{
		free_render(sdl.render);
		quit_game(&sdl);
	}

	return (gr);
}

static t_vector3 find_playerspawn(t_world *world)
{
	int i;
	t_entitycache *cache;
	t_entity *ent;
	t_vector3 result;

	cache = &world->entitycache;
	result = vector3_zero();
	i = 0;
	while (i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status == es_active && ent->component.type == COMP_PLAYERSPAWN)
		{
			result = ent->transform.position;
			result.z -= 10.0f;
			destroy_entity(world, ent);
		}
		i++;
	}
	return (result);
}

// TODO: this can use entity->worldvertices once that is in the main branch, for now this'll use this
// Defined in editor_raycast.c
t_vector3_tri worldspace_tri(t_entity *entity, int index);

static void world_add_ramp(t_world *world, t_entity *entity)
{
	int i;
	t_vector3_tri tri;

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
static void playmode_preprocess_world(t_world *world)
{
	t_entitycache *cache;
	t_object *ramp;

	int i;

	cache = &world->entitycache;
	i = 0;
	ramp = get_object_by_name(*world->sdl, "concrete_ramp.obj");
	while (i < cache->alloc_count)
	{
		if (cache->entities[i].obj == ramp)
			world_add_ramp(world, &cache->entities[i]);
		if (cache->entities[i].transform.position.x > 20000000.0f || cache->entities[i].transform.position.y > 20000000.0f)
			destroy_entity(world, &cache->entities[i]);
		i++;
	}
}

// Resetlevel??
/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game game;
	t_gamereturn gr;

	bzero(&game, sizeof(t_game));
	// Locks mouse
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
		doomlog(LOG_EC_SDL_SETRELATIVEMOUSEMODE, NULL);
	game.world = load_world_args("leveltest_bu", &sdl, LOAD_ARG_FULL);
	game.world.gamemode = MODE_PLAY;
	initialize_controllers(&game.hid);
	create_navmesh(&game.world);
	playmode_preprocess_world(&game.world);
	player_init(&game.player, &sdl, &game.world);
	game.player.transform.position = find_playerspawn(&game.world);
	sdl.fog = true;
	gr = gameloop(sdl, game);
	if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0)
		doomlog(LOG_EC_SDL_SETRELATIVEMOUSEMODE, NULL);
	return (gr);
}
