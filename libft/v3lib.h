/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3lib.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:57:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/04/28 03:20:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef V3LIB_H
# define V3LIB_H

# include "libft.h"

# define X 0
# define Y 1
# define Z 2

void	v3_mul(float matrix[3][3], float *v3);
void	v3_add(float *v3, float *add);

#endif