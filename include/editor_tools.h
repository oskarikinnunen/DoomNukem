/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 14:52:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 13:59:22 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shapes.h"

struct s_editor;
struct s_sdlcontext;

typedef struct	s_tool
{
	void		(*update)(struct s_editor *ed);
	void		(*draw_update)(struct s_editor *ed, struct s_sdlcontext *sdl);
	//void		(*click_func)(struct s_editor *ed);
	//id		(*key_func)(int);
}	t_tool; //TODO: make static array of these and use test them in editor_mouse


typedef struct	s_tool_button
{
	t_rectangle	rect;
	t_tool	*(*tool_get)(void);
}	t_tool_button;

typedef enum e_point_tool_state
{
	place_start,
	place_end
}	t_point_tool_state;

void	point_tool_delete(struct s_editor *ed, t_point crd); 
t_tool	*get_point_tool();
t_tool	*get_entity_tool();