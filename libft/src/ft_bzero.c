/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 16:06:39 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 13:12:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	while (n > sizeof(long))
	{
		*(long *)s = 0L;
		s = (char *)s + sizeof(long);
		n -= sizeof(long);
	}
	while (n > sizeof(int))
	{
		*(int *)s = 0;
		s = (char *)s + sizeof(int);
		n -= sizeof(int);
	}
	char *ptr = (char*)s;
	while (n-- > 0)
		*ptr++ = 0;
}
