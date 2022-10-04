/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_saveline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:21:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 11:12:43 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool	removeduplicate(t_editor *ed)
{
	t_list	*node;
	t_list	*prev;
	t_line	line;

	node = ed->linelist;
	prev = NULL;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		if ((v2cmp(ed->line.start, line.start) && v2cmp(ed->line.end, line.end))
			|| (v2cmp(ed->line.end, line.start) && v2cmp(ed->line.start, line.end)))
		{
			if (prev != NULL)
				prev->next = node->next;
			free(node);
			node = NULL;
			return (true);
		}
		prev = node;
		node = node->next;
	}
	return (false);
}

void	saveline(t_editor *ed)
{
	t_list	*node;
	if (ed->linelist == NULL)
		ed->linelist = ft_lstnew(&ed->line, sizeof(t_line));
	else
	{
		if (removeduplicate(ed))
			return ;
		node = ft_lstnew(&ed->line, sizeof(t_line));
		ft_lstapp(&ed->linelist, node);
	}
}