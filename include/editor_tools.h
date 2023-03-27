/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 14:52:30 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 22:43:40 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_TOOLS_H
# define EDITOR_TOOLS_H

# include "shapes.h"
# include "render.h"
# include "doomnukem.h"
# include "collision.h"

# define TITLE_BAR_HEIGHT 32

enum	e_load_arg;
struct	s_mouse;

typedef enum e_point_tool_state
{
	place_start,
	place_end
}	t_point_tool_state;

typedef enum e_autogui_layout
{
	agl_vertical,
	agl_horizontal
}	t_autogui_layout;

typedef struct s_button_return
{
	bool		clicked;
	t_rectangle	rect;
}	t_button_return;

typedef struct s_autogui
{
	t_rectangle			rect;
	t_point				minimum_size;
	t_point				offset;
	t_point				scroll;
	t_hid_info			*hid;
	char				*title;
	int					min_x;
	int					overdraw;
	int					x_max_drawn;
	t_sdlcontext		*sdl;
	t_player			*player;
	t_autogui_layout	agl;
	struct s_autogui	*dock;
	bool				allow_user_hide;
	bool				hidden;
	bool				locked;
	bool				scrollable;
	bool				locking_player;
	bool				scroll_held;
	bool				move_held;
	bool				drag_held;
}	t_autogui;

typedef struct s_editor_prefs
{
	char		world_name[32];
	t_vector3	player_pos;
}	t_editor_prefs;

typedef struct s_editor
{
	t_world				world;
	t_autogui			toolbar_gui;
	t_autogui			graphics_gui;
	t_hid_info			hid;
	t_player			player;
	bool				ceiling_toggle;
	struct s_tool		*tool;
}	t_editor;

typedef struct s_tool
{
	void	(*init)(t_editor *ed, t_sdlcontext *sdl);
	void	(*update)(t_editor *ed, t_sdlcontext *sdl);
	void	(*cleanup)(t_editor *ed, t_sdlcontext *sdl);
	void	*tool_data;
	char	icon_name[256];
	t_img	*icon;
}	t_tool;

typedef struct s_editor_raycast
{
	t_raycast_info	internal_info;
	t_entity		*ent;
	int				found;
	bool			hit;
	int				i;
}	t_editor_raycast;

typedef struct s_editor_late_gui
{
	t_graphic_prefs	prefs;
	t_point			*resolutions;
	char			*str;
	t_point			res;
}	t_editor_late_gui;

typedef struct s_gui_init
{
	t_sdlcontext	*sdl;
	t_hid_info		*hid;
	t_player		*player;
	t_point			origin;
	char			*title;
}	t_gui_init;

//struct	s_sdlcontext;

//struct	s_editor;

void			editor_quit(void);
void			editor_save_quit(t_editor *ed);
void			set_up_editor(t_sdlcontext *sdl, t_editor *ed);

/* INTERNAL GUI */
t_button_return	autogui_internal_colored_button(char *str, t_autogui *gui, \
												uint32_t color);
t_button_return	autogui_internal_button(char *str, t_autogui *gui);
bool			gui_int_slider_internal(int *i, float mul, t_rectangle rect, \
										t_autogui *gui);
void			gui_layout(t_autogui *gui, t_rectangle rect);
t_point			gui_current_pos(t_autogui *gui);
t_rectangle		empty_rect(void);
void			gui_limit_rect(t_autogui *gui);
bool			clicked_rectangle(t_mouse m, t_rectangle rect);
void			update_hidecross(t_autogui *gui);
void			update_scrollbar(t_autogui *gui);
void			gui_layout_big(t_autogui *gui, t_rectangle rect);
bool			gui_should_draw(t_autogui *gui);
void			gui_lock_mouse(t_autogui *gui);

void			update_editor_toolbar(t_editor *ed, t_autogui *toolbar);
void			editor_events(t_editor *ed);
void			editor_world_setup(t_editor *ed, char *world_name, \
									t_sdlcontext *sdl, enum e_load_arg args);

/* EDITORPREFERENCES.C */
void			editor_save_prefs(t_editor *ed);

bool			object_looked_at(t_editor *ed, t_sdlcontext sdl, \
								t_object *obj);

//Call this only once, returns a gui with correctly initialized values
t_autogui		init_gui(t_gui_init gui_init);
//Tells the gui to start, you can call gui drawing functions after this
void			gui_start(t_autogui *gui);
//Tells the gui to end, 
//do not call any gui drawing functions after calling gui_end
void			gui_end(t_autogui *gui);

//Tells the gui to start laying out future blocks horizontally
void			gui_start_horizontal(t_autogui *gui);
//Tells the gui to end laying out blocks horizontally
void			gui_end_horizontal(t_autogui *gui);

//Gui preset for modifying a transform
void			gui_preset_transform(t_transform *t, t_autogui *gui);
//Gui preset for modifying a transforms scale and rotation
void			gui_preset_scale_and_rotate(t_transform *t, t_autogui *gui);

//Draws an empty vertical block on the gui
void			gui_empty_vertical(int y, t_autogui *gui);
//Draws a label on the gui
void			gui_label(char *str, t_autogui *gui);
//Draws an integer on the gui
void			gui_int(int i, t_autogui *gui);
//Draws a t_point on the gui
void			gui_point(t_point point, t_autogui *gui);
//Draws a button with text 'str' on the gui,
//and returns true if the button was pressed
bool			gui_button(char *str, t_autogui *gui);
bool			gui_shortcut_button(char *str, int alpha_or_keymask, \
									t_autogui *gui);
bool			gui_image_button(t_img	*img, t_autogui *gui);
bool			gui_highlighted_button(char *str, t_autogui *gui);
bool			gui_highlighted_button_if(char *str, t_autogui *gui, \
											bool condition);

//TODO: document
void			gui_string_edit(char *str, t_autogui	*gui);
//TODO: document
bool			gui_bool_edit(bool *b, t_autogui *gui);

void			gui_labeled_bool(char *str, bool b, t_autogui *gui);
//Draws an integer slider which allows modifying the integers value,
//returns true if the integer changed
bool			gui_int_slider(int *i, float mul, t_autogui *gui);
//Draws a float slider which allows modifying the floats value,
//returns true if the float changed
bool			gui_float_slider(float	*f, float mul, t_autogui *gui);

//TODO: document
bool			gui_labeled_bool_edit(char *str, bool *b, t_autogui *gui);
//Draws a label and an integer next to eachother
void			gui_labeled_int(char *str, int i, t_autogui *gui);
//Draws a label and a point next to eachother
void			gui_labeled_point(char *str, t_point point, t_autogui *gui);
//Draws a label and a modifiable float slider next to eachother
bool			gui_labeled_float_slider(char *str, float *f, float mul, \
										t_autogui *gui);
//Draws a label and a modifiable int slider next to eachother
bool			gui_labeled_int_slider(char *str, int *i, float mul, \
										t_autogui *gui);
//Draws a label and a modifiable vector slider next to eachother
void			gui_labeled_vector3_slider(char *str, t_vector3 *vec, \
											float mul, t_autogui *gui);

bool			gui_hoverlabel(char *str, t_autogui *gui);

void			gui_autosize(t_autogui *gui);
//TODO: rename and write documentation
//TODO: move somewhere else so game can use it aswell?
void			update_editor_buttons(t_editor *ed, t_sdlcontext *sdl);
void			empty_click_func(t_editor *ed);
t_tool			*get_gun_tool(void);
t_tool			*get_room_tool(void);
t_tool			*get_entity_tool(void);
t_tool			*get_npc_tool(void);
t_tool			*get_load_tool(void);
void			player_update_gun(t_player *player, t_world *world);
void			render_grid(t_world *world, t_vector3 position, int size, \
							uint32_t color);
bool			rooms_share_zspace(t_area *room1, t_area *room2);
bool			entity_looked_at(t_editor *ed, t_sdlcontext sdl, \
								t_entity *entity);
bool			triangle_looked_at(t_render r, t_world_triangle tri, \
									t_sdlcontext sdl);
int32_t			entity_looked_at_triangle_index(t_editor *ed, \
									t_sdlcontext sdl, t_entity *entity);
t_vector3		*entity_looked_at_vertex(t_editor *ed, t_sdlcontext sdl, \
										t_entity *entity);
t_entity		*selected_entity(t_editor *ed, t_sdlcontext sdl);

void			room_make_floor(t_world *world, t_area *room);
void			room_make_ceilings(t_world *world, t_area *room);

/* TOOL_COMMON_FUNCTIONS.C */
t_object		*object_selector2(t_editor *ed, t_sdlcontext sdl, \
									t_entity *original);

#endif