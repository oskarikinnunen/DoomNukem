/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_log.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 20:21:24 by raho              #+#    #+#             */
/*   Updated: 2022/11/24 14:10:56 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_LOG_H
# define ERROR_LOG_H

# define EC_SDL_INIT 1
# define EC_SDL_CREATEWINDOW 2
# define EC_SDL_GETWINDOW_SURFACE 3
# define EC_SDL_UPDATEWINDOWSURFACE 4
# define EC_SDL_SETRELATIVEMOUSEMODE 5
# define EC_SDL_GAMECONTROLLEROPEN 6
# define EC_SDL_JOYSTICKINSTANCEID 7
# define EC_SDL_OPENAUDIODEVICE -1
# define EC_OPEN 11
# define EC_CLOSE 12
# define EC_WRITE 13
# define EC_MALLOC 15
# define EC_GETNEXTLINE 16

#endif