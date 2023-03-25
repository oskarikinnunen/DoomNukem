/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nav_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 17:45:00 by vlaine            #+#    #+#             */
/*   Updated: 2023/03/25 17:46:03 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	*ft_realloc(void *src, size_t dest_size, size_t src_size)
{
	void	*dest;

	dest = prot_memalloc(dest_size);
	if (!dest)
		return(NULL);
	ft_memcpy(dest, src, src_size);
	free(src);
	return (dest);
}
