/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_new_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:34:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/28 19:01:17 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

typedef void	(*t_button_click_fptr)(t_editor *ed);

void	empty_click_func(t_editor *ed)
{
	printf("button has undefined click function! \n");
}

#include "vectors.h"

typedef struct s_test
{
	t_vector3 p[3];
	t_vector3 uv[3];
}	t_test;

void	debug_click_func(t_editor *ed)
{
	printf("button has DEBUG click function! \n");
}

#include "assert.h"

t_click_func_def	get_button_func(int	index)
{
	static	t_click_func_def functions[256] =
	{
		{"empty_click", empty_click_func},
		{"DEBUG_click", debug_click_func}
	};
	int	i;

	i = 0;
	while (i < 256)
	{
		if (functions[i].onclick == NULL)
			break ;
		if (i == index)
			return (functions[i]);
		i++;
	}
	return (functions[0]);
}

/*static t_button_click_fptr	get_button_func(int	index)
{
	static	t_button_click_fptr functions[256] =
	{
		empty_click_func
	};
	int	i;

	i = 0;
	while (i < 256)
	{
		if (functions[i] == NULL)
		{
			printf("last button func was %i \n", i);
			break ;
		}
		i++;
	}
	return (functions[0]);
}*/

void	initialize_buttons(t_list *buttonlist, t_sdlcontext sdl)
{
	t_list		*l;
	t_guibutton	*button;

	l = buttonlist;
	while (l != NULL)
	{
		button = (t_guibutton *)l->content;
		button->img = get_image_by_name(sdl, button->imagename);
		//button->onclick = ;
		//button->onclick = &point_tool_test;
		//button->func_index = 10;
		button->onclick = get_button_func(0).onclick;
		l = l->next;
	}
}
