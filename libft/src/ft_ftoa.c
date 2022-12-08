/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:53:37 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/07 12:36:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>


char	*ft_ftoa(float f, int dec)
{
	char	*str;

	str = ft_strnew(32);
	if (f >= 0.01f)
		snprintf(str, 32, "%.2f", f);
	else
		snprintf(str, 32, "%.6f", f);
	return (str);
	/*char	*final;
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
	final = ft_strnew(ft_strlen(strinteger) + ft_strlen(strdecimal) + 1 + neg);
	if (neg)
		ft_strcat(final, "-");
	ft_strcat(final, strinteger);
	ft_strcat(final, ".");
	ft_strcat(final, strdecimal);
	free(strinteger);
	free(strdecimal);
	return (final);*/
}
