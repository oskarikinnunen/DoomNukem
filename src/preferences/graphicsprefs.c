/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphicsprefs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:37:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/22 16:58:50 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "file_io.h"
#include "doomnukem.h"

t_graphicprefs defaultprefs()
{
	t_graphicprefs	prefs;

	prefs.screenmode = screenmode_windowed;
	prefs.resolution_x = 1280;
	prefs.resolution_y = 720;
	prefs.resolutionscale = 1.0f;
	prefs.volume = 5.0f;
	return (prefs);
}

/*void	editor_load_prefs(t_editor *ed, t_sdlcontext *sdl)
{
	char			*prefpath;
	char			*pref_filename;
	t_editorprefs	prefs;
	int				fd;
	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(256);
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "editor.pref");
	fd = open(pref_filename, O_RDONLY);
	if (fd != -1)
	{
		read(fd, &prefs, sizeof(t_editorprefs));
		ed->player.transform.position = prefs.playerpos;
		editor_load_and_init_world(ed, prefs.worldname, sdl);
		printf("loading prefs from '%s' , worldname %s \n", prefpath, prefs.worldname);
	}
	else
		editor_load_and_init_world(ed, "worlds/test123.world", sdl);
	SDL_free(prefpath);
	free(pref_filename);
}*/

t_graphicprefs get_prefs_from_sdl(t_sdlcontext *sdl)
{
	t_graphicprefs	prefs;

	prefs.screenmode = screenmode_windowed;
	prefs.resolution_x = sdl->window_w;
	prefs.resolution_y = sdl->window_h;
	prefs.resolutionscale = sdl->resolution_scaling;
	prefs.volume = sdl->audio.sfx_volume;
	return (prefs);
}

void	reset_graphics_prefs()
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
	/*fd = fileopen(pref_filename, O_CREAT | O_TRUNC);
	if (fd != -1)
	{
		close(fd);
	}*/
	SDL_free(prefpath);
	free(pref_filename);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

void	apply_graphics_prefs(t_graphicprefs prefs)
{
	char			*prefpath;
	char			*pref_filename;
	int				fd;

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

void	save_graphics_prefs(t_sdlcontext *sdl)
{
	char			*prefpath;
	char			*pref_filename;
	int				fd;
	t_graphicprefs	prefs;

	prefs = get_prefs_from_sdl(sdl);
	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(ft_strlen(prefpath) + 20);
	if (pref_filename == NULL)
		doomlog(LOG_EC_MALLOC, "save_graphics_prefs");
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "graphics.pref");
	fd = fileopen(pref_filename, O_RDWR | O_CREAT | O_TRUNC);
	if (write(fd, &prefs, sizeof(prefs)) == -1)
		doomlog(LOG_EC_WRITE, "apply_graphics_prefs");
	fileclose(fd, pref_filename);
	SDL_free(prefpath);
	free(pref_filename);
}

#pragma GCC diagnostic pop

t_graphicprefs	load_graphicsprefs()
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
		int rd = read(fd, &prefs, sizeof(prefs));
		if (rd != sizeof(prefs)) //TODO: add other protections?
			prefs = defaultprefs();
		fileclose(fd, pref_filename);
	}
	SDL_free(prefpath);
	free(pref_filename);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
		"prefs resolution", s_itoa(prefs.resolution_x), s_itoa(prefs.resolution_y), NULL});
	return (prefs);
}