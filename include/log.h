/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 20:21:24 by raho              #+#    #+#             */
/*   Updated: 2023/02/07 16:23:59 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# define LOG_NORMAL 0
# define LOG_WARNING 1
# define LOG_FATAL 2
# define LOG_EC_SDL_INIT 3
# define LOG_EC_SDL_CREATEWINDOW 4
# define LOG_EC_SDL_GETWINDOW_SURFACE 5
# define LOG_EC_SDL_CREATERGBSURFACE 6
# define LOG_EC_SDL_UPDATEWINDOWSURFACE 7
# define LOG_EC_SDL_SETRELATIVEMOUSEMODE 8
# define LOG_EC_SDL_GAMECONTROLLEROPEN 9
# define LOG_EC_SDL_JOYSTICKINSTANCEID 10
# define LOG_EC_SDL_OPENAUDIODEVICE 11
# define LOG_EC_SDL_LOADWAV 12
# define LOG_EC_SDL_QUEUEAUDIO 13
# define LOG_EC_TTF_OPENFONT 14
# define LOG_EC_TTF_RENDERTEXTBLENDED 15
# define LOG_EC_OPEN 30
# define LOG_EC_CLOSE 31
# define LOG_EC_WRITE 32
# define LOG_EC_MALLOC 33
# define LOG_EC_GETNEXTLINE 34
# define LOG_EC_FMOD_SYSTEMCREATE 50
# define LOG_EC_FMOD_SYSTEMINIT 51
# define LOG_EC_FMOD_SYSTEMCLOSE 52
# define LOG_EC_FMOD_SYSTEMRELEASE 53
# define LOG_EC_FMOD_SYSTEMCREATESOUND 54
# define LOG_EC_FMOD_SYSTEMPLAYSOUND 55
# define LOG_EC_FMOD_CHANNELSETVOLUME 56
# define LOG_EC_FMOD_CHANNELSETPAUSED 57
# define LOG_EC_FMOD_CHANNELSET3DATTRIBUTES 58
# define LOG_EC_FMOD_CHANNELISPLAYING 59
# define LOG_EC_FMOD_CHANNELSTOP 60
# define LOG_EC_FMOD_CHANNELGETCURRENTSOUND 61

/* code defines start with LOG */
void	doomlog(int code, char *str);

/* example: doomlog_mul(LOG_NORMAL, (char *[32]){"parsed", s_itoa(i), "imagefiles", NULL}); */
void	doomlog_mul(int code, char **str);

/* don't use this manually, call doomlog() instead */
void	error_codes(int ec, int fd);

char	*signal_name(int wait_status);

void	normal_message_mul(int fd, char **str);

/* converts integer to a stack memory string */
char *s_itoa(int i);

#endif