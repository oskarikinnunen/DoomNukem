/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy_term.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 18:44:17 by raho              #+#    #+#             */
/*   Updated: 2023/03/23 15:08:40 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Same as ft_strncpy but stops one char before the num
// and null terminates the string
char	*ft_strncpy_term(char *dest, const char *src, size_t num)
{
	size_t	i;
	size_t	len;

	if (num == 0)
		return (dest);
	i = 0;
	len = (size_t)ft_strlen((char *)src);
	while (i < num - 1)
	{
		if (i > len)
		{
			dest[i] = '\0';
			return (dest);
		}
		else
			dest[i] = src[i];
		i++;
	}
	if (i == (num - 1))
		dest[i] = '\0';
	return (dest);
}
