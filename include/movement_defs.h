/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_defs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:56:24 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:56:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVEMENT_DEFS_H
# define MOVEMENT_DEFS_H

# define PLAYERRADIUS 16

//Global gravity for all capsulephysics entities
# define GRAVITY -0.66f

# define EDITOR_FLYSPEED 0.25f
# define EDITOR_FLYFASTSPEED 0.65f

# define PLAYER_WALKSPEED 0.1f
# define PLAYER_RUNSPEED 0.2f
# define PLAYER_ACCELERATION 0.1f
# define PLAYER_DECELERATION 0.01f
# define PLAYER_HEIGHT 50.0f
# define PLAYER_CROUCHHEIGHT 25.0f

//How big of an step can the player step over
# define COL_STEP 15.0f

//The speed player goes to crouch
# define PLAYER_CROUCHSPEED 0.1f
# define JUMP_DELAY 400

//# define MAXMOVEMENTSPEED 0.08f
# define ROTATESPEED 0.002f
# define MOUSESPEED 0.002f

void	player_update_physics(t_player *player, t_world *world);
void	player_rotate(t_player *player);
void	player_move(t_player *player, t_world *world);
void	crouch_update(t_player *player, t_world *world);
void	player_jump(t_player *player, t_world *world);
void	player_update_animations(t_player *player, t_world *world);
void	player_ceiling_check(t_player *player);

#endif