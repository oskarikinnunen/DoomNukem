/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 14:52:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/28 17:55:10 by okinnune         ###   ########.fr       */
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
	void		*tooldata;
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
	char		imagename[256];
	void		(*onclick)(t_editor *ed);
	uint32_t	func_index; //Indexer which is used to get this buttons' 'onclick' function
}	t_guibutton;

typedef struct s_imagedropdown
{
	bool		inprogress;
	t_img		*selected;
}	t_imagedropdown;

typedef struct s_tool_button
{
	t_rectangle	rect;
	t_tool		*(*tool_get)(void);
}	t_tool_button;

typedef enum e_point_tool_state
{
	place_start,
	place_end
}	t_point_tool_state;

void				initialize_buttons(t_list *buttonlist, t_sdlcontext sdl);
void				point_tool_delete(struct s_editor *ed, t_point crd);
t_click_func_def	get_button_func(int	index);
//default button click function, just prints a debug message
void				empty_click_func(t_editor *ed);
t_tool				*get_point_tool(void);
t_tool				*get_entity_tool(void);

#endif