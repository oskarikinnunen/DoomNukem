/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_listlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 14:59:21 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 15:01:50 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_listlen(t_list *lst)
{
	t_list	*cur;
	int		i;

	i = 0;
	cur = lst;
	i += (cur != NULL);
	while (cur != NULL)
	{
		cur = cur->next;
		i++;
	}
	return (i);
}