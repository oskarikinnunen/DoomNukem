/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 16:55:51 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 20:48:50 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strreplace(char *str, int c1, int c2)
{
	char	*occurence;

	occurence = ft_strchr(str, c1);
	while (occurence != NULL)
	{
		*occurence = c2;
		occurence = ft_strchr(occurence + 1, c1);
	}
}
