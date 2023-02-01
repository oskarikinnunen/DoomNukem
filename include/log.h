/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 20:21:24 by raho              #+#    #+#             */
/*   Updated: 2023/02/01 20:18:08 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# define LOG_NORMAL 0
# define LOG_WARNING 1
# define LOGEC_SDL_INIT 2
# define LOGEC_SDL_CREATEWINDOW 3
# define LOGEC_SDL_GETWINDOW_SURFACE 4
# define LOGEC_SDL_CREATERGBSURFACE 5
# define LOGEC_SDL_UPDATEWINDOWSURFACE 6
# define LOGEC_SDL_SETRELATIVEMOUSEMODE 7
# define LOGEC_SDL_GAMECONTROLLEROPEN 8
# define LOGEC_SDL_JOYSTICKINSTANCEID 9
# define LOGEC_SDL_OPENAUDIODEVICE 10
# define LOGEC_SDL_LOADWAV 11
# define LOGEC_SDL_QUEUEAUDIO 12
# define LOGEC_TTF_OPENFONT 13
# define LOGEC_TTF_RENDERTEXTBLENDED 14
# define LOGEC_OPEN 30
# define LOGEC_CLOSE 31
# define LOGEC_WRITE 32
# define LOGEC_MALLOC 33
# define LOGEC_GETNEXTLINE 34
# define LOGEC_FMOD_SYSTEMCREATE 50
# define LOGEC_FMOD_SYSTEMINIT 51
# define LOGEC_FMOD_SYSTEMCLOSE 52
# define LOGEC_FMOD_SYSTEMRELEASE 53
# define LOGEC_FMOD_SYSTEMCREATESOUND 54
# define LOGEC_FMOD_SYSTEMPLAYSOUND 55
# define LOGEC_FMOD_CHANNELSETVOLUME 56
# define LOGEC_FMOD_CHANNELSETPAUSED 57
# define LOGEC_FMOD_CHANNELSET3DATTRIBUTES 58
# define LOGEC_FMOD_CHANNELISPLAYING 59
# define LOGEC_FMOD_CHANNELSTOP 60
# define LOGEC_FMOD_CHANNELGETCURRENTSOUND 61

/* code defines start with LOG */
void	doomlog(int code, char *str);

/* example: doomlog_mul(LOG_NORMAL, (char *[32]){"parsed", s_itoa(i), "imagefiles", NULL}); */
void	doomlog_mul(int code, char **str);

/* don't use this manually, call doomlog() instead */
void	error_codes(int ec, int fd);

/* converts integer to a stack memory string */
char *s_itoa(int i);

#endif