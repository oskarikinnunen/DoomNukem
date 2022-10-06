/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine3d.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 17:10:21 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/06 17:36:01 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

typedef struct s_vec3d
{
	int x;
	int y;
	int z;
	int w;
}	t_vec3d;

typedef struct	s_triangle
{
	struct s_vec3d p[3];
}	t_triangle;

