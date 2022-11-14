/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 14:52:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/14 12:58:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_TOOLS_H
# define EDITOR_TOOLS_H

# include "shapes.h"
# include "doomnukem.h"

struct	s_editor;
struct	s_sdlcontext;

typedef struct s_tool
{
	void		(*update)(struct s_editor *ed);
	void		(*draw_update)(struct s_editor *ed, struct s_sdlcontext sdl);
	//void		(*cleanup)(struct s_editor *ed);
	void		*tooldata;
	char		icon_name[256];
	t_img		*icon;
}	t_tool;


typedef struct s_click_func_def
{
	char		func_name[256];
	void		(*onclick)(t_editor *ed);
}	t_click_func_def;

typedef struct s_guibutton
{
	t_rectangle	rect;
	t_img		*img;
	char		imagename[256]; //TODO: unused but I'll have to replace all of the buttons if I remove this, I'm lazy
	uint32_t	imageindex;
	void		(*onclick)(t_editor *ed);
	uint32_t	func_index; //Indexer which is used to get this buttons' 'onclick' function
}	t_guibutton;

typedef struct s_imagedropdown
{
	bool		inprogress;
	bool		changed;
	t_point		origin;
	t_guibutton	buttons[3];
	int			scroll;
	uint32_t	selected;
}	t_imagedropdown;

typedef enum e_point_tool_state
{
	place_start,
	place_end
}	t_point_tool_state;

typedef struct s_buttontooldata
{
	t_guibutton		button;
	t_guibutton		*selected;
	t_imagedropdown	dropdown;
}	t_buttontooldata;

void				initialize_buttons(t_list *buttonlist, t_sdlcontext sdl);
void				point_tool_delete(struct s_editor *ed, t_point crd);
t_click_func_def	get_button_func(int	index);
//TODO: rename and write documentation
t_guibutton			*hovered(t_list *buttonlist, t_point mousepos);
void				draw_buttons(t_editor ed, t_sdlcontext sdl); //TODO: move somewhere else so game can use it aswell?
bool				instantbutton(struct s_rectangle rect, struct s_mouse *m, struct s_sdlcontext sdl, char *imgname);
void				empty_click_func(t_editor *ed);
t_tool				*get_wall_tool(void);
t_tool				*get_room_tool(void);
t_tool				*get_entity_tool(void);
t_tool				*get_button_editor_tool();
void				start_imagedropdown(t_point origin, t_imagedropdown *dd);
void				update_imagedropdown(t_editor *ed, t_imagedropdown *dd);
void				draw_imagedropdown(t_sdlcontext sdl, t_imagedropdown dd);
t_vector3			raycast(t_editor *ed);

#endif