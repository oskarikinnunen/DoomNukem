/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:53:37 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 17:10:21 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//TODO: does this need to be more accurate?
char	*ft_ftoa(float f, int dec)
{
	char	*final;
	char	*strinteger;
	char	*strdecimal;
	int		neg;
	int		i;

	i = (int)f;
	f = f - i;
	f = f * (ft_pow(10, dec));
	neg = (f < 0.0f);
	strinteger = ft_itoa(ft_abs(i));
	strdecimal = ft_itoa(ft_abs((int)f));
	if (strinteger == NULL || strdecimal == NULL)
		return (NULL);
	final = ft_strnew(ft_strlen(strinteger) + ft_strlen(strdecimal) + neg + 2);
	if (final == NULL)
		return (NULL);
	if (neg)
		ft_strcat(final, "-");
	ft_strcat(final, strinteger);
	ft_strcat(final, ".");
	ft_strcat(final, strdecimal);
	free(strinteger);
	free(strdecimal);
	return (final);
}
