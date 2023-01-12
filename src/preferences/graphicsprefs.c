/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphicsprefs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:37:15 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/11 16:17:59 by okinnune         ###   ########.fr       */
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
	return (prefs);
}

void	reset_graphics_prefs()
{
	char			*prefpath;
	char			*pref_filename;
	int				fd;

	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(ft_strlen(prefpath) + 20);
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

void	apply_graphics_prefs(t_graphicprefs prefs)
{
	char			*prefpath;
	char			*pref_filename;
	int				fd;

	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(ft_strlen(prefpath) + 20);
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "graphics.pref");
	fd = fileopen(pref_filename, O_RDWR | O_CREAT | O_TRUNC);
	if (fd != -1)
	{
		write(fd, &prefs, sizeof(prefs));
		close(fd);
	}
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
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "graphics.pref");
	fd = fileopen(pref_filename, O_RDWR | O_CREAT | O_TRUNC);
	if (fd != -1)
	{
		write(fd, &prefs, sizeof(prefs));
		close(fd);
	}
	SDL_free(prefpath);
	free(pref_filename);
}

t_graphicprefs	load_graphicsprefs()
{
	char			*prefpath;
	char			*pref_filename;
	int				fd;
	t_graphicprefs	prefs;

	
	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(ft_strlen(prefpath) + 20);
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "graphics.pref");
	fd = fileopen(pref_filename, O_RDONLY);
	if (fd == -1)
		prefs = defaultprefs();
	else
	{
		int rd = read(fd, &prefs, sizeof(prefs));
		if (rd != sizeof(prefs)) //TODO: add other protections?
			prefs = defaultprefs();
		close(fd);
	}
	SDL_free(prefpath);
	free(pref_filename);
	printf("prefs res %i %i \n", prefs.resolution_x, prefs.resolution_y);
	return (prefs);
}