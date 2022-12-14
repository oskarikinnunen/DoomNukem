/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flerp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 13:42:41 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/24 13:49:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float ft_flerp(float from, float to, float delta)
{
	return (from + ((to - from) * delta));
}
