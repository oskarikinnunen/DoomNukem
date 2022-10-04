/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_saveline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:21:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 11:36:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void del_list(void *content, size_t size)
{
	free(content);
}

//TODO: fix this, this isn't able to remove the first one, program segfaults :D
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
		if (node->content == NULL)
			return (true);
		if ((v2cmp(ed->line.start, line.start) && v2cmp(ed->line.end, line.end)) ||
			(v2cmp(ed->line.start, line.end) && v2cmp(ed->line.end, line.start)))
		{
			if (prev != NULL)
				prev->next = node->next;
			ft_lstdelone(&node, &del_list);
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
		if (ed->linelist != NULL && removeduplicate(ed))
			return ;
		node = ft_lstnew(&ed->line, sizeof(t_line));
		ft_lstapp(&ed->linelist, node);
	}
}