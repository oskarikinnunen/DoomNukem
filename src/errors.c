/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:53:51 by raho              #+#    #+#             */
/*   Updated: 2023/01/31 16:29:06 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

/* Haven't checked yet whether SDL will leak if SDL_Quit hasn't been called 
** before exiting. For now, SDL_Quit is only called when ESC is pressed.
*/

static void	sdl_errors(int error_code, int fd, void (*f)(const char *, int))
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
	f(SDL_GetError(), fd);
}

static void	std_fnc_errors(int ec, int fd, void (*f)(const char *, int))
{
	if (ec == LOGEC_OPEN)
		f("Open failed", fd);
	if (ec == LOGEC_CLOSE)
		f("Close failed", fd);
	if (ec == LOGEC_WRITE)
		f("Write failed", fd);
	if (ec == LOGEC_MALLOC)
		f("Malloc failed", fd);
	if (ec == LOGEC_GETNEXTLINE)
		f("get_next_line failed", fd);
}

static void	fmod_errors(int ec, int fd, void (*f)(const char *, int))
{
	if (ec == LOGEC_FMOD_SYSTEMCREATE)
		f("FMOD_System_Create failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMINIT)
		f("FMOD_System_Init failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMCLOSE)
		f("FMOD_System_Close failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMRELEASE)
		f("FMOD_System_Release failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMCREATESOUND)
		f("FMOD_System_CreateSound failed", fd);
	if (ec == LOGEC_FMOD_SYSTEMPLAYSOUND)
		f("FMOD_System_PlaySound failed", fd);
	if (ec == LOGEC_FMOD_CHANNELSETVOLUME)
		f("FMOD_Channel_SetVolume failed", fd);
	if (ec == LOGEC_FMOD_CHANNELSETPAUSED)
		f("FMOD_Channel_SetPaused failed", fd);
	if (ec == LOGEC_FMOD_CHANNELSET3DATTRIBUTES)
		f("FMOD_Channel_Set3DAttributes failed", fd);
	if (ec == LOGEC_FMOD_CHANNELISPLAYING)
		f("FMOD_Channel_IsPlaying failed", fd);
	if (ec == LOGEC_FMOD_CHANNELSTOP)
		f("FMOD_Channel_Stop failed", fd);
	if (ec == LOGEC_FMOD_CHANNELGETCURRENTSOUND)
		f("FMOD_Channel_GetCurrentSound failed", fd);
}

void	choose_error(int ec, int fd, void (*f)(int, int, void(*f)))
{
	
}

void	errors(int ec, int fd, char *str)
{
	if (ec >= 2 && ec <= 20)
	{
		if (str)
		{
			sdl_errors(ec, fd, ft_putstr_fd);
			ft_putstr_fd(" - ", fd);
			ft_putendl_fd(str, fd);
		}
		else
			sdl_errors(ec, fd, ft_putendl_fd);
	}
	if (ec >= 30 && ec <= 45)
	{
		if (str)
		{
			std_fnc_errors(ec, fd, ft_putstr_fd);
			ft_putstr_fd(" - ", fd);
			ft_putendl_fd(str, fd);
		}
		else
			std_fnc_errors(ec, fd, ft_putendl_fd);
	}
	if (ec >= 50)
	{
		if (str)
		{
			fmod_errors(ec, fd, ft_putstr_fd);
			ft_putstr_fd(" - ", fd);
			ft_putendl_fd(str, fd);
		}
		else
			fmod_errors(ec, fd, ft_putendl_fd);	
	}
	exit (1);
}
