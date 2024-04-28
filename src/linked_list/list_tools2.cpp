/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 12:32:25 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 13:27:38 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	free_list_node(void *content, size_t size)
{
	free(content);
}

//removes match from list
void	list_remove(t_list **head, void *match, size_t content_size)
{
	t_list	*node;
	t_list	*prev;

	node = *head;
	prev = NULL;
	while (node != NULL && match != NULL)
	{
		if (node->content == NULL)
			return ;
		if (ft_memcmp(match, node->content, content_size) == 0)
		{
			if (prev != NULL)
				prev->next = node->next;
			else
				*head = node->next;
			ft_lstdelone(&node, &free_list_node);
			return ;
		}
		prev = node;
		node = node->next;
	}
}
