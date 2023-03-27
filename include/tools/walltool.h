/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walltool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:17:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/27 11:03:03 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WALLTOOL_H
# define WALLTOOL_H

# include "room.h"

# define MAXSELECTED 32

struct	s_sdlcontext;
struct	s_editor;
struct	s_room_tool_data;
struct	s_room;

typedef enum e_wall_tool_mode
{
	place_first,
	place_other,
	place_height
}	t_wall_tool_mode;

typedef struct s_ear
{
	t_vector2	first;
	t_vector2	center;
	t_vector2	second;
}	t_ear;

typedef struct s_floorcalc
{
	t_vector2	edges[1024];
	t_face		faces[1024];
	t_line		normals[1024];
	uint8_t		edge_count;
	uint8_t		face_count;
}	t_floorcalc;

typedef struct s_wall_tool_data
{
	t_wall				wall;
	t_wall				*hover;
	t_wall				*selected[MAXSELECTED];
	t_wall_tool_mode	mode;
	t_floorcalc			fc;
	uint8_t				fc_debug;
}	t_wall_tool_data;

void		make_floor(struct s_editor *ed, struct s_room_tool_data dat, \
						struct s_sdlcontext *sdl);

#endif