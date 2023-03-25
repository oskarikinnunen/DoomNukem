/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 20:42:11 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 20:46:31 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static double	get_double_from_string(const char *str)
{
	double	res;
	int		dot_found;
	int		counter;
	int		i;

	res = 0;
	dot_found = 0;
	counter = 0;
	i = 0;
	while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
	{
		if (str[i] != '.')
		{
			if (dot_found == 1)
				counter -= 1;
			res = (res * 10.0) + (str[i] - '0');
		}
		if (str[i] == '.')
			dot_found += 1;
		if (dot_found == 2)
			break ;
		i++;
	}
	return ((res * pow(10.0, (double)counter)));
}

double	ft_atof(const char *str)
{
	double	res;
	int		i;
	int		neg;

	i = 0;
	neg = 1;
	while ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n') && \
			str[i] != '\0')
		i++;
	if (str[i] == '-' && (str[i + 1] > 47 && str[i + 1] < 58))
	{
		neg = neg * -1;
		i++;
	}
	else if (str[i] == '+' && (str[i + 1] > 47 && str[i + 1] < 58))
		i++;
	res = get_double_from_string(&str[i]);
	return ((res * (double)neg));
}
