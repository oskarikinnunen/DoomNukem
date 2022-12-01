/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 14:52:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/01 16:58:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_TOOLS_H
# define EDITOR_TOOLS_H

# include "shapes.h"
# include "doomnukem.h"

typedef struct s_tool
{
	void		(*draw_update)(t_editor *ed, t_sdlcontext sdl);
	void		(*cleanup)(t_editor *ed, t_sdlcontext sdl);
	void		*tooldata;
	char		icon_name[256];
	t_img		*icon;
}	t_tool;


typedef enum e_point_tool_state
{
	place_start,
	place_end
}	t_point_tool_state;

typedef	enum e_autoguilayout
{
	agl_vertical,
	agl_horizontal
}	t_autoguilayout;

typedef struct s_buttonreturn
{
	bool		clicked;
	t_rectangle	rect;
}	t_buttonreturn;

typedef struct s_autogui
{
	t_rectangle			rect;
	t_point				offset;
	t_point				scroll;
	t_hid_info			*hid;
	char				*title;
	int					min_x;
	int					overdraw;
	t_sdlcontext		*sdl;
	t_player			*player;
	t_autoguilayout		agl;
	struct	s_autogui	*dock;
	bool				allow_user_hide;
	bool				hidden;
	bool				scrollable;
	bool				locking_player;
	bool				scroll_held;
	bool				move_held;
	bool				drag_held;
}	t_autogui;

struct	s_sdlcontext;
struct	s_mouse;
struct	s_editor;

void				gui_start(t_autogui *gui);
void				gui_starthorizontal(t_autogui *gui);
void				gui_endhorizontal(t_autogui *gui);
void				gui_update(t_autogui *gui);
void				gui_int_slider(int *i, int mul, t_autogui *gui);
void				gui_float_slider(float	*f, float mul, t_autogui *gui);
void				gui_labeled_vector3_slider(char *str, t_vector3 *vec, float mul, t_autogui *gui);
void				gui_vector3_slider(t_vector3 *vec, float mul, t_autogui *gui);
void				gui_labeled_float_slider(char *str, float *f, float mul, t_autogui *gui);
void				gui_label(char *str, t_autogui *gui);
bool				gui_button(char *str, t_autogui *gui);
t_autogui			init_gui(t_sdlcontext *sdl, t_hid_info *hid, t_player *player, t_point origin, char *title);
void				point_tool_delete(struct s_editor *ed, t_point crd);
//TODO: rename and write documentation
void				update_editor_buttons(t_editor *ed, t_sdlcontext *sdl); //TODO: move somewhere else so game can use it aswell?
bool				instantbutton(struct s_rectangle rect, struct s_mouse *m, struct s_sdlcontext sdl, char *imgname);
int					object_selector(t_editor *ed, struct s_sdlcontext sdl, int	original);
// Combines draw_text_boxed and instantbutton to make a button out of the text
bool				instant_text_button(t_sdlcontext sdl, t_mouse *m, const char *str, t_point pos);
void				empty_click_func(t_editor *ed);
t_tool				*get_gun_tool();
t_tool				*get_room_tool(void);
t_tool				*get_entity_tool(void);
t_tool				*get_npc_tool(void);
t_vector3			raycast(t_editor *ed);
bool				entity_lookedat(t_editor *ed, t_sdlcontext sdl, t_entity *entity);
bool				triangle_lookedat(t_render r, t_triangle tri, t_sdlcontext sdl);
int32_t				entity_lookedat_triangle_index(t_editor *ed, t_sdlcontext sdl, t_entity *entity);
t_vector3			*entity_lookedat_vertex(t_editor *ed, t_sdlcontext sdl, t_entity *entity);
t_entity			*selected_entity(t_editor *ed, t_sdlcontext sdl);

/* TOOL_COMMON_FUNCTIONS.C */
void				string_box(char *name, char *str, t_editor *ed, t_sdlcontext sdl, t_point pos);
void				bool_toggle(char *name, bool *b, t_editor *ed, t_sdlcontext sdl, t_point pos);
void				vector3_slider(char *name, t_vector3 *vec, t_editor *ed, t_sdlcontext sdl, t_point pos);
void				float_slider(float *f, float mul, t_editor *ed, t_sdlcontext sdl, t_point pos);
void				float_slider_name(char *name, float *f, float mul, t_editor *ed, t_sdlcontext sdl, t_point pos);
void				int_slider_name(char *name, int *i, int mul, t_editor *ed, t_sdlcontext sdl, t_point pos);
t_object			*object_selector2(t_editor *ed, t_sdlcontext sdl, t_entity *original);

#endif