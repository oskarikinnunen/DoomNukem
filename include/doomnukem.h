/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/07 12:16:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOMNUKEM_H
# define DOOMNUKEM_H

# include "libft.h"
# include "error_log.h"
# include "limits.h" //TODO: remove mby? just define INT_MAX yourself
# include <math.h>
# include <fcntl.h>
# include <stdbool.h>
# include "physics.h"//DEPRECATED UPDATED TO COLLISION.H
# include "animation.h" //PLAYER USES THIS, MOVE PLAYER TO SEPARATE HEADER
# include "room.h"
# include "npcs.h"
# include "player.h"
# include "input.h"
# include "debug.h"

# define PI	3.14159265359
# define FULLRAD M_PI * 2.0
# define RAD90		1.57079633f //90 degrees in radian

# define DEBUG_ON 1

#define A_MASK 0xFF >> 24 //DEBUGGING

# define IMGPATH "assets/images/"
# define OBJPATH "assets/objects/"

// Playmode defines
# define OVERHEADCAMSPEED 0.028f
# define PLAYERRADIUS 16

# define EDITOR_MOVESPEED 0.2f
# define MOVESPEED 0.2f
# define MAXMOVEMENTSPEED 0.08f
# define ROTATESPEED 0.002f
# define MOUSESPEED 0.0002f

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

typedef struct s_world
{
	t_clock				clock;
	t_debugconsole		debugconsole;
	struct s_autogui	*debug_gui;
	t_sdlcontext		*sdl;
	t_list				*guns;
	t_list				*objectmetadatalist; //Move to sdl, this is only used when objects are initialized
	t_npc				npcpool[128];
	//t_list				*wall_list;
	t_list				*roomlist;
	t_entitycache		entitycache;
	t_entity			skybox;
}	t_world;

t_vector2	flipped_uv(t_vector2 og);
void		for_all_active_entities(t_world	*world, void	(*func)(t_entity *ent, t_world *world));
void		update_world3d(t_world *world, t_render *render);
t_world		load_world(char *filename, t_sdlcontext *sdl);
void		destroy_entity(t_world *world, t_entity *ent);
t_entity	*spawn_entity(t_world	*world);
t_entity	*spawn_basic_entity(t_world *world, char *objectname, t_vector3 position);
void		save_world(char *filename, t_world world);
void		init_roomwalls(t_world *world, t_room *room);
void		free_floor(t_world *world, t_room *room);

typedef struct s_game
{
	t_world			world;
	t_clock			clock;
	t_hid_info		hid;
	t_player		player;
	t_cam_mode		cam_mode; //Unused but will be reimplemented?
} t_game;

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
t_point vector3_to_screenspace(t_render r, t_vector3 vec, t_sdlcontext sdl);//TODO: vectors.h

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

/* PLAYMODE.C */
int		playmode(t_sdlcontext sdl);

/* PLAYER.C */
void	player_init(t_player *player, t_sdlcontext *sdl);
void	update_render(t_render *render, t_player *player);

/* MOVEPLAYER.C */
void	moveplayer(t_player *player, t_input *input, t_clock clock);

/* ERRORS.C */
void	error_log(int error_code);

/* SDL */
void	quit_game(t_sdlcontext *sdl);

/* FONT.C */

void	load_fonts(t_font *font);

void	draw_black_background(t_sdlcontext *sdl, t_point pos, t_point size);

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
void	update_occlusion(t_sdlcontext sdl, t_render *render);

//settings
void	default_entity_occlusion_settings(t_entity *e, t_world *world);
void	default_floor_occlusion_settings(t_meshtri *f, t_world *world);
void	default_wall_occlusion_settings(t_wall *w, t_world *world);

void	update_entity_bounds(t_entity *e);
void	update_floor_bounds(t_meshtri *f);
void	update_wall_bounds(t_wall *w);


//TODO: temp for occlusion
int32_t		get_id(t_world *world);

//

#endif
