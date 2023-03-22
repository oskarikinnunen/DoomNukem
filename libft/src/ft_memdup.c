/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 20:35:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/17 17:31:34 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_memdup(const void *m, size_t n)
{
	void	*result;

	result = (char *)malloc(n);
	if (result)
		ft_memcpy(result, m, n);
	return (result);
}
