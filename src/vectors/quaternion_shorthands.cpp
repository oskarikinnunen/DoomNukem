/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion_shorthands.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:27:29 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 11:46:59 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

t_quaternion	quaternion_identity(void)
{
	return ((t_quaternion){1.0f, 1.0f, 1.0f, 1.0f});
}
