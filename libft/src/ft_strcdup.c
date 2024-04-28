/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:00:00 by raho              #+#    #+#             */
/*   Updated: 2022/10/31 22:00:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Duplicates a given string up until the first encounter of the given
** character. Returns a pointer to the newly created string.
*/

char	*ft_strcdup(const char *str, char c)
{
	int		index;
	char	*newStr;

	index = 0;
	while (str[index] != '\0' && str[index] != c)
		index++;
	newStr = (char *)malloc(sizeof(char) * (index + 1));
	if (newStr == NULL)
		return (NULL);
	index = 0;
	while (str[index] != '\0' && str[index] != c)
	{
		newStr[index] = str[index];
		index++;
	}
	newStr[index] = '\0';
	return (newStr);
}
