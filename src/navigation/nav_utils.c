/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nav_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:45:00 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/25 22:15:39 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	*ft_realloc(void *src, size_t dest_size, size_t src_size)
{
	void	*dest;

	dest = ft_memalloc(dest_size);
	if (!dest)
		return (NULL);
	ft_memcpy(dest, src, src_size);
	free(src);
	return (dest);
}
