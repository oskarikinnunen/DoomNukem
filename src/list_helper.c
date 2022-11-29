/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 12:32:25 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/18 16:26:22 by okinnune         ###   ########.fr       */
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

t_list	*ptr_to_list(void	*src, uint32_t len, size_t size)
{
	int		i;
	t_list	*head;

	i = 0;
	head = NULL;
	while (i < len)
	{
		void *cur = src + (size * i);
		list_push(&head, cur, size);
		i++;
	}
	return (head);
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
		return (result);
	l = source;
	i = 0;
	
	while (l != NULL)
	{
		if (l->content_size != source->content_size)
		{
			printf("list node %i content size doesn't match with initial lists' content size!\n", i); //TODO: log
			break ;
		}
		ft_memcpy(result + (i * l->content_size), l->content, l->content_size);
		/*printf("content size %lu \n", l->content_size);
		printf("l      %i content %f %f %f \n", i, (*(t_vector3 *)l->content).x, (*(t_vector3 *)l->content).y, (*(t_vector3 *)l->content).z);
		printf("result %i content %f %f %f \n\n", i, ((t_vector3 *)result)[i].x, ((t_vector3 *)result)[i].y, ((t_vector3 *)result)[i].z);*/
		l = l->next;
		i++;
	}
	*set_length = i;
	return (result);
}
