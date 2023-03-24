/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 20:52:15 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 20:59:33 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "libft.h"

char	*s_pitoa(t_point p)
{
	static char	str[64];
	char		*a;

	a = ft_itoa(p.x);
	ft_strncpy_term(str, a, 30);
	free(a);
	a = ft_itoa(p.y);
	ft_strcat(str, " ");
	ft_strncat(str, a, 30);
	free(a);
	return (str);
}

char	*s_itoa(int i)
{
	static char	str[32];
	char		*a;

	a = ft_itoa(i);
	ft_strncpy_term(str, a, 30);
	free(a);
	return (str);
}
