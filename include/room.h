/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:20:36 by raho              #+#    #+#             */
/*   Updated: 2023/03/27 12:49:30 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOM_H
# define ROOM_H

# include "entity.h"
# include "render.h"
# include "gamestring.h"

struct	s_world;

typedef struct s_meshtri
{
	t_entity	*entity;
	t_face		face;
	t_vector3	v[3];
	t_vector2	uv[3];
}	t_meshtri;

typedef struct s_edge_line
{
	uint32_t	start_index;
	uint32_t	end_index;
	t_vector2	*start;
	t_vector2	*end;
}	t_edge_line;

typedef struct s_wall
{
	t_entity		*entity;
	t_edge_line		edge_line;
	uint32_t		z_offset;
	uint32_t		height;
	bool			ceiling_wall;
	bool			connection;
	t_game_string	s_walltex;
	t_vector2		uv_offset;
	float			uv_scale;
}	t_wall;

typedef struct s_area_status
{
	bool	is_floor;
	bool	is_exterior;
	bool	is_room;
}	t_area_status;

typedef struct s_room
{
	char			name[32];
	t_vector2		edges[32];
	t_wall			walls[32];
	t_meshtri		floors[32];
	t_meshtri		ceilings[32];
	t_meshtri		ramps[32];
	t_game_string	s_ceil_tex;
	t_game_string	s_floor_tex;
	bool			walls_enabled;
	bool			floor_enabled;
	bool			ceiling_enabled;
	bool			loop;
	uint32_t		height;
	uint32_t		ceiling_height;
	uint32_t		edge_count;
	uint32_t		wall_count;
	uint32_t		floor_count;
	uint32_t		ceiling_count;
	uint32_t		ramp_count;
}	t_area;

bool			room_edge_is_legal(t_vector2 *edge, t_area *room);
void			room_recalculate_joined_rooms(struct s_world *world, \
												t_area *room);
void			room_init(t_area *room, struct s_world *world);
void			room_init_shallow(t_area *room, struct s_world *world);
bool			room_is_legal(struct s_world *world, t_area *room);
void			room_remove_entities(t_area *room, struct s_world *world);
bool			edge_exists(t_vector2 edge, t_area	*room);
void			room_make_floor(struct s_world *world, t_area *room);
t_area			*world_add_room(struct s_world *world, t_area *room);
void			world_remove_room(struct s_world *world, t_area *room);
void			room_set_preset_floor(t_area *room);
void			room_set_preset_exterior(t_area *room);
void			room_set_preset_room(t_area *room);
void			apply_wallmesh(t_wall *wall, t_area *room, \
								struct s_world *world);
void			apply_trimesh(t_meshtri tri, t_object *obj);
void			walls_init(struct s_world *world, struct s_sdlcontext *sdl);

#endif