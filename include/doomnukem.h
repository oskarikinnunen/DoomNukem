/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/28 11:57:56 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOMNUKEM_H
# define DOOMNUKEM_H

# include "libft.h"
# include "log.h"
# include <errno.h>
//TODO: remove mby? just define INT_MAX yourself
# include "limits.h"
# include <math.h>
# include <fcntl.h>
# include <stdbool.h>
//PLAYER USES THIS, MOVE PLAYER TO SEPARATE HEADER
# include "animation.h"
# include "room.h"
# include "player.h"
# include "input.h"
# include "navigation.h"
# include "thread.h"
# include "decal.h"

# define PI 3.14159265359
# define FULLRAD 6.28318530718
//90 degrees in radian
# define RAD90 1.57079633f

# define IMGPATH "assets/images/"
# define OBJPATH "assets/objects/"

# define IMGLISTPATH "assets/asset_lists/image_list.txt"
# define IMGENVLISTPATH "assets/asset_lists/image_env_list.txt"
# define OBJLISTPATH "assets/asset_lists/object_list.txt"
# define MTLLISTPATH "assets/asset_lists/material_list.txt"
# define FONTLISTPATH "assets/asset_lists/font_list.txt"
# define SOUNDLISTPATH "assets/asset_lists/sound_list.txt"
# define MUSICLISTPATH "assets/asset_lists/music_list.txt"
# define ANIMLISTPATH "assets/asset_lists/anim_list.txt"
# define ANIMLEGENDPATH "assets/objects/animations/anim_legend.txt"

# define TEMPIMGLIST "assets/temp/.temp_image_list"
# define TEMPIMGENVLIST "assets/temp/.temp_image_env_list"
# define TEMPOBJLIST "assets/temp/.temp_object_list"
# define TEMPFONTLIST "assets/temp/.temp_font_list"
# define TEMPSOUNDLIST "assets/temp/.temp_sound_list"
# define TEMPMUSICLIST "assets/temp/.temp_music_list"
# define TEMPANIMLIST "assets/temp/.temp_anim_list"

# define TEMPIMG "assets/temp/.temp_image"
# define TEMPIMGENV "assets/temp/.temp_image_env"
# define TEMPOBJ "assets/temp/.temp_object"
# define TEMPMTL "assets/temp/.temp_mtl"
# define TEMPFONT "assets/temp/.temp_font"
# define TEMPSOUND "assets/temp/.temp_sound"
# define TEMPSOUNDLOOP "assets/temp/.temp_sound_loop"
# define TEMPMUSIC "assets/temp/.temp_music"
# define TEMPANIM "assets/temp/.temp_anim"
# define TEMPANIMLEGEND "assets/temp/.temp_anim_legend"

# define DEFAULTLEVEL "level0"
# define LOADINGSCREENIMG "assets/images/tga/loading_screen.tga"

# define TEXTBACKGROUND_BORDERPADDING 6
# define PERFGRAPH_SAMPLES 64

struct	s_autogui;
enum	e_load_arg;

typedef struct s_perf_graph
{
	t_img		image;
	uint32_t	deltas[PERFGRAPH_SAMPLES];
}	t_perf_graph;

typedef struct s_clock
{
	Uint32	time;
	Uint32	delta;
	Uint32	fps;
}	t_clock;

/* Playmode */

typedef enum e_cam_mode
{
	player_view,
	overhead_follow
}	t_cam_mode;

typedef struct s_log
{
	int	fd;
}	t_log;

typedef enum e_app_mode
{
	APPMODE_INVALID,
	APPMODE_EDIT,
	APPMODE_PLAY,
}	t_app_mode;

typedef struct s_app_argument
{
	t_app_mode	app_mode;
	char		level_name[128];
}	t_app_argument;

//TODO: make player just a local player, not a pointer?
typedef struct s_world
{
	char				name[32];
	t_player			*player;
	t_clock				clock;
	struct s_autogui	*debug_gui;
	t_log				log;
	t_sdlcontext		*sdl;
	t_lighting			lighting;
	t_list				*arealist;
	t_list				*ramps;
	t_entitycache		entitycache;
	t_entity			skybox;
	bool				ceiling_toggle;
	t_navigation		nav;
	uint32_t			last_save_time;
	t_app_mode			app_mode;
}	t_world;

typedef struct s_game
{
	t_world			world;
	t_hid_info		hid;
	t_player		player;
}	t_game;

t_vector2		flipped_uv(t_vector2 vec);
void			for_all_active_entities(t_world	*world, \
								void (*func)(t_entity *ent, t_world *world));
void			void_for_all_active_entities(t_world *world, void *ptr, \
									void (*func)(t_entity *ent, void *ptr));
void			for_all_entities(t_world *world, void (*func)(t_entity *ent, \
									t_world *world));
void			update_world3d(t_world *world, t_render *render);
void			toggle_ceilings(t_world *world);

t_world			load_world(t_app_argument app_argument, t_sdlcontext *sdl);

void			destroy_entity(t_world *world, t_entity *ent);
t_entity		*find_entity_with_comp(t_world *world, t_component_type comp);
t_entity		*spawn_entity(t_world *world, t_object *obj);
t_entity		*spawn_basic_entity(t_world *world, char *objectname, \
									t_vector3 position);
void			entity_assign_object(t_world *world, t_entity *entity, \
										t_object *obj);

void			world_save_to_file(t_world world);
void			world_sanitize_all_room_pointers(t_world *world);
void			world_remove_all_room_entities(t_world *world);
void			world_init(t_world *world, t_sdlcontext *sdl);
void			world_init_rooms(t_world *world);

//TODO: move to room.h
void			_room_initwalls(t_world *world, t_area *room);
void			_room_initwalls_shallow(t_world *world, t_area *room);
t_wall			*find_wall(t_wall wall, t_area *room);
void			free_floor(t_world *world, t_area *room);
void			free_ceilings(t_world *world, t_area *room);
void			free_walls(t_area *room, t_world *world);

void			draw_skybox_image(t_world *world);
void			free_list_node(void *content, size_t size);
void			playmode_quit(t_game *game);
void			update_audio(t_world *world);
void			respawn_player(t_game *game);
t_vector3		find_playerspawn(t_world *world);
void			set_up_world(t_sdlcontext *sdl, t_game *game);
void			update_window_surface(t_sdlcontext *sdl);
void			debug_ramps(t_game *game);
void			draw_fps_counter(t_sdlcontext *sdl, uint32_t fps);
void			editor_loading_screen(char *loading_message, t_sdlcontext *sdl);
void			playmode_loading_screen(char *loading_message, \
											t_sdlcontext *sdl);
void			playmode_loading_screen_loop(char *loading_message, \
											t_sdlcontext *sdl);
void			print_loading_message(char *loading_message, t_sdlcontext *sdl);
void			log_loaded_animations(t_sdlcontext *sdl);

void			editor_load_assets(t_sdlcontext *sdl);
void			editor_load_images(t_sdlcontext *sdl);
void			editor_load_env_textures(t_sdlcontext *sdl);
void			editor_load_objects(t_sdlcontext *sdl);
void			editor_load_fonts(t_sdlcontext *sdl);
void			editor_load_sounds(t_audio *audio);
void			editor_load_music(t_audio *audio);
void			editor_load_anims(char *anim_name, t_object *object);
void			editor_load_anim_legend(t_sdlcontext *sdl);

// THE PLAYMODE_LOAD_ASSET FUNCTION NEEDS TO CALL OTHER LOAD FUNCTIONS
// IN THE SAME EXACT ORDER AS THEY ARE CALLED IN WORLD_SAVE
void			playmode_load_assets(char *level_name, t_sdlcontext *sdl);
void			playmode_load_images(int level_fd, t_sdlcontext *sdl);
void			playmode_load_env_textures(int level_fd, t_sdlcontext *sdl);
void			playmode_load_objects(int level_fd, t_sdlcontext *sdl);
void			playmode_load_fonts(int level_fd, t_sdlcontext *sdl);
void			playmode_load_sounds(int level_fd, t_audio *audio);
void			playmode_load_music(int level_fd, t_audio *audio);
void			playmode_load_anims(int level_fd, char *anim_name, \
									t_object *object);
void			playmode_load_anim_legend(int level_fd, t_sdlcontext *sdl);
void			playmode_protect_npcs(t_world *world);

/* TOOLS */
// Protected ft_memalloc that calls doomlog with error code in case of an error
void			*prot_memalloc(size_t size);

/* LOG.C */
int				init_log(void);

// combines given array of strings to a singular string and 
// returns a pointer to it
char			*combine_strings(char **str);

/* EDITOR.C */
void			editor(t_app_argument app_argument, t_sdlcontext sdl);

/* EDITOR_EVENTS.C */
bool			iskey(SDL_Event e, int keycode);
void			force_mouseunlock(t_hid_info *hid);
void			force_mouselock(t_hid_info *hid);

/* EDITOR_MOUSE.C */
t_point			screentogridspace(t_point point);
t_quaternion	transformed_vector3(t_transform transform, t_vector3 v);
void			mouse_event(SDL_Event e, t_mouse *mouse);

/* SPACECONVERSIONS.C */
t_point			vector3_to_screenspace(t_vector3 vec, t_sdlcontext sdl);

/* EDITOR_MAP_IO.C */
t_list			*loadmap(char *filename);

/* DELTATIME.C */
void			update_clock(t_clock *c);

/* EDITOR_BUTTONS.C */
void			draw_editor_buttons(t_sdlcontext sdl, uint8_t tool_selected);

/* PERFGRAPH.C */
void			drawperfgraph(t_perf_graph *graph, uint32_t delta, \
								t_sdlcontext sdl);

/* CONTROLLER.C */
void			initialize_controllers(t_hid_info *hid);
void			controller_events(SDL_Event e, t_hid_info *hid);
void			controller_button_press(SDL_ControllerButtonEvent cbutton, \
										t_hid_info *hid);
void			controller_button_release(SDL_ControllerButtonEvent cbutton, \
											t_hid_info *hid);

/* GAME_RANDOM.C */
//Generates a random number between 0 and UINT32_MAX
uint32_t		game_random(t_world *world);
//Generates a random number between min and max
uint32_t		game_random_range(t_world *world, uint32_t min, uint32_t max);
bool			game_random_coinflip(t_world *world);

/* PLAYMODE.C */
void			playmode(t_app_argument app_argument, t_sdlcontext sdl);
void			playmode_death(t_game *game);

/* PLAYER.C */
void			player_init(t_player *player, t_sdlcontext *sdl, \
							t_world *world);

void			render_start_new(t_sdlcontext *sdl, t_player *player);

/* MOVEPLAYER.C */
void			moveplayer(t_player *player, t_input *input, t_world *world);

/* NAVIGATION */
t_path			pathfind(t_vector3 start_vec, t_vector3 end_vec, \
							t_world *world);
void			create_navmesh(t_world *world);
void			navmesh(t_world *world);
void			create_space_for_navmesh(t_world *world);
void			check_neighbors(t_navigation *nav, int i, t_vector3 start, \
								t_vector3 end);
void			show_navmesh(t_world *world);

void			*ft_realloc(void *src, size_t dest_size, size_t src_size);

/* COLLISION */

/* ERRORS.C */
void			error_log(int error_code);

/* SDL */
void			create_sdlcontext(t_app_argument app_argument, \
									t_sdlcontext	*sdl);
void			create_sdl_window(t_sdlcontext *sdl, t_screen_mode mode);
void			set_sdl_settings(t_sdlcontext *sdl);
void			quit_game(t_sdlcontext *sdl);

/* FONT.C */

t_font			font_parse(char *file_name);
void			draw_black_background(t_sdlcontext *sdl, t_point pos, \
										t_point size);

//TEMP, TODO: move
t_line			line_shorten(t_line line);

/* TEXT.C */

// Prints text and returns the rectangle of the printed text
// Font size and color can be set using:
// sdl->font->font = sdl->font->font_sizes[x] where x can be 0-3
// sdl->font->color = color32_to_sdlcolor(CLR_GREEN)
t_rectangle		print_text(t_sdlcontext *sdl, const char *text, t_point pos);

// Does the same as print_ftext but also fills in the background for the text
t_rectangle		print_text_boxed(t_sdlcontext *sdl, const char *text, \
								t_point pos);

t_rectangle		print_text_colored(t_sdlcontext *sdl, const char *text, \
									t_point pos, uint32_t color);
/* LIST_HELPER.C TODO: MOVE THESE TO LIBFT*/
t_list			*ptr_to_list(void	*src, uint32_t len, size_t size);
void			list_push(t_list **head, void *content, size_t content_size);
void			*list_findlast(t_list *head);
void			*list_to_ptr(t_list *source, uint32_t *set_length);
void			*list_find(t_list *head, void *match, size_t content_size);
//TODO: documentation here
void			list_remove(t_list **head, void *match, size_t content_size);

/* OCCLUSION.C */
void			update_occlusion(struct s_world *world, t_render *render);

//settings
void			default_entity_occlusion_settings(t_entity *e, t_world *world);
void			default_floor_occlusion_settings(t_meshtri *f, t_world *world);
void			default_wall_occlusion_settings(t_wall *w, t_world *world);

void			free_roomwalls(t_world *world, t_area *room);

void			set_nullentities(t_wall **ptr, int count);

//TODO: temp for lights
void			recalculate_lighting(t_world *world);
void			calculate_pointlight(t_world *world, t_entity *entity);
void			free_pointlight(t_light *light);
void			allocate_map_for_entities(t_world *world);
void			calculate_lighting(struct s_world *world);
void			calculate_texture_for_entities(struct s_world *world);
void			parse_entity_texture(int index, t_entity *entity);

void			update_entitycache(t_sdlcontext *sdl, t_world *world, \
									t_render *render);

bool			entity_has_transparent_mat(t_entity *entity);
void			update_transparent(t_sdlcontext *sdl, t_world *world, \
									t_render *render);

//Pathfind
uint32_t		get_nearest_target_node(t_world *world, t_vector3 target);

// NPC folder
void			enemy_action(t_entity *entity, t_world *world);
void			enemy_anim(t_entity *entity, t_world *world);
void			enemy_movement(t_entity *entity, t_world *world);
void			enemy_update(t_entity *entity, t_world *world);
void			enemy_take_damage(t_entity *entity, t_world *world);

void			civilian_action(t_entity *entity, t_world *world);
void			civilian_anim(t_entity *entity, t_world *world);
void			civilian_movement(t_entity *entity, t_world *world);
void			civilian_update(t_entity *entity, t_world *world);
void			civilian_take_damage(t_entity *entity, t_world *world);

#endif