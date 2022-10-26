/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_tool_delete.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 21:22:18 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/26 14:31:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

//Use 
void	point_tool_delete(t_editor *ed, t_point crd)
{
	t_list	*node;
	t_line	line;

	node = ed->linelist;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		if (node->content == NULL)
			return ;
		if ((point_cmp(crd, line.start)
			|| point_cmp(crd, line.end)))
		{
			list_remove(&ed->linelist, &line, sizeof(t_line));
			return ;
		}
		node = node->next;
	}
}