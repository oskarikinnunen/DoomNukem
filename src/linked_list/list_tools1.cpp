/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 13:26:05 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 15:56:23 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	*list_findlast(t_list *head)
{
	t_list	*l;

	l = head;
	while (l->next != NULL)
	{
		l = l->next;
	}
	return (l->content);
}

void	*list_find(t_list *head, void *match, size_t content_size)
{
	t_list	*node;

	node = head;
	while (node != NULL)
	{
		if (node->content == NULL || match == NULL)
			return (NULL);
		if (ft_memcmp(match, node->content, content_size) == 0)
			return (node->content);
		node = node->next;
	}
	return (NULL);
}

void	list_push(t_list **head, void *content, size_t content_size)
{
	t_list	*node;

	node = ft_lstnew(content, content_size);
	if (node == NULL)
		doomlog(LOG_EC_MALLOC, "list_push");
	if (*head == NULL)
		*head = node;
	else
		ft_lstapp(head, node);
}

//Returns a mallocated and NULL terminated array of list contents
void	*list_to_ptr(t_list *source, uint32_t *set_length)
{
	t_list	*l;
	int		i;
	void	*result;

	*set_length = 0;
	if (source == NULL || source->content_size == 0)
		return (NULL);
	result = prot_memalloc((ft_listlen(source) + 1) * source->content_size);
	l = source;
	i = 0;
	while (l != NULL)
	{
		if (l->content_size != source->content_size)
		{
			doomlog_mul(LOG_FATAL, (char *[4]){\
				"list node", s_itoa(i), \
				"content size doesn't match with initial lists' content size!", \
				NULL});
		}
		ft_memcpy(result + (i * l->content_size), l->content, l->content_size);
		l = l->next;
		i++;
	}
	*set_length = i;
	return (result);
}
