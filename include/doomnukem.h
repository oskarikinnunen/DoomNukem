/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/27 22:08:35 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOMNUKEM_H
# define DOOMNUKEM_H

# include "libft.h"
# include "log.h"
# include "limits.h" //TODO: remove mby? just define INT_MAX yourself
# include <math.h>
# include <fcntl.h>
# include <stdbool.h>
//# include "physics.h"
# include "animation.h" //PLAYER USES THIS, MOVE PLAYER TO SEPARATE HEADER
# include "room.h"
# include "npcs.h"
# include "player.h"
# include "input.h"
# include "debug.h"
# include "navigation.h"

# define PI	3.14159265359
# define FULLRAD M_PI * 2.0
# define RAD90		1.57079633f //90 degrees in radian

# define DEBUG_ON 1

#define A_MASK 0xFF >> 24 //DEBUGGING

# define IMGPATH "assets/images/"
# define OBJPATH "assets/objects/"

# define TEXTBACKGROUND_BORDERPADDING 6

void	screen_blank(t_sdlcontext sdl);
# define PERFGRAPH_SAMPLES 64

typedef struct s_perfgraph
{
	t_img		image;
	uint32_t	deltas[PERFGRAPH_SAMPLES];
}	t_perfgraph;

typedef struct s_clock
{
	Uint32	prev_time;
	Uint32	delta;
	Uint32	fps;
} t_clock;

/* Playmode */

typedef enum	e_cam_mode
{
	player_view,
	overhead_follow
}	t_cam_mode;

typedef enum e_gamereturn
{
	game_error,
	game_continue,
	game_exit,
	game_switchmode
} t_gamereturn;


/*typedef struct s_animframe
{
	struct deltamove = 
	{
		int			index;
		t_vector3	delta;
	};
	int deltaverticecount;
}	t_animframe;*/

/*
	void	spawn_npc(t_world *world, char *objectname, t_vector3 position)
	void	update_npcs(t_world *world) //moves npcs towards their destination, updates their animations
*/

struct s_autogui;

typedef struct s_log
{
	int	fd;
}	t_log;

typedef enum e_gamemode
{
	MODE_PLAY,
	MODE_EDITOR
}	t_gamemode;

typedef struct s_world
{
	char				name[32];
	t_player			*player;
	t_clock				clock;
	t_debugconsole		debugconsole;
	struct s_autogui	*debug_gui;
	t_log				log;
	t_sdlcontext		*sdl;
	t_list				*guns;
	t_list				*prefabs; //TODO: move to editor
	t_lighting			lighting;
	t_list				*roomlist;
	t_entitycache		entitycache;
	t_entity			skybox;
	bool				ceiling_toggle;
	t_navigation		nav;
	uint32_t			lastsavetime;
	t_gamemode			gamemode;
}	t_world;

t_vector2	flipped_uv(t_vector2 og);
void		for_all_active_entities(t_world	*world, void	(*func)(t_entity *ent, t_world *world));
void		for_all_entities(t_world	*world, void	(*func)(t_entity *ent, t_world *world));
void		update_world3d(t_world *world, t_render *render);
void		toggle_ceilings(t_world *world);


enum e_load_arg;
t_world		load_world_args(char *filename, t_sdlcontext *sdl, enum e_load_arg arg);
t_world		load_world(char *filename, t_sdlcontext *sdl);


void		destroy_entity(t_world *world, t_entity *ent);
t_entity	*spawn_entity(t_world	*world);
t_entity	*find_entity_with_comp(t_world	*world, t_componenttype comp);
t_entity	*spawn_basic_entity(t_world *world, char *objectname, t_vector3 position);
void		entity_assign_object(t_world *world, t_entity *entity, t_object *obj);
void		save_world(char *filename, t_world world);


void		world_save_to_file(t_world world);
void		_world_sanitize_all_room_pointers(t_world *world);
void		_world_init_rooms(t_world *world);

//TODO: move to room.h
void		_room_initwalls(t_world *world, t_area *room);
t_wall		*find_wall(t_wall wall, t_area *room);
void		free_floor(t_world *world, t_area *room);
void		free_ceilings(t_world *world, t_area *room);
void		free_walls(t_area *room, t_world *world);

typedef struct s_game
{
	t_world			world;
	t_clock			clock;
	t_hid_info		hid;
	t_player		player;
	t_cam_mode		cam_mode; //Unused but will be reimplemented?
} t_game;

/* LOG.C */
int		init_log(void);

/* EDITOR.C */
int		editorloop(t_sdlcontext sdl);

/* EDITOR_EVENTS.C */
bool			iskey(SDL_Event e, int keycode);
void			force_mouseunlock(t_hid_info *hid);
void			force_mouselock(t_hid_info *hid);

/* EDITOR_MOUSE.C */
t_point			screentogridspace(t_point point);
t_quaternion	transformed_vector3(t_transform transform, t_vector3 v);//TODO: vectors.h
void			mouse_event(SDL_Event e, t_mouse *mouse);//TODO: input.h

/* SPACECONVERSIONS.C */
t_point vector3_to_screenspace(t_vector3 vec, t_sdlcontext sdl);

/* EDITOR_MAP_IO.C */
t_list	*loadmap(char *filename);

/* DELTATIME.C */
void	update_deltatime(t_clock *c);

/* EDITOR_BUTTONS.C */
void	draw_editor_buttons(t_sdlcontext sdl, uint8_t tool_selected); //TODO: MOVE TO EDITOR_TOOLS

/* PERFGRAPH.C */
void	drawperfgraph(t_perfgraph *graph, uint32_t delta, t_sdlcontext sdl);

/* CONTROLLER.C */
void	initialize_controllers(t_hid_info *hid);
int		controller_events(SDL_Event e, t_hid_info *hid);

/* GAME_RANDOM.C */
//Generates a random number between 0 and UINT32_MAX
uint32_t	game_random(t_world *world);
//Generates a random number between min and max
uint32_t	game_random_range(t_world *world, uint32_t min, uint32_t max);
bool		game_random_coinflip(t_world *world);

/* PLAYMODE.C */
int		playmode(t_sdlcontext sdl);

/* PLAYER.C */
void	player_init(t_player *player, t_sdlcontext *sdl, t_world *world);
void	update_render(t_render *render, t_player *player);

/* MOVEPLAYER.C */
void	moveplayer(t_player *player, t_input *input, t_world *world);

/* NAVIGATION */
bool	pathfind(t_world *world, t_path *path); //TODO: deprecated
t_path	path_find(t_vector3 start_vec, t_vector3 end_vec, t_world *world);
void	create_navmesh(t_world *world);
void	show_navmesh(t_world *world);

/* COLLISION */
bool	check_collision(t_world *world, t_player *player, t_vector3 potential_pos, t_vector3 *newpos);
bool	alaiwan_collision(t_world *world, t_player *player, t_vector3 potential_pos, t_vector3 *new_pos);


/* ERRORS.C */
void	error_log(int error_code);

/* SDL */
void	quit_game(t_sdlcontext *sdl);

void	apply_graphics_prefs(t_graphicprefs prefs);

/* FONT.C */

void	load_fonts(t_font *font);

void	draw_black_background(t_sdlcontext *sdl, t_point pos, t_point size);

//TEMP, TODO: move
t_line	line_shorten(t_line line);

/* TEXT.C */

// Prints text and returns the rectangle of the printed text
// Font size and color can be set using:
// sdl->font->font = sdll->font->font_sizes[x] where x can be 0-3
// sdl->font->color = sdl->font->font_colors.x where x is the color
SDL_Color	color32_to_sdlcolor(uint32_t color);

t_rectangle	print_text(t_sdlcontext *sdl, const char *text, t_point pos);

// Does the same as print_ftext but also fills in the background for the text
t_rectangle	print_text_boxed(t_sdlcontext *sdl, const char *text, t_point pos);

t_rectangle	print_text_colored(t_sdlcontext *sdl, const char *text, t_point pos, uint32_t color);


void			entity_start_anim(t_entity *entity, char *animname);
/* LIST_HELPER.C TODO: MOVE THESE TO LIBFT*/
t_list	*ptr_to_list(void	*src, uint32_t len, size_t size);
void	ptr_add(void **ptr, uint32_t *len, size_t size, void *add);
void	list_push(t_list **head, void *content, size_t content_size);
void	*list_findlast(t_list *head);
void	*list_to_ptr(t_list *source, uint32_t *set_length);
void	*list_find(t_list *head, void *match, size_t content_size);
//TODO: documentation here
void	list_remove(t_list **head, void *match, size_t content_size);

/* OCCLUSION.C */
void	update_occlusion(struct s_world *world, t_render *render);

//settings
void	default_entity_occlusion_settings(t_entity *e, t_world *world);
void	default_floor_occlusion_settings(t_meshtri *f, t_world *world);
void	default_wall_occlusion_settings(t_wall *w, t_world *world);

void	update_object_bounds(t_object *obj);
void	update_floor_bounds(t_meshtri *f);
void	update_wall_bounds(t_wall *w);

void	free_roomwalls(t_world *world, t_area *room);

void	set_nullentities(t_wall **ptr, int count);

//TODO: temp for lights
void	start_lightbake(t_render *render, t_world *world);
void	bake_lights(t_render *render, t_world *world);
void	recalculate_lighting(t_world *world);

void	rasterize_lightpoly(t_lightpoly triangle, t_world *world);


uint8_t *smooth_lightmap(t_lightmap *lmap);

void	bake_lighting(t_render *render, t_world *world);
void	bake_lighting_shadows(t_render *render, t_world *world);
void	render_entity_depth_buffer(t_sdlcontext sdl, t_render *render, t_entity *entity);
void	update_arealights_for_entity(t_sdlcontext sdl, t_render *render, t_entity *entity);
void	update_pointlight_for_entity(t_sdlcontext sdl, t_render *render, t_entity *entity);
void	calculate_pointlight(t_pointlight *pointlight, t_world *world, t_render *render);
void	calculate_pointlight_step(t_pointlight *pointlight, t_world *world, t_render *render);
//

//Pathfind
uint32_t get_nearest_target_node(t_world *world, t_vector3 target);

// NPC folder
void	enemy_action(t_entity *entity, t_world *world);
void	enemy_anim(t_entity *entity, t_world *world);
void	enemy_movement(t_entity *entity, t_world *world);
void	enemy_update(t_entity *entity, t_world *world);
void	enemy_take_damage(t_entity *entity, t_world *world);

void	civilian_action(t_entity *entity, t_world *world);
void	civilian_anim(t_entity *entity, t_world *world);
void	civilian_movement(t_entity *entity, t_world *world);
void	civilian_update(t_entity *entity, t_world *world);
void	civilian_take_damage(t_entity *entity, t_world *world);
#endif
