/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:53:51 by raho              #+#    #+#             */
/*   Updated: 2023/01/28 17:14:10 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

/* Haven't checked yet whether SDL will leak if SDL_Quit hasn't been called 
** before exiting. For now, SDL_Quit is only called when ESC is pressed.
*/

static void	sdl_errors(int error_code, int fd, char *str)
{
	if (error_code == LOGEC_SDL_INIT)
		ft_putstr_fd("SDL_Init failed: ", fd);
	if (error_code == LOGEC_SDL_CREATEWINDOW)
		ft_putstr_fd("SDL_CreateWindow failed: ", fd);
	if (error_code == LOGEC_SDL_GETWINDOW_SURFACE)
		ft_putstr_fd("SDL_GetWindowSurface failed: ", fd);
	if (error_code == LOGEC_SDL_CREATERGBSURFACE)
		ft_putstr_fd("SDL_CreateRGBSurface failed: ", fd);
	if (error_code == LOGEC_SDL_UPDATEWINDOWSURFACE)
		ft_putstr_fd("SDL_UpdateWindowSurface failed: ", fd);
	if (error_code == LOGEC_SDL_SETRELATIVEMOUSEMODE)
		ft_putstr_fd("SDL_SetRelativeMouseMode failed: ", fd);
	if (error_code == LOGEC_SDL_GAMECONTROLLEROPEN)
		ft_putstr_fd("SDL_GameControllerOpen failed: ", fd);
	if (error_code == LOGEC_SDL_JOYSTICKINSTANCEID)
		ft_putstr_fd("SDL_JoystickInstanceID failed: ", fd);
	if (error_code == LOGEC_TTF_OPENFONT)
		ft_putstr_fd("TTF_OpenFont failed: ", fd);
	if (error_code == LOGEC_TTF_RENDERTEXTBLENDED)
		ft_putstr_fd("TTF_RenderText_Blended failed: ", fd);
	ft_putendl_fd(SDL_GetError(), fd);
	if (str)
		ft_putendl_fd(str, fd);
}

static void	standard_function_errors(int error_code, int fd, char *str)
{
	if (error_code == LOGEC_OPEN)
		ft_putendl_fd("Open failed\n", fd);
	if (error_code == LOGEC_CLOSE)
		ft_putendl_fd("Close failed\n", fd);
	if (error_code == LOGEC_WRITE)
		ft_putendl_fd("Write failed\n", fd);
	if (error_code == LOGEC_MALLOC)
		ft_putendl_fd("Malloc failed\n", fd);
	if (error_code == LOGEC_GETNEXTLINE)
		ft_putendl_fd("get_next_line failed\n", fd);
	if (str)
		ft_putendl_fd(str, fd);
}

static void	fmod_errors(int error_code, int fd, char *str)
{
	if (error_code == LOGEC_FMOD_SYSTEMCREATE)
		ft_putendl_fd("FMOD_System_Create failed\n", fd);
	if (error_code == LOGEC_FMOD_SYSTEMINIT)
		ft_putendl_fd("FMOD_System_Init failed\n", fd);
	if (error_code == LOGEC_FMOD_SYSTEMCLOSE)
		ft_putendl_fd("FMOD_System_Close failed\n", fd);
	if (error_code == LOGEC_FMOD_SYSTEMRELEASE)
		ft_putendl_fd("FMOD_System_Release failed\n", fd);
	if (error_code == LOGEC_FMOD_SYSTEMCREATESOUND)
		ft_putendl_fd("FMOD_System_CreateSound failed\n", fd);
	if (error_code == LOGEC_FMOD_SYSTEMPLAYSOUND)
		ft_putendl_fd("FMOD_System_PlaySound failed\n", fd);
	if (error_code == LOGEC_FMOD_CHANNELSETVOLUME)
		ft_putendl_fd("FMOD_Channel_SetVolume failed\n", fd);
	if (error_code == LOGEC_FMOD_CHANNELSETPAUSED)
		ft_putendl_fd("FMOD_Channel_SetPaused failed\n", fd);
	if (error_code == LOGEC_FMOD_CHANNELSET3DATTRIBUTES)
		ft_putendl_fd("FMOD_Channel_Set3DAttributes failed\n", fd);
	if (error_code == LOGEC_FMOD_CHANNELISPLAYING)
		ft_putendl_fd("FMOD_Channel_IsPlaying failed\n", fd);
	if (error_code == LOGEC_FMOD_CHANNELSTOP)
		ft_putendl_fd("FMOD_Channel_Stop failed\n", fd);
	if (error_code == LOGEC_FMOD_CHANNELGETCURRENTSOUND)
		ft_putendl_fd("FMOD_Channel_GetCurrentSound failed\n", fd);
	if (str)
		ft_putendl_fd(str, fd);
}

void	errors(int error_code, int fd, char *str)
{
	if (error_code < 30)
		sdl_errors(error_code, fd, str);
	if (error_code >= 30 && error_code < 50)
		standard_function_errors(error_code, fd, str);
	if (error_code >= 50)
		fmod_errors(error_code, fd, str);
}
