/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 23:33:34 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/17 23:52:06 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void point_tool_click(t_point crd, int mousebtn)
{
	printf("btn %i \n", mousebtn);
	return ;
}

void point_tool_key(int key)
{
	return ;
}

void entity_tool_click(t_point crd, int mousebtn)
{
	return ;
}

void entity_tool_key(int key)
{
	return ;
}

t_tool *get_tools()
{
	static	t_tool tools[2] = 
	{
		{
		point_tool_click, point_tool_key
		},
		{
		entity_tool_click, entity_tool_key
		}
	};
	
	tools[0].click_func(point_zero(), 42);
	return (tools);
};