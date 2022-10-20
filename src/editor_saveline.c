/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_saveline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:21:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/19 14:44:50 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static void del_list(void *content, size_t size)
{
	free(content);
}

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
		if ((point_cmp(ed->line.start, line.start) && point_cmp(ed->line.end, line.end)) ||
			(point_cmp(ed->line.start, line.end) && point_cmp(ed->line.end, line.start)))
		{
			if (prev)
				prev->next = node->next;
			else
				ed->linelist = node->next;
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
	{
		ed->linelist = ft_lstnew(&ed->line, sizeof(t_line));
		if (!ed->linelist)
			error_log(EC_MALLOC);
	}
	else
	{
		if (ed->linelist != NULL && removeduplicate(ed))
			return ;
		node = ft_lstnew(&ed->line, sizeof(t_line));
		if (!node)
			error_log(EC_MALLOC);
		ft_lstapp(&ed->linelist, node);
	}
}
