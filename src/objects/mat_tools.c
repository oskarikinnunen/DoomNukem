/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 12:14:27 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 18:12:15 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"

t_material	*default_mat(void)
{
	t_material	*mat;

	mat = prot_memalloc(sizeof(t_material));
	ft_strncpy_term(mat->texturename, "metal03", 250);
	mat->kd = INT_MAX;
	return (mat);
}
