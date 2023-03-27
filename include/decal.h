/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decal.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:21:13 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 18:07:45 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECAL_H
# define DECAL_H

# include "entity.h"

typedef struct s_decal
{
	t_img		*img;
	float		size;
	t_vector3	position;
	t_vector3	normal;
}	t_decal;

void	decal(struct s_world *world, t_decal decal);

#endif