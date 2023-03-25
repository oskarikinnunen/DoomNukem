/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 15:34:55 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 20:52:16 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list	**temp;
	t_list	*next;

	temp = alst;
	while ((*alst) != NULL)
	{
		next = (*alst)->next;
		temp = alst;
		del((*temp)->content, (*temp)->content_size);
		free (*temp);
		*alst = next;
	}
}

// Actually deletes the list without the dumb function pointer shenanigans
// Calls free for each content
void	listdel(t_list **alst)
{
	t_list	**temp;
	t_list	*next;

	temp = alst;
	while ((*alst) != NULL)
	{
		next = (*alst)->next;
		temp = alst;
		free((*temp)->content);
		free (*temp);
		*alst = next;
	}
}
