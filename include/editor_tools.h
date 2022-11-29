/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 14:52:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/29 15:42:50 by okinnune         ###   ########.fr       */
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

struct	s_sdlcontext;
struct	s_mouse;
struct	s_editor;

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

#endif