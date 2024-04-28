/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prot_memalloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:48:23 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 11:58:07 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	*prot_memalloc(size_t size)
{
	void	*newPtr;

	newPtr = ft_memalloc(size);
	if (newPtr == NULL)
		doomlog(LOG_EC_MALLOC, NULL);
	return (newPtr);
}
