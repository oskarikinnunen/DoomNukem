/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/05 18:31:58 by okinnune         ###   ########.fr       */
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
# include "entity.h"
# include "objects.h"
# include "animation.h" //PLAYER USES THIS, MOVE PLAYER TO SEPARATE HEADER
# include "room.h"
# include "npcs.h"
# include "render.h"
# include "player.h"
# include "input.h"
# include "collision.h"
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
# define COLLISION_ON //Comment/uncomment to toggle experimental collision

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

typedef struct s_world
{
	t_clock			clock;
	t_debugconsole	debugconsole;
	t_sdlcontext	*sdl;
	t_list			*guns;
	t_list			*objectmetadatalist; //Move to sdl, this is only used when objects are initialized
	t_npc			npcpool[128];
	t_list			*wall_list;
	t_list			*roomlist;
	t_entitycache	entitycache;
	t_entity		skybox;
}	t_world;

t_vector2	flipped_uv(t_vector2 og);
void		for_all_active_entities(t_world	*world, void	(*func)(t_entity *ent, t_world *world));
void		update_world3d(t_sdlcontext sdl, t_world *world, t_render *render);
t_world		load_world(char *filename, t_sdlcontext *sdl);
void		erase_entity(t_world *world, t_entity *ent);
t_entity	*raise_entity(t_world	*world);
t_entity	*raise_basic_entity(t_world *world, char *objectname, t_vector3 position);
void		save_world(char *filename, t_world world);

typedef struct s_editor
{
	t_world			world;
	t_clock			clock;
	t_hid_info		hid;
	t_player		player;
	t_gamereturn	gamereturn;
	t_render		render;
	struct s_tool	*tool;
}	t_editor;

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
t_gamereturn	editor_events(t_editor *ed);
bool			iskey(SDL_Event e, int keycode);
void			force_mouseunlock(t_hid_info *hid);
void			force_mouselock(t_hid_info *hid);
void			move_editor(t_editor *ed);

/* EDITOR_MOUSE.C */
t_point			mousetoworldspace(t_editor *ed);
t_point			mousetogridspace(t_editor *ed);
t_point			screentogridspace(t_point point);
t_quaternion	transformed_vector3(t_transform transform, t_vector3 v);//TODO: vectors.h
void			mouse_event(SDL_Event e, t_mouse *mouse);//TODO: input.h

/* SPACECONVERSIONS.C */
t_point vector3_to_screenspace(t_render r, t_vector3 vec, t_sdlcontext sdl);//TODO: vectors.h

/* EDITOR_SAVELINE.C */
void	saveline(t_editor *ed);

/* EDITOR_MAP_IO.C */
t_list	*loadmap(char *filename);
void	savemap(t_editor *ed, char *filename);

/* DELTATIME.C */
void	update_deltatime(t_clock *c);

/* EDITOR_BUTTONS.C */
void	draw_editor_buttons(t_sdlcontext sdl, uint8_t tool_selected); //TODO: MOVE TO EDITOR_TOOLS
void	check_tool_change_click(t_point cursor, t_editor *ed); //TODO: MOVE TO EDITOR_TOOLS

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

void	load_fonts(t_sdlcontext *sdl);

void	draw_black_background(t_sdlcontext *sdl, t_point pos, t_point size);

// Fonts: 0 = 11, 1 = 12, 2 = 14, 3 = 16, 4 = 18, 5 = 20, 6 = 22
void	set_font_size(t_sdlcontext *sdl, int font_size);

/* TEXT.C */

// Saves the text to font->texts[i] in sequential order.
// Can be drawn with draw_saved_text()
void	save_text(t_font *font, const char *str);

// Draws a text that has been saved to font->texts[i] with save_text()
void	draw_saved_text(t_sdlcontext *sdl, t_img *text, t_point pos);

// Draws text without saving it anywhere
// Uses a pre-set font size that can be changed by calling set_font_size()
// Fonts: 0 = 11, 1 = 12, 2 = 14, 3 = 16, 4 = 18, 5 = 20, 6 = 22
void	draw_text(t_sdlcontext *sdl, const char *str, t_point pos, t_point boundaries);

// Draws text, with a black box in the background, without saving it anywhere
// Uses a pre-set font size that can be changed by calling set_font_size()
// Fonts: 0 = 11, 1 = 12, 2 = 14, 3 = 16, 4 = 18, 5 = 20, 6 = 22
// Returns the rectangle of the drawed textbox
t_rectangle	draw_text_boxed(t_sdlcontext *sdl, const char *str, t_point pos, t_point boundaries);
t_rectangle	draw_text_black(t_sdlcontext *sdl, const char *str, t_point pos, t_point boundaries);


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

//
bool			object_lookedat(t_editor *ed, t_sdlcontext sdl, t_object *obj);
#endif
