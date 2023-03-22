/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 14:07:10 by raho              #+#    #+#             */
/*   Updated: 2023/03/16 19:05:46 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memjoin(const void *dst, const void *src,
						size_t dst_len, size_t src_len)
{
	void	*res;

	res = malloc(dst_len + src_len);
	if (res != NULL)
	{
		ft_memcpy(res, dst, dst_len);
		ft_memcpy(res + dst_len, src, src_len);
	}
	return (res);
}
