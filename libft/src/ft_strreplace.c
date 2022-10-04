/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 16:55:51 by okinnune          #+#    #+#             */
/*   Updated: 2022/04/25 19:59:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strreplace (char *str, int c1, int c2)
{
	char	*occurence;

	occurence = ft_strchr(str, c1);
	while (occurence != NULL)
	{
		*occurence = c2;
		occurence = ft_strchr(occurence + 1, c1);
	}
}
