/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_saveline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:21:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 10:52:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

bool	removeduplicate(t_editor *ed)
{
	t_list	*node;
	t_list	*prev;
	t_line	line;

	node = ed->linelist;
	while (node != NULL)
	{
		line = *(t_line *)node->content;
		if (v2cmp(ed->line.start, line.start)) {
			/*if (node->next != NULL)
				prev->next = node->next;
			free(node->content);
			free(node);*/
			prev->next = NULL;
			free(node);
			//ft_memdel((void **)&node);
			return (true);
		}
		node = node->next;
		prev = node;
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