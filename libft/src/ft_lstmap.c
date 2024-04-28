/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:51:09 by okinnune          #+#    #+#             */
/*   Updated: 2021/12/16 19:28:53 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	deleteContent(void *content, size_t size)
{
	if (size > 0)
		free(content);
}

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*newList;
	t_list	*temp;

	newList = NULL;
	while (lst != NULL)
	{
		temp = f(lst);
		if (temp == NULL)
		{
			if (newList != NULL)
				ft_lstdel(&newList, deleteContent);
			return (NULL);
		}
		if (newList == NULL)
			newList = temp;
		else
			ft_lstapp(&newList, temp);
		lst = lst->next;
	}
	return (newList);
}
