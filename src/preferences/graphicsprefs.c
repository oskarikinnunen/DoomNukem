/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphicsprefs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:37:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 21:16:11 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "file_io.h"
#include "doomnukem.h"

t_graphicprefs	defaultprefs(void)
{
	t_graphicprefs	prefs;

	prefs.screenmode = screenmode_windowed;
	prefs.resolution_x = 1280;
	prefs.resolution_y = 720;
	prefs.resolutionscale = 0.5f;
	prefs.volume = 5.0f;
	return (prefs);
}

t_graphicprefs	get_prefs_from_sdl(t_sdlcontext *sdl)
{
	t_graphicprefs	prefs;

	prefs.screenmode = screenmode_windowed;
	prefs.resolution_x = sdl->window_w;
	prefs.resolution_y = sdl->window_h;
	prefs.resolutionscale = sdl->resolution_scaling;
	prefs.volume = sdl->audio.sfx_volume;
	return (prefs);
}

void	reset_graphics_prefs(void)
{
	char			*prefpath;
	char			*pref_filename;
	int				fd;

	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(ft_strlen(prefpath) + 20);
	if (pref_filename == NULL)
		doomlog(LOG_EC_MALLOC, "reset graphics pref filename");
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "graphics.pref");
	remove(pref_filename);
	SDL_free(prefpath);
	free(pref_filename);
}

void	apply_graphics_prefs(t_graphicprefs prefs)
{
	char			*prefpath;
	char			*pref_filename;
	int				fd;

	printf("applying gprefs\n");
	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(ft_strlen(prefpath) + 20);
	if (pref_filename == NULL)
		doomlog(LOG_EC_MALLOC, "apply_graphics_prefs");
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "graphics.pref");
	fd = fileopen(pref_filename, O_RDWR | O_CREAT | O_TRUNC);
	if (write(fd, &prefs, sizeof(prefs)) == -1)
		doomlog(LOG_EC_WRITE, "apply_graphics_prefs");
	fileclose(fd, pref_filename);
	SDL_free(prefpath);
	free(pref_filename);
}

t_graphicprefs	load_graphicsprefs(void)
{
	char			*prefpath;
	char			*pref_filename;
	int				fd;
	t_graphicprefs	prefs;

	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(ft_strlen(prefpath) + 20);
	if (pref_filename == NULL)
		doomlog(LOG_EC_MALLOC, "load_graphicsprefs filename");
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "graphics.pref");
	fd = open(pref_filename, O_RDONLY);
	if (fd == -1)
		prefs = defaultprefs();
	else
	{
		if (read(fd, &prefs, sizeof(prefs)) != sizeof(prefs))
			prefs = defaultprefs();
		fileclose(fd, pref_filename);
	}
	SDL_free(prefpath);
	free(pref_filename);
	doomlog_mul(LOG_NORMAL, (char *[4]){"prefs resolution", \
			s_pitoa((t_point){prefs.resolution_x, prefs.resolution_y}), NULL});
	return (prefs);
}
