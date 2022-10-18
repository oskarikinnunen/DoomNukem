/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/18 19:02:55 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

static void	entity_tool_change(t_editor *ed)
{
	ed->tool = get_entity_tool();
	printf("changed to entity tool\n"); //TODO: implement
	return ;
}

static void	entity_tool_click(t_point crd, int mousebtn)
{
	printf("entity tool clicked %i \n", mousebtn);
	return ;
}

static void	entity_tool_key(int key) //make static?
{
	return ;
}

t_tool	*get_entity_tool()
{
	static t_tool	tool
	= {
		NULL, NULL, NULL
	};

	return (&tool);
}
