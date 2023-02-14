/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 20:35:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/09 13:42:33 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_memdup(const void *m, size_t n)
{
	void	*result;
	size_t	i;

	i = 0;
	result = (char *)malloc(n);
	ft_memcpy(result, m, n);
	/*if (result != NULL)
	{
		while (i < n)
		{
			((unsigned char *)result)[i] = ((unsigned char *)m)[i];
			i++;
		}
	}*/
	return (result);
}
