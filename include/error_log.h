/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_log.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 20:21:24 by raho              #+#    #+#             */
/*   Updated: 2022/12/27 10:51:36 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_LOG_H
# define ERROR_LOG_H

# define EC_SDL_INIT 1
# define EC_SDL_CREATEWINDOW 2
# define EC_SDL_GETWINDOW_SURFACE 3
# define EC_SDL_CREATERGBSURFACE -4
# define EC_SDL_UPDATEWINDOWSURFACE 4
# define EC_SDL_SETRELATIVEMOUSEMODE 5
# define EC_SDL_GAMECONTROLLEROPEN 6
# define EC_SDL_JOYSTICKINSTANCEID 7
# define EC_SDL_OPENAUDIODEVICE -1
# define EC_SDL_LOADWAV -2
# define EC_SDL_QUEUEAUDIO -3
# define EC_TTF_OPENFONT 9
# define EC_TTF_RENDERTEXTBLENDED 10
# define EC_OPEN 11
# define EC_CLOSE 12
# define EC_WRITE 13
# define EC_MALLOC 15
# define EC_GETNEXTLINE 16
# define EC_FMOD_SYSTEMCREATE 40
# define EC_FMOD_SYSTEMINIT 41
# define EC_FMOD_SYSTEMCLOSE 42
# define EC_FMOD_SYSTEMRELEASE 43
# define EC_FMOD_SYSTEMCREATESOUND 44
# define EC_FMOD_SYSTEMPLAYSOUND 45
# define EC_FMOD_CHANNELSETVOLUME 46
# define EC_FMOD_CHANNELSETPAUSED 47
# define EC_FMOD_CHANNELSET3DATTRIBUTES 48
# define EC_FMOD_CHANNELISPLAYING 49
# define EC_FMOD_CHANNELSTOP 50

#endif