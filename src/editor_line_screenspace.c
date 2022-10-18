/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_line_screenspace.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 19:52:32 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/18 20:51:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

t_line	line_in_screenspace(t_editor *ed, t_line line)
{
	t_line	result;

	result.start = point_mul(line.start, GRIDSIZE);
	result.start = point_add(result.start, ed->offset);
	result.end = point_mul(line.end, GRIDSIZE);
	result.end = point_add(result.end, ed->offset);
	return (result);
}