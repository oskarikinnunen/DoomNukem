/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 12:32:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/26 14:21:43 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "doomnukem.h"

void	list_push(t_list **head, void *content, size_t content_size)
{
	t_list	*node;

	node = ft_lstnew(content, content_size);
	if (node == NULL)
		error_log(EC_MALLOC);
	if (*head == NULL)
		*head = node;
	else
		ft_lstapp(head, node);
}

static void del_list(void *content, size_t size)
{
	free(content);
}

//TODO: unused but might be useful, also make ft_memcmp protected?
void	*list_find(t_list *head, void *match, size_t content_size)
{
	t_list	*node;

	node = head;
	while (node != NULL)
	{
		if (node->content == NULL)
			return (NULL);
		if (ft_memcmp(match, node->content, content_size) == 0)
			return (node->content);
		node = node->next;
	}
	return (NULL);
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
			ft_lstdelone(&node, &del_list);
			return ;
		}
		prev = node;
		node = node->next;
	}
}