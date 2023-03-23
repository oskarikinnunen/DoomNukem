/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prot_memalloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:48:23 by raho              #+#    #+#             */
/*   Updated: 2023/03/23 17:12:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	*prot_memalloc(size_t size)
{
	void	*new;

	new = ft_memalloc(size);
	if (new == NULL && size != 0)
		doomlog(LOG_EC_MALLOC, NULL);
	return (new);
}
