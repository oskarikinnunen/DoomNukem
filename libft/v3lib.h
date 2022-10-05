/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3lib.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:57:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/05 11:29:26 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef V3LIB_H
# define V3LIB_H

# include "libft.h"

# define X 0
# define Y 1
# define Z 2

void	v3_mul(float mul, float *v3);
void	v3_matrix_mul(float matrix[3][3], float *v3);
void	fv3_to_iv3(float *f3, int *i3);
void	v3_add(float *v3, float *add);

#endif