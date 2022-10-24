/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_tool_delete.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 21:22:18 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/18 22:06:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"


static void del_list(void *content, size_t size)
{
	free(content);
}

//Use 
void	point_tool_delete(t_editor *ed, t_point crd)
{
	t_list	*node;
	t_list	*prev;
	t_line	line;

	node = ed->linelist;
	prev = NULL;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		if (node->content == NULL)
			return ;
		if ((point_cmp(crd, line.start)
			|| point_cmp(crd, line.end)))
		{
			if (prev)
				prev->next = node->next;
			else
				ed->linelist = node->next;
			ft_lstdelone(&node, &del_list);
			return ;
		}
		prev = node;
		node = node->next;
	}
}