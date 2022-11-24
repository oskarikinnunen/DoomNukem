/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_min_max.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 02:41:42 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/11 09:07:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float	ft_minf(float f1, float f2)
{
	if (f1 < f2)
		return (f1);
	return (f2);
}

float	ft_maxf(float f1, float f2)
{
	if (f1 > f2)
		return (f1);
	return (f2);
}

int	ft_min(int i1, int i2)
{
	if (i1 < i2)
		return (i1);
	return (i2);
}

int	ft_max(int i1, int i2)
{
	if (i1 > i2)
		return (i1);
	return (i2);
}
