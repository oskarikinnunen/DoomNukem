/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:28:19 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 18:07:53 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_UTILS_H
# define RENDER_UTILS_H

# include "vectors.h"

typedef struct s_camera
{
	t_mat4x4	mat_view;
	t_mat4x4	mat_proj;
	t_vector3	position;
	t_vector3	look_dir;
	float		aspect_ratio;
}	t_camera;

#endif