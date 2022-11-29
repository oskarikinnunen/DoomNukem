/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 15:34:55 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/17 16:44:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list	**temp;

	temp = alst;
	while ((*alst) != NULL)
	{
		temp = alst;
		del((*temp)->content, (*temp)->content_size);
		free (*temp);
		*alst = (*alst)->next;
	}
}

//actually deletes the list without the dumb function pointer shenanigans. Calls free for each content
void	listdel(t_list **alst)
{
	t_list	**temp;

	temp = alst;
	while ((*alst) != NULL)
	{
		temp = alst;
		free((*temp)->content);
		free (*temp);
		*alst = (*alst)->next;
	}
}
