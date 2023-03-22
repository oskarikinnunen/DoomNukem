/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_codes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:53:51 by raho              #+#    #+#             */
/*   Updated: 2023/03/08 17:05:29 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

/* Haven't checked yet whether SDL will leak if SDL_Quit hasn't been called 
** before exiting. For now, SDL_Quit is only called when ESC is pressed.
*/

static void	sdl_errors(int error_code, int fd)
{
	if (error_code == LOG_EC_SDL_INIT)
		ft_putstr_fd("SDL_Init failed: ", fd);
	if (error_code == LOG_EC_SDL_CREATEWINDOW)
		ft_putstr_fd("SDL_CreateWindow failed: ", fd);
	if (error_code == LOG_EC_SDL_GETWINDOW_SURFACE)
		ft_putstr_fd("SDL_GetWindowSurface failed: ", fd);
	if (error_code == LOG_EC_SDL_CREATERGBSURFACE)
		ft_putstr_fd("SDL_CreateRGBSurface failed: ", fd);
	if (error_code == LOG_EC_SDL_UPDATEWINDOWSURFACE)
		ft_putstr_fd("SDL_UpdateWindowSurface failed: ", fd);
	if (error_code == LOG_EC_SDL_SETRELATIVEMOUSEMODE)
		ft_putstr_fd("SDL_SetRelativeMouseMode failed: ", fd);
	if (error_code == LOG_EC_SDL_GAMECONTROLLEROPEN)
		ft_putstr_fd("SDL_GameControllerOpen failed: ", fd);
	if (error_code == LOG_EC_SDL_JOYSTICKINSTANCEID)
		ft_putstr_fd("SDL_JoystickInstanceID failed: ", fd);
	if (error_code == LOG_EC_TTF_OPENFONT)
		ft_putstr_fd("TTF_OpenFont failed: ", fd);
	if (error_code == LOG_EC_TTF_RENDERTEXTBLENDED)
		ft_putstr_fd("TTF_RenderText_Blended failed: ", fd);
	ft_putendl_fd(SDL_GetError(), fd);
}

static void	std_fnc_errors(int ec, int fd)
{
	if (ec == LOG_EC_OPEN)
		ft_putendl_fd(combine_strings((char *[3]){
				"Open failed -", strerror(errno)}), fd);
	if (ec == LOG_EC_CLOSE)
		ft_putendl_fd(combine_strings((char *[3]){
				"Close failed -", strerror(errno)}), fd);
	if (ec == LOG_EC_WRITE)
		ft_putendl_fd(combine_strings((char *[3]){
				"Write failed -", strerror(errno)}), fd);
	if (ec == LOG_EC_MALLOC)
		ft_putendl_fd(combine_strings((char *[3]){
				"Malloc failed -", strerror(errno)}), fd);
	if (ec == LOG_EC_GETNEXTLINE)
		ft_putendl_fd("get_next_line failed", fd);
	if (ec == LOG_EC_FORK)
		ft_putendl_fd(combine_strings((char *[3]){
				"Fork failed -", strerror(errno)}), fd);
	if (ec == LOG_EC_READ)
		ft_putendl_fd(combine_strings((char *[3]){
				"Read failed -", strerror(errno)}), fd);
	if (ec == LOG_EC_LSEEK)
		ft_putendl_fd(combine_strings((char *[3]){
				"Lseek failed -", strerror(errno)}), fd);
}

static void	fmod_errors(int ec, int fd)
{
	if (ec == LOG_EC_FMOD_SYSTEMCREATE)
		ft_putendl_fd("FMOD_System_Create failed", fd);
	if (ec == LOG_EC_FMOD_SYSTEMINIT)
		ft_putendl_fd("FMOD_System_Init failed", fd);
	if (ec == LOG_EC_FMOD_SYSTEMCLOSE)
		ft_putendl_fd("FMOD_System_Close failed", fd);
	if (ec == LOG_EC_FMOD_SYSTEMRELEASE)
		ft_putendl_fd("FMOD_System_Release failed", fd);
	if (ec == LOG_EC_FMOD_SYSTEMCREATESOUND)
		ft_putendl_fd("FMOD_System_CreateSound failed", fd);
	if (ec == LOG_EC_FMOD_SYSTEMPLAYSOUND)
		ft_putendl_fd("FMOD_System_PlaySound failed", fd);
	if (ec == LOG_EC_FMOD_CHANNELSETVOLUME)
		ft_putendl_fd("FMOD_Channel_SetVolume failed", fd);
	if (ec == LOG_EC_FMOD_CHANNELSETPAUSED)
		ft_putendl_fd("FMOD_Channel_SetPaused failed", fd);
	if (ec == LOG_EC_FMOD_CHANNELSET3DATTRIBUTES)
		ft_putendl_fd("FMOD_Channel_Set3DAttributes failed", fd);
	if (ec == LOG_EC_FMOD_CHANNELISPLAYING)
		ft_putendl_fd("FMOD_Channel_IsPlaying failed", fd);
	if (ec == LOG_EC_FMOD_CHANNELSTOP)
		ft_putendl_fd("FMOD_Channel_Stop failed", fd);
	if (ec == LOG_EC_FMOD_CHANNELGETCURRENTSOUND)
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
