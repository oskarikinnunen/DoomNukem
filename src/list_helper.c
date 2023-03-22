/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 12:32:25 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/20 17:22:06 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "doomnukem.h"

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
			printf("found match, removing");
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

//Returns a mallocated and NULL terminated array of list contents.

void	*list_to_ptr(t_list *source, uint32_t *set_length)
{
	t_list	*l;
	int		i;
	void	*result;

	*set_length = 0;
	if (source == NULL || source->content_size == 0)
		return (NULL);
	result = ft_memalloc((ft_listlen(source) + 1) * source->content_size); //assumes the linked list only contains equal sized contents
	if (result == NULL)
		doomlog(LOG_EC_MALLOC, "list_to_ptr");
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
