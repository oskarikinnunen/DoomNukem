/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:53:51 by raho              #+#    #+#             */
/*   Updated: 2023/02/03 22:42:39 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

/* Haven't checked yet whether SDL will leak if SDL_Quit hasn't been called 
** before exiting. For now, SDL_Quit is only called when ESC is pressed.
*/

static void	sdl_errors(int error_code, int fd)
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
}

static void	std_fnc_errors(int ec, int fd)
{
	if (ec == LOGEC_OPEN)
		ft_putendl_fd("Open failed", fd);
	if (ec == LOGEC_CLOSE)
		ft_putendl_fd("Close failed", fd);
	if (ec == LOGEC_WRITE)
		ft_putendl_fd("Write failed", fd);
	if (ec == LOGEC_MALLOC)
		ft_putendl_fd("Malloc failed", fd);
	if (ec == LOGEC_GETNEXTLINE)
		ft_putendl_fd("get_next_line failed", fd);
}

static void	fmod_errors(int ec, int fd)
{
	if (ec == LOGEC_FMOD_SYSTEMCREATE)
		ft_putendl_fd("FMOD_System_Create failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMINIT)
		ft_putendl_fd("FMOD_System_Init failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMCLOSE)
		ft_putendl_fd("FMOD_System_Close failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMRELEASE)
		ft_putendl_fd("FMOD_System_Release failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMCREATESOUND)
		ft_putendl_fd("FMOD_System_CreateSound failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMPLAYSOUND)
		ft_putendl_fd("FMOD_System_PlaySound failed", fd);
	if (ec == LOGEC_FMOD_CHANNELSETVOLUME)
		ft_putendl_fd("FMOD_Channel_SetVolume failed", fd);
	if (ec == LOGEC_FMOD_CHANNELSETPAUSED)
		ft_putendl_fd("FMOD_Channel_SetPaused failed", fd);
	if (ec == LOGEC_FMOD_CHANNELSET3DATTRIBUTES)
		ft_putendl_fd("FMOD_Channel_Set3DAttributes failed", fd);
	if (ec == LOGEC_FMOD_CHANNELISPLAYING)
		ft_putendl_fd("FMOD_Channel_IsPlaying failed", fd);
	if (ec == LOGEC_FMOD_CHANNELSTOP)
		ft_putendl_fd("FMOD_Channel_Stop failed", fd);
	if (ec == LOGEC_FMOD_CHANNELGETCURRENTSOUND)
		ft_putendl_fd("FMOD_Channel_GetCurrentSound failed", fd);
}

void	error_codes(int ec, int fd)
{
	if (ec >= 3 && ec <= 20)
		sdl_errors(ec, fd);
	else if (ec >= 30 && ec <= 45)
		std_fnc_errors(ec, fd);
	else if (ec >= 50 && ec <= 70)
		fmod_errors(ec, fd);
	else
		ft_putendl_fd("invalid log/error code", fd);
}
